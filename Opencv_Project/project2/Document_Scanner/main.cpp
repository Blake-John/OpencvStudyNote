#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

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

void getContours (Mat imgPre, Mat img)
{
    vector <vector <Point>> contours;
    
    findContours (imgPre,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
    
}


// 导入图片
int main ()
{
    string path = "/home/blake/桌面/Opencv_learn/Opencv_Project/project2/Document_Scanner/paper.jpg" ;
    Mat img = imread (path);
    imshow ("Image",img);
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