#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

float w = 400, h = 300;
Mat matrix, imgWarp;

int main ()
{
    string path = "../chapter5/vscode3.png";
    Mat img = imread (path);

    //todo warpPerspective
    //* we should create a matrix help to transform the img
    //* to create the img, we should define two assemble of points
    //* which represent the initial displacement and the ultimate displacement

    Point2f scr[4] = {{700,350},{1200,400},{600,600},{1000,700}};
    // Point2f is used to create an assemble of different point
    // 2f  proclaims the data type of the point is float
    // [4] tells that there are four points in it
    Point2f dst[4] = {{0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h}};
    // here define a assemble of the destination point

    matrix = getPerspectiveTransform (scr,dst);
    // getPerspectiveTransform (source,destination)
    // generate a matrix help to get the img transformed
    // which declares the img parameters before transform and after respectively
    warpPerspective (img,imgWarp,matrix,Point (w,h));
    // warpPerspective (input,output,matrix_helped,Size_of_ouput_img)

    //todo circle the initial points
    for (int i = 0; i < 4; i++)
    {
        circle (img,scr[i],10,Scalar (0,0,255),FILLED);
    }
    // we shouldn't put it above the transformation,
    // because the circles will appear in the transformed img
    
    imshow ("Image",img);
    imshow ("Image Warp",imgWarp);

    waitKey ();

    return 0;
}