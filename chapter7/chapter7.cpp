#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


//todo define a new function to detect the shapes
void getContours (Mat imgDila,Mat img)
{
    //* we can define an assemble of the contours with the data type of vector
    // {
    //     {Point (20,30),Point (50,60)},
    //     {},
    //     {}
    // };
    //* the assemble is also a vector,so we have the following definition:
    vector <vector <Point> > contours;
    vector <Vec4i> hierarchy;
    findContours (imgDila,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    // the output contours is a vector

    // drawContours (img,contours,-1,Scalar (255,0,255),2);
    // drawContours (output,contours,which_to_draw,color,thickness)
    // the third parameter contourIdx idicates which contour to draw,
    // if it is negative, all the contours will be drawn

    vector < vector <Point> > conPoly (contours.size ());
    vector < Rect > boundRect (contours.size ());

    //todo  we can write a filter to remove we don't want(the noise)
    for (int i = 0; i < contours.size (); i++)
    {
        int area = contourArea (contours[i]);
        cout << area << endl;



        if (area > 1000 and area != 20287)
        {
            double peri = arcLength (contours[i],true);
            // Calculates a contour perimeter or a curve length
            // arcLength (target,bool_is_closed)
            // closed – Flag indicating whether the curve is closed or not

            approxPolyDP (contours[i],conPoly[i],0.02 * peri,true);
            //* we can draw the whole contours,
            // drawContours (img,contours,i,Scalar (255,0,255),2);
            //* or we can only draw some points which are lined:
            drawContours (img,conPoly,i,Scalar (255,0,255),2);
            // cout << conPoly[i] << endl;
            // cout << contours[i].size () << endl;
            cout << conPoly[i].size () << endl;

            //todo by all the attempt above, we can draw a rectangle around the contours
            boundRect[i] = boundingRect (conPoly[i]);
            rectangle (img,boundRect[i].tl (),boundRect[i].br (),Scalar (0,255,0),5);


        }
    }
}

int main ()
{
    string path = "chapter7/colorful_shapes.jpg";
    Mat img = imread (path), imgGray, imgBlur, imgCanny, imgDila;

    //todo Shapes Detection
    //* to detect the shapes, we should pre-process the imgs
    //* we can use Canny Edge Detector to get the edge
    //* so that we can find what shape is that edge
    
    cvtColor (img,imgGray,COLOR_BGR2GRAY);
    GaussianBlur (imgGray,imgBlur,Size (7,7),3,0);
    Canny (imgBlur,imgCanny,25,75);

    Mat kernel = getStructuringElement (MORPH_RECT,Size (5,5));
    dilate (imgCanny,imgDila,kernel);

    getContours (imgDila,img);

    // imshow ("Image Canny",imgCanny);
    // imshow ("Image Dila",imgDila);
    imshow ("Image",img);
    waitKey(0);

    return 0;
}