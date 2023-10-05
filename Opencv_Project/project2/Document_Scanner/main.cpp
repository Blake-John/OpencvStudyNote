#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>

using namespace cv;
using namespace std;



Mat imgPre (Mat img)
{
    Mat imgGray, imgCanny, imgDila;
    Mat kernel = getStructuringElement (MORPH_RECT,Size (5,5));

    cvtColor (img,imgGray,COLOR_BGR2GRAY);
    Canny (imgGray,imgCanny,50,150);
    dilate (imgCanny,imgDila,kernel);

    return imgDila;
}

vector <Point> getContours (Mat imgPre,Mat img)
{
    vector <vector <Point>> contours;    
    vector <Vec4i> hierarchy;
    findContours (imgPre,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    vector <vector <Point>> conPoly (contours.size ());
    vector <Point> biggest;
    int maxArea = 1000;

    for (int i = 0; i < contours.size (); i++)
    {
        int area = contourArea (contours[i]);
        double peri = arcLength (contours[i],true);
        approxPolyDP (contours[i],conPoly[i],0.02 * peri,true);

        if (area > maxArea and conPoly[i].size () == 4)
        {

            biggest = {conPoly[i][0],conPoly[i][1],conPoly[i][2],conPoly[i][3]};
            maxArea = area;

            // drawContours (img,conPoly,i,Scalar (255,0,0),2);
        }
    }
    return biggest;
}

void drawPoint (Mat img, vector <Point> points,Scalar color)
{
    for (int i = 0; i < points.size (); i++)
    {
        circle (img,points[i],5,color,FILLED);
        putText (img,to_string (i),points[i],FONT_HERSHEY_DUPLEX,2,color,1);
    }
}

vector <Point> reorder (vector <Point> points)
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
    ordered_p.push_back (points[min_element (sumPoints.begin (),sumPoints.end ()) - sumPoints.begin ()]);// 0
    ordered_p.push_back (points[max_element (subPoints.begin (),subPoints.end ()) - subPoints.begin ()]);// 1
    ordered_p.push_back (points[min_element (subPoints.begin (),subPoints.end ()) - subPoints.begin ()]);// 2
    ordered_p.push_back (points[max_element (sumPoints.begin (),sumPoints.end ()) - sumPoints.begin ()]);// 3

    return ordered_p;
}

Mat WarpImg (Mat img, vector <Point> ordered_p, float w, float h)
{
    Mat imgWarp;
    Point2f scr[4] = {ordered_p[0],ordered_p[1],ordered_p[2],ordered_p[3]};
    Point2f dst[4] = {{0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h}};
    Mat matrix = getPerspectiveTransform (scr,dst);
    // warpPerspective (getContours (imgPre (img),img),imgWarp);
    warpPerspective (img,imgWarp,matrix,Point (w,h));

    return imgWarp;
}

Point calculate (vector <Point> ordered_p)
{
    double dxs, dys;
    vector <int> length, width;
    double wl, hl;

    for (int i = 0; i < 4; i += 2)
    {
        dxs = pow (ordered_p[i].x - ordered_p[i + 1].x,2);
        dys = pow (ordered_p[i].y - ordered_p[i + 1].y,2);
        width.push_back (sqrt (dxs + dys));
    }

    for (int i = 0; i < 2; i++)
    {
        dxs = pow (ordered_p[i].x - ordered_p[i + 2].x,2);
        dys = pow (ordered_p[i].y - ordered_p[i + 2].y,2);
        length.push_back (sqrt (dxs + dys));
    }
    wl = width[max_element (width.begin (),width.end ()) - width.begin ()];
    hl = length[max_element (length.begin (),length.end ()) - length.begin ()];

    cout << wl << hl << endl;

    Point2d a = {wl,hl};

    return a;


}

int main ()
{
    string path = "/home/blake/桌面/Opencv_learn/Opencv_Project/project2/Document_Scanner/paper.jpg" ;
    Mat img = imread (path);
    Mat imgP, imgCir, imgWarp, imgCrop;
    vector <Point> points, ordered_p;
    Mat matrix;
    double w, h;


    // preprocessing the img
    imgP = imgPre (img);
    // get the biggest contours
    points = getContours (imgP,img);
    // reorder the points
    ordered_p = reorder (points);
    // draw the point of the biggest contours(document)
    // drawPoint (img,ordered_p,Scalar (255,255,0));
    // get the img warped
    // calculate (ordered_p);
    // w = calculate (ordered_p).x;
    // h = calculate (ordered_p).y;
    w = 420*1.5;
    h = 596*1.5;
    // A4
    imgWarp = WarpImg (img,ordered_p,w,h);
    // crop the img
    int Cropvalue = 10;
    Rect roi (Cropvalue,Cropvalue,w - 2*Cropvalue,h - 2*Cropvalue);
    imgCrop = imgWarp(roi);


    imshow ("Image",img);
    imshow ("Image Dila",imgP);
    imshow ("Image Crop",imgCrop);
    waitKey (0);

    return 0;
}


// 导入视频
// 与导入图片有一点不同，因为视频是有许多图片组成的
// 我们需要遍历所有的图像或所有帧，并一一显示
// int main ()
// {
//     string path = "/home/blake/桌面/Opencv_learn/videos/ssh_introduction.mp4" ;
//     VideoCapture cap (path); // 说明使用视频模块
//     Mat img;

//     while (true)
//     {
//         cap.read (img); // 利用将cap中存储的视频读取，并存储在img中

//         imshow ("Image",img);
//         waitKey (1); // 该处的单位为ms
//     }
//     return 0;

// }

// // 网络摄像头
// int main ()
// {
//     VideoCapture cap (0); // 将视频替换为摄像头的id，电脑默认摄像头为0
//     Mat img;

//     while (true)
//     {
//         cap.read (img); // 利用将cap中获取的的视频读取，并存储在img中

//         imshow ("Image",img);
//         waitKey (20); // 该处的单位为ms
    

//     }
//     return 0;

// }