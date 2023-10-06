#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

using namespace std;
using namespace cv;

// 预处理，通过颜色阈值将图像过滤
Mat imgPre (Mat img)
{
    int mycolor[6] = {0,0,225,101,209,255};
    // int hmin = 5, smin = 0, vmin = 221;
    // int hmax = 101, smax = 209, vmax = 255;
    Mat imgHSV, mask, imgGray, imgDila;
    Scalar lower (mycolor[0],mycolor[1],mycolor[2]), upper (mycolor[3],mycolor[4],mycolor[5]);
    Mat kernel = getStructuringElement (MORPH_RECT,Size (4,4));

    cvtColor (img,imgHSV,COLOR_BGR2HSV);
    inRange (imgHSV,lower,upper,mask);
    // cvtColor (mask,imgGray,COLOR_BayerBG2GRAY);
    dilate (mask,imgDila,kernel);

    return imgDila;
    // vector <Mat> channels, channels2;
    // Mat imgChannel, imgThre, imgDila;
    // string target_color = "blue";
    // Mat kernel = getStructuringElement (MORPH_RECT,Size (3,3));
    // Mat imgPp, channel_b, channel_g;

    // split (img,channels);

    // channel_b = channels.at (0);
    // channel_g = channels.at (1);
    // channels2.push_back (channel_b);
    // channels2.push_back (channel_g);
    // merge (channels2,imgChannel);
    // threshold (imgChannel,imgThre,50,255,THRESH_BINARY);
    // // dilate (imgThre,imgDila,kernel);
    
    // return imgThre;

}

// 获取灯条的最小外接矩形
vector <RotatedRect> getContours (Mat imgPp,Mat img)
{
    vector <vector <Point>> contours;
    findContours (imgPp,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

    // 筛选，用灯条长宽比和面积筛选
    vector <RotatedRect> armorRect;
    for (int i = 0; i < contours.size (); i++)
    {
        float area;
        area = contourArea (contours[i]);

        RotatedRect roRect = minAreaRect (contours[i]);
        float aspectRatio;
        aspectRatio = max (roRect.size.width,roRect.size.height) / min (roRect.size.width,roRect.size.height);
        if (aspectRatio > 2 and aspectRatio < 8 and area > 100)
        {
            armorRect.push_back (roRect);
        }
    }

    // 绘制旋转矩形
    for (int i = 0; i < armorRect.size (); i++)
    {
        Point2f points[4];
        armorRect[i].points (points);
        for (int j = 0; j < 4; j++)
        {
            // cout << points[j] << endl;
            // line (img,points[j],points[(j + 1) % 4],Scalar (0,255,0),3);
            // circle (img,points[j],10,Scalar (0,0,255),FILLED);
            // putText (img,to_string (j),points[j],FONT_HERSHEY_DUPLEX,1,Scalar (255,255,255),1);
        }
        // putText(img,to_string (i),points[i],FONT_HERSHEY_DUPLEX,1,Scalar (255,0,255),1);
    }

    return armorRect;
}

// 计算两个点之间的距离
double getDistance (Point2f pt1,Point2f pt2)
{
    double dxs, dys,d;
    dxs = pow ((pt1.x - pt2.x),2);
    dys = pow ((pt1.y - pt2.y),2);

    d = sqrt (dxs + dys);

    return d;
}

// 计算两个点的中点
Point2f getCenter (Point2f pt1, Point2f pt2)
{
    double x, y;
    x = (pt1.x + pt2.x) / 2;
    y = (pt1.y + pt2.y) / 2;

    return Point2f (x,y);
}

// 计算装甲板中点
Point2f getArmorCenter (Point2f pt0, Point2f pt1, Point2f pt2, Point2f pt3)
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

// 对获得的矩形进行筛选，匹配灯条
vector <vector<RotatedRect>> getLights (vector <RotatedRect> armorRect, Mat img)
{
    vector <vector<RotatedRect>> light_assem;

    // 将灯条匹配
    for (int i = 0; i < armorRect.size (); i++)
    {
        for (int j = i + 1; j < armorRect.size (); j++)
        {   
            auto angle1 = armorRect[i].angle, angle2 = armorRect[j].angle;
            
            if (abs (angle1) > 60)
            {
                angle1 = abs (angle1 - 90);
            }
            if (abs (angle2) > 60)
            {
                angle2 = abs (angle2 - 90);
            }
            cout << angle1 << '\t' << angle2 << abs (angle1 - angle2) << endl;
            if (abs (angle1 - angle2) < 2.5 )
            {
                double dratio_wl =  min (armorRect[i].size.width,armorRect[i].size.height) / max (armorRect[i].size.width,armorRect[i].size.height) - \
                    min (armorRect[j].size.width,armorRect[j].size.height) / max (armorRect[j].size.width,armorRect[j].size.height);
                if (abs (dratio_wl) < 0.8)
                {
                // if (max (armorRect[i].size.width,armorRect[i].size.height) - max (armorRect[j].size.width,armorRect[j].size.height) < 10)
                // {
                // if (abs (armorRect[i].size.height - armorRect[j].size.height) < 120)
                // {
                //     if (abs (armorRect[i].size.width - armorRect[j].size.width) < 120)
                //     {
                    double d = getDistance (armorRect[i].center,armorRect[j].center);
                    double ratio_dl1 = d / max (armorRect[i].size.width,armorRect[i].size.height);
                    // double ratio_dl2 = d / max (armorRect[j].size.width,armorRect[j].size.height);

                    // double ratio2 = d / max (armorRect[j].size.width,armorRect[j].size.height);
                    if (ratio_dl1 > 2 and ratio_dl1 < 3 and d > 20)
                    {
                        vector <RotatedRect> armor;
                        armor.push_back (armorRect[i]);
                        armor.push_back (armorRect[j]);
                        light_assem.push_back (armor);

                    }
                //     }
                // }
                // }
                }
            }
        }
    }

    return light_assem;
}

// 对匹配成功的灯条进行装甲板的绘制
void getArmor (vector <vector<RotatedRect>> light_assem,Mat img)
{   
    Point2f c_b1;
    Point2f c_t1;
    Point2f c_b2;
    Point2f c_t2;

    for (auto lights : light_assem)
    {
        RotatedRect rect1 = lights[0], rect2 = lights[1];
        Point2f center1 = rect1.center, center2 = rect2.center;
        Point2f points1[4], points2[4];
        rect1.points (points1);
        rect2.points (points2);
        Point center;

        vector <Point2f> high, down;
        for (int i = 0; i < 4; i++)
        {
            if (points1[i].y > center1.y)
            {
                high.push_back (points1[i]);
            }
            else
            {
                down.push_back (points1[i]);
            }
        }

        
        c_t1 = getCenter (high[0],high[1]);
        
        c_b1 = getCenter (down[0],down[1]);

        high.clear ();
        down.clear ();

        for (int i = 0; i < 4; i++)
        {
            if (points2[i].y > center2.y)
            {
                high.push_back (points2[i]);
            }
            else
            {
                down.push_back (points2[i]);
            }
        }
        c_t2 = getCenter (high[0],high[1]);
        c_b2 = getCenter (down[0],down[1]);

        high.clear ();
        down.clear ();
        
        center = getArmorCenter (c_b1,c_t1,c_t2,c_b2);

        line (img,c_b1,c_t2,Scalar (255,255,0),2);
        line (img,c_b2,c_t1,Scalar (255,255,0),2);
        circle (img,center,5,Scalar (0,0,255),FILLED);
        circle (img,c_b1,5,Scalar (0,0,255),FILLED);
        circle (img,c_b2,5,Scalar (0,0,255),FILLED);
        circle (img,c_t1,5,Scalar (0,0,255),FILLED);
        circle (img,c_t2,5,Scalar (0,0,255),FILLED);




    }
}

// int main ()
// {
//     Mat img, mask;
//     string path = "/home/blake/桌面/华工机器人实验室/实习任务/正式批任务及相关资料/装甲板/装甲板.avi";
//     VideoCapture cap (path);

//     while (true)
//     {   
//         if (!cap.read (img))
//         {
//             break;
//         }

//         if (waitKey (20) >= 0)
//         {
//             break;
//         }

//         // cap.read (img);

//         mask = imgPre (img);
//         getArmor (getLights (getContours (mask,img),img),img);

        
//         imshow ("Image",img);
//         imshow ("Mask",mask);

//         // waitKey (20);
//     }
// }

int main ()
{
    string path = "/home/blake/桌面/detector/20231006-130844.png";
    Mat img = imread (path), imgPp;

    imgPp = imgPre (img);
    getArmor (getLights (getContours (imgPp,img),img),img);

    imshow ("mask", imgPp);
    imshow ("Image",img);
    waitKey (0);


    return 0;
}