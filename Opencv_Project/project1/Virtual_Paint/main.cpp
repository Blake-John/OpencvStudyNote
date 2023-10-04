#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

vector <vector <int>> myColors {
    {124,48,117,143,170,255}, // purple
    {68,72,156,102,126,255}, // green
    };
// {hmin,smin,vmin,hmax,smax,vmax}

vector <Scalar> myColorValues {
    {255,0,255}, //purple
    {0,255,0}, // green
    };

Mat img;


void getContours(Mat imgDil) 
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	 
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;
		string objectType;

		if (area > 1000) 
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
		
			int objCor = (int)conPoly[i].size();


			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
		}
	}
}


void findColor (Mat img)
{
    Mat imgHSV, mask;

    cvtColor (img,imgHSV,COLOR_BGR2HSV);

    for (int i = 0; i < myColors.size (); i++)
    {
        Scalar lower (myColors[i][0],myColors[i][1],myColors[i][2]);
        Scalar upper (myColors[i][3],myColors[i][4],myColors[i][5]);
        
        inRange (img,lower,upper,mask);

        // imshow (to_string (i),mask);
        getContours (mask);

    }

}


int main ()
{
    VideoCapture cap (0);

    while (true)
    {
        cap.read (img);
        findColor (img);

        imshow ("Webcam",img);
        waitKey (1);
    }



    return 0;
}