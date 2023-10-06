#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

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
        if (aspectRatio > 3.0 and area > 300.0)
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
            // circle (img,points[j],10,Scalar (0,0,255),FILLED);
            // putText (img,to_string (j),points[j],FONT_HERSHEY_DUPLEX,3,Scalar (255,255,255),2);
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
                if (abs (armorRect[i].size.height - armorRect[j].size.height) < 10)
                {
                    if (abs (armorRect[i].size.width - armorRect[j].size.width) < 10)
                    {
                        double d = getDistance (armorRect[i].center,armorRect[j].center);
                        if (d / max (armorRect[i].size.width,armorRect[i].size.height) < 3 and d / max (armorRect[i].size.width,armorRect[i].size.height) > 2)
                        {
                            vector <RotatedRect> armor;
                            armor.push_back (armorRect[i]);
                            armor.push_back (armorRect[j]);
                            light_assem.push_back (armor);

                        }
                    }
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
        i[0].points (points1);
        i[1].points (points2);

        pt0.x = (points1[0].x + points1[3].x) / 2;
        pt0.y = (points1[0].y + points1[3].y) / 2;

        pt1.x = (points1[1].x + points1[2].x) / 2;
        pt1.y = (points1[1].y + points1[2].y) / 2;

        pt2.x = (points2[1].x + points2[2].x) / 2;
        pt2.y = (points2[1].y + points2[2].y) / 2;

        pt3.x = (points2[0].x + points2[3].x) / 2;
        pt3.y = (points2[0].y + points2[3].y) / 2;

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
    int mycolor[6] = {10,77,99,33,248,255};
    string path = "/home/blake/桌面/华工机器人实验室/实习任务/正式批任务及相关资料/装甲板/装甲板.avi";
    VideoCapture cap (path);
    // string path = "/home/blake/桌面/detector/color_pick/target.png";
    Mat img, imgHSV, mask;
    Scalar lower (mycolor[0],mycolor[1],mycolor[2]), upper (mycolor[3],mycolor[4],mycolor[5]);

    while (true)
    {   
        cap.read (img);

        cvtColor (img,imgHSV,COLOR_BGR2HSV);
        inRange (imgHSV,lower,upper,mask);

        getArmor (getContours (mask,img),img);
        

        imshow ("Image",img);
        imshow ("Mask",mask);

        waitKey (20);
    }

}