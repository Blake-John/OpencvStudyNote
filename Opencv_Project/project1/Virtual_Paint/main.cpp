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

vector <vector <int>> newPoint;


Point getContours(Mat imgDil) 
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	Point myPoint (0,0);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		// cout << area << endl;
		string objectType;

		if (area > 1000) 
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			// cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
		
            myPoint.x = boundRect[i].x + boundRect[i].width / 2;
            myPoint.y = boundRect[i].y + boundRect[i].height;

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
            rectangle (img,boundRect[i].tl (),boundRect[i].br (),Scalar (0,255,0),5);
		}
	}
    return myPoint;
}


vector <vector <int>> findColor (Mat img)
{
    Mat imgHSV, mask;

    cvtColor (img,imgHSV,COLOR_BGR2HSV);

    for (int i = 0; i < myColors.size (); i++)
    {
        Scalar lower (myColors[i][0],myColors[i][1],myColors[i][2]);
        Scalar upper (myColors[i][3],myColors[i][4],myColors[i][5]);
        
        inRange (img,lower,upper,mask);

        // imshow (to_string (i),mask);
        Point myPoint = getContours (mask);
        
        if (myPoint.x != 0 and myPoint.y != 0)
        {
            newPoint.push_back ({myPoint.x,myPoint.y,i});
        }
    }
    return newPoint;
}


void drawPoint (vector <vector <int>> newPoint,vector <Scalar> myColorValues)
{
    for (int i = 0; i < newPoint.size (); i++)
    {
        circle (img,Point (newPoint[i][0],newPoint[i][1]),10,myColorValues[newPoint[i][2]],FILLED);
    }
}

int main ()
{
    VideoCapture cap (0);

    while (true)
    {
        cap.read (img);
        drawPoint (findColor (img),myColorValues);

        imshow ("Webcam",img);
        waitKey (1);
    }



    return 0;
}