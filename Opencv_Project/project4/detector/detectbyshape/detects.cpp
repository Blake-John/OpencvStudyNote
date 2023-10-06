#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

using namespace std;
using namespace cv;

Mat imgPre (Mat img)
{
    Mat imgGray, imgBlur, imgThre, imgPre;
    
    cvtColor (img,imgGray,COLOR_BGR2GRAY);
    GaussianBlur (imgGray,imgBlur,Size (3,3),0);
    threshold (imgBlur,imgThre,100,255,THRESH_BINARY);
    // bitwise_not (imgThre,imgPre);

    return imgThre;

}

vector <vector <RotatedRect>> reorderRect (vector <vector<RotatedRect>> light_assem)
{
    vector <vector<RotatedRect>> reor_light_assem;
    for (int i = 0; i < light_assem.size (); i++)
    {
        vector<RotatedRect> reor_rect_assem;

        if (light_assem[i][0].center.x < light_assem[i][1].center.x)
        {
            reor_rect_assem.push_back (light_assem[i][0]);
            reor_rect_assem.push_back (light_assem[i][1]);
        }
        else if (light_assem[i][0].center.x > light_assem[i][1].center.x)
        {
            reor_rect_assem.push_back (light_assem[i][1]);
            reor_rect_assem.push_back (light_assem[i][0]);
        }

        reor_light_assem.push_back (reor_rect_assem);
        
    }
    
    return reor_light_assem;
}

vector <Point> reorderPoints (vector <Point> points)
{
    // 利用坐标相加减得到正确的顺序：0，1，2，3
    vector <Point> ordered_p;
    vector <int> sumPoints, subPoints;


    for (int i = 0; i < points.size (); i++)
    {
        sumPoints.push_back (points[i].x + points[i].y);
        subPoints.push_back (points[i].x - points[i].y);
    }
    // max_element 和 min_element 能返回指向区间元素的最大最小值的迭代器
    // 要获得其位置信息，还需要减去起始排序位置
    ordered_p.push_back (points[min_element (subPoints.begin (),subPoints.end ()) - subPoints.begin ()]);// 0
    ordered_p.push_back (points[min_element (sumPoints.begin (),sumPoints.end ()) - sumPoints.begin ()]);// 1
    ordered_p.push_back (points[max_element (subPoints.begin (),subPoints.end ()) - subPoints.begin ()]);// 2
    ordered_p.push_back (points[max_element (sumPoints.begin (),sumPoints.end ()) - sumPoints.begin ()]);// 3

    return ordered_p;
}

vector <RotatedRect> getContours (Mat imgThre,Mat img)
{
    vector <vector <Point>> contours;
    findContours (imgThre,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

    // 筛选，在此图片中用灯条长宽比和面积筛选
    vector <RotatedRect> armorRect;
    for (int i = 0; i < contours.size (); i++)
    {
        float area;
        area = contourArea (contours[i]);

        RotatedRect roRect = minAreaRect (contours[i]);
        float aspectRatio;
        aspectRatio = max (roRect.size.width,roRect.size.height) / min (roRect.size.width,roRect.size.height);
        if (aspectRatio > 3.0 and area > 50.0)
        {
            armorRect.push_back (roRect);
        }
    }

    // // 利用面积筛选
    // vector <RotatedRect> armorRect;
    // for (int i = 0; i < contours.size (); i++)
    // {
    //     RotatedRect roRect = minAreaRect (contours[i]);
    //     float area;
    //     area = contourArea (contours[i]);
    //     if (area > 50.0)
    //     {
    //         armorRect.push_back (roRect);
    //         cout << roRect.size << endl;
    //         cout << roRect.angle << endl;
    //     }
    // }

    // 绘制旋转矩形
    for (int i = 0; i < armorRect.size (); i++)
    {
        Point2f points[4];
        armorRect[i].points (points);
        cout << points << endl;
        for (int j = 0; j < 4; j++)
        {
            line (img,points[j],points[(j + 1) % 4],Scalar (0,255,0),3);
            circle (img,points[j],10,Scalar (0,0,255),FILLED);
            putText (img,to_string (j),points[j],FONT_HERSHEY_DUPLEX,2,Scalar (255,255,255),2);
        }
    }

    return armorRect;
}

double getDistance (Point2f pt1,Point2f pt2)
{
    double dxs, dys,d;
    dxs = pow ((pt1.x - pt2.x),2);
    dys = pow ((pt1.y - pt2.y),2);

    d = sqrt (dxs + dys);

    return d;
}

Point2f getCenter (Point2f pt0, Point2f pt1, Point2f pt2, Point2f pt3)
{
    double h1, h2, ratio, d1, d2, x, y;

    // d = getDistance (pt1,pt3);
    d1 = getDistance (pt0,pt1);
    d2 = getDistance (pt2,pt3);
    ratio = d1 / d2;

    x = (ratio*pt3.x + pt1.x) / (1 + ratio);
    y = (ratio*pt3.y + pt1.y) / (1 + ratio);

    return Point (x,y);

}

void getArmor (vector <RotatedRect> armorRect, Mat img)
{
    vector <vector<RotatedRect>> light_assem;

    // 将灯条匹配
    for (int i = 0; i < armorRect.size (); i++)
    {
        for (int j = i + 1; j < armorRect.size (); j++)
        {
            if (abs (armorRect[i].angle - armorRect[j].angle) < 3)
            {
                // if (abs (armorRect[i].size.height - armorRect[j].size.height) < 10)
                // {
                //     if (abs (armorRect[i].size.width - armorRect[j].size.width) < 10)
                //     {
                        double d = getDistance (armorRect[i].center,armorRect[j].center);
                        if (d / max (armorRect[i].size.width,armorRect[i].size.height) < 3 and d / max (armorRect[i].size.width,armorRect[i].size.height) > 2)
                        {
                            vector <RotatedRect> armor;
                            armor.push_back (armorRect[i]);
                            armor.push_back (armorRect[j]);
                            light_assem.push_back (armor);

                    //     }
                    // }
                }
            }
        }
    }
    vector <vector <RotatedRect>> l_a_guodu;
    l_a_guodu = reorderRect (light_assem);

    // 绘制
    for (auto i : l_a_guodu)
    {
        Point2f pt0, pt1, pt2, pt3, points1[4],points2[4];
        vector <Point> or_points, re_points;

        i[0].points (points1);
        for (int i = 0; i < 4; i ++)
        {
            or_points.push_back (points1[i]);
        }
        re_points = reorderPoints (or_points);
        
        pt0.x = (re_points[0].x + re_points[3].x) / 2;
        pt0.y = (re_points[0].y + re_points[3].y) / 2;

        pt1.x = (re_points[1].x + re_points[2].x) / 2;
        pt1.y = (re_points[1].y + re_points[2].y) / 2;

        or_points.clear ();
        re_points.clear ();


        i[1].points (points2);
        for (int i = 0; i < 4; i ++)
        {
            or_points.push_back (points2[i]);
        }
        re_points = reorderPoints (or_points);

        pt2.x = (re_points[1].x + re_points[2].x) / 2;
        pt2.y = (re_points[1].y + re_points[2].y) / 2;

        pt3.x = (re_points[0].x + re_points[3].x) / 2;
        pt3.y = (re_points[0].y + re_points[3].y) / 2;

        line (img,pt1,pt3,Scalar (255,255,0),3);
        line (img,pt0,pt2,Scalar (255,255,0),3);

        circle (img,getCenter (pt0,pt1,pt2,pt3),10,Scalar (0,255,0),FILLED);
        

    }

    // // 将匹配的灯条的点取出，用于绘制矩形
    // vector <vector <Point2f>> rect (l_a_guodu.size ());
    // for  (int i = 0; i < l_a_guodu.size (); i++)
    // {
    //     Point2f points[4];
    //     l_a_guodu[i][0].points (points);
    //     rect[i].push_back (points[0]);
    //     rect[i].push_back (points[1]);
    //     l_a_guodu[i][1].points (points);
    //     rect[i].push_back (points[2]);
    //     rect[i].push_back (points[3]);
    // }

    // // 绘制矩形
    // for (int i = 0; i < rect.size (); i++)
    // {
    //     line (img,rect[i][0],rect[i][2],Scalar (255,255,0),2);
    //     line (img,rect[i][1],rect[i][3],Scalar (255,255,0),3);
    //     float c_x, c_y;
    //     c_x = (min (min (rect[i][0].x,rect[i][1].x),min (rect[i][2].x,rect[i][3].x)) + max (max (rect[i][0].x,rect[i][1].x),max (rect[i][2].x,rect[i][3].x))) / 2;
    //     c_y = (min (min (rect[i][0].y,rect[i][1].y),min (rect[i][2].y,rect[i][3].y)) + max (max (rect[i][0].y,rect[i][1].y),max (rect[i][2].x,rect[i][3].y))) / 2;
    //     circle (img,Point (c_x,c_y),5,Scalar (0,255,0),FILLED);
    // }

}

int main ()
{
    string path = "/home/blake/桌面/华工机器人实验室/实习任务/正式批任务及相关资料/装甲板/装甲板.avi";
    VideoCapture cap (path);
    Mat img, imgThre;

    while (true)
    {
        cap.read (img);
        imgThre = imgPre (img);
        getArmor (getContours (imgThre,img),img);

        imshow ("Image Thre",imgThre);
        imshow ("Image",img);
        waitKey (50);
    }
    waitKey (0);

    return 0;
}

// int main ()
// {
//     string path = "/home/blake/桌面/detector/20231006-130844.png";
//     Mat img = imread (path), imgThre;

//     imgThre = imgPre (img);
//     getArmor (getContours (imgThre,img),img);

//     imshow ("Image Thre",imgThre);
//     imshow ("Image",img);
//     waitKey (0);


//     return 0;
// }