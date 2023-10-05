#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>

using namespace std;
using namespace cv;

int main ()
{
    VideoCapture cap (0); 
    Mat img;
    CascadeClassifier plateCascade;
    plateCascade.load ("/home/blake/桌面/Opencv_learn/Opencv_Project/project3/License Plate Detector/haarcascade_russian_plate_number.xml");
    // load the module which can detect the face

    //* check if the module is loaded properly
    if (plateCascade.empty ()) {cout << "XML file not loaded" << endl;}

    vector <Rect> plates;

    while (true)
    {
        cap.read (img);

        plateCascade.detectMultiScale (img,plates,1.1,10);
        // detectMultiScale (input_img,output_track,scaleFactor,minNeighbors)

        for (int i = 0; i < plates.size (); i++)
        {
            rectangle (img,plates[i].tl (),plates[i].br (),Scalar (255,0,255),2);
        }

        imshow ("Image",img);
        waitKey (2); 
    

    }
    return 0;

}