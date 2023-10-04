#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;


// 导入图片
int main ()
{
    string path = "/home/blake/桌面/Opencv_learn/chapter8/3.webp" ;
    Mat img = imread (path);

    CascadeClassifier faceCascade;
    faceCascade.load ("/home/blake/桌面/Opencv_learn/chapter8/haarcascade_frontalface_default.xml");
    // load the module which can detect the face

    //* check if the module is loaded properly
    if (faceCascade.empty ()) {cout << "XML file not loaded" << endl;}

    vector <Rect> faces;
    faceCascade.detectMultiScale (img,faces,1.1,10);
    // detectMultiScale (input_img,output_track,scaleFactor,minNeighbors)

    for (int i = 0; i < faces.size (); i++)
    {
        rectangle (img,faces[i].tl (),faces[i].br (),Scalar (255,0,255),2);
    }



    imshow ("Image",img);
    waitKey (0);

    return 0;
}
