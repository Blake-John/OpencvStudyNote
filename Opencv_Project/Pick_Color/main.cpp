#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
/*
创建track bar用于跟踪颜色
将图片换成摄像头，实现实时跟踪
*/ 
int main ()
{
    VideoCapture cap (0);
    Mat img, imgHSV, mask;
    int hmin = 0, smin = 0, vmin = 0;
    int hmax = 179, smax = 255, vmax = 255;


    namedWindow ("Color Picker");
    createTrackbar ("Hmin","Color Picker",&hmin,179);
    createTrackbar ("Hmax","Color Picker",&hmax,179);
    createTrackbar ("Smin","Color Picker",&smin,255);
    createTrackbar ("Smax","Color Picker",&smax,255);
    createTrackbar ("Vmin","Color Picker",&vmin,255);
    createTrackbar ("Vmax","Color Picker",&vmax,255);

    
    while (true)
    {
        cap.read (img);

        cvtColor (img,imgHSV,COLOR_BGR2HSV);
        Scalar lower (hmin,smin,vmin), upper (hmax,smax,vmax);
        // we need to change the value of the lower and upper
        inRange (imgHSV,lower,upper,mask);

        imshow ("Image",img);
        imshow ("Mask",mask);
        waitKey (1);
    }


    return 0;
}