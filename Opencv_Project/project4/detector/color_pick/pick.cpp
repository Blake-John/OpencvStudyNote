#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main ()
{
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
        img =imread ("/home/blake/桌面/detector/color_pick/target.png");

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