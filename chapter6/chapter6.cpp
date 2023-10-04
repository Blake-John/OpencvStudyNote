#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int hmin = 0, smin = 110, vmin = 153;
int hmax = 19, smax = 240, vmax = 255;

int main ()
{
    string path = "chapter6/colorful_shapes.jpg";
    Mat img = imread (path), imgHSV, mask;

    //todo Color Detection
    //* we should first convert img to hsv space
    //* in which it is easier to find the color
    //* hsv space 基本上是是色相hue、饱和度saturation和明度value
    cvtColor (img,imgHSV,COLOR_BGR2HSV);

    //* we needn't to create the bar again ad again
    namedWindow ("TrackBars",(2000,600));
    // nameWindow (windows_name,windows_size)
    createTrackbar ("Hue Min","TrackBars",&hmin,179);
    // createTrackbar (bar_name,in_which_window,linking_valuable,max_value)
    createTrackbar ("Hue Max","TrackBars",&hmax,179);
    createTrackbar ("Saturation Min","TrackBars",&smin,255);
    createTrackbar ("Saturation Max","TrackBars",&smax,255);
    createTrackbar ("Value Min","TrackBars",&vmin,255);
    createTrackbar ("Value Max","TrackBars",&vmax,255);



    while (true)
    {
        //* we use inRange to get the mask(遮罩/蒙版，即最后我们想要输出的结果)
        //* to get the mask,we should regulate the color range
        //* the mask ouput what we want in white
        //* it says when we use the track bars, we should keep what we want remain white
        Scalar lower (hmin,smin,vmin), upper (hmax,smax,vmax);
        inRange (imgHSV,lower,upper,mask);
        // inRange (inuput_img,lower_color,upper_color,output_img)

        //todo Create a track bar
        //? why to create a track bar?
        //* it's painful to get the color paremeters.it's hard and they are up to 6!
        //* so we can create a track bar to change these values in real time
        //* to achieve that, we could use while function to try again and again

        imshow ("Image",img);
        imshow ("Image HSV",imgHSV);
        imshow ("Image Mask",mask);

        waitKey (1);
        //* like the video and webcam, we can refresh it again and again
    }
    return 0;
}