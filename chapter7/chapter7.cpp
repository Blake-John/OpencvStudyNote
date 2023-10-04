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
    // 用于存放得到的多边形的点的坐标
    // 每一组坐标值连起来就对应一个多边形
    
    vector <Vec4i> hierarchy;
    findContours (imgDila,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    // findContours (input_img,output_contours,hierarchy,mode,method)
    // output_contours: Detected contours.Each contour is stored as a vector of points
    //todo 弄懂 hierarchy 的作用
    // hierarchy: containing information about the image topology
    // mode: Contour retrieval mode, see #RetrievalModes
    // method: Contour approximation method

    // drawContours (img,contours,-1,Scalar (255,0,255),2);
    // drawContours (output,contours,which_to_draw/contoursIdx,color,thickness)
    // the third parameter contourIdx idicates which contour to draw,
    // if it is negative, all the contours will be drawn

    //* we need to create a container to save the approx_result
	//* and becasue we proclaim that the container of the contours is vecter
	//* so we must match the type
    vector <vector <Point>> conPoly (contours.size ());
    // the input data contours.size () regulate the number of array
    // it says that the container could contain how many arrays
    // 我们只有 contours.size () 个形状，所以只需要计算这么多个近似多边形就可以了，故它只需要能存放这么多个计算结果就可以

    //* to draw a bounding rectangle, we need a container to save the output boundingRects
    //* as the function boundingRect input and output vector, 
    //* we should proclaim the type of the container is vector <Rect>
    vector <Rect> boundRect (contours.size ());

    string objType;

    //todo  we can write a filter to remove what we don't want(the noise)
    for (int i = 0; i < contours.size (); i++)
    // .size () will return the number of elements in the value
    {
        int area = contourArea (contours[i]);
        cout << area << endl;



        if (area > 1000 and area != 20287)
        // define the filter regulation
        {
            double peri = arcLength (contours[i],true);
            // Calculates a contour perimeter or a curve length
            // arcLength (target,bool_is_closed)
            // closed – Flag indicating whether the curve is closed or not

            approxPolyDP (contours[i],conPoly[i],0.02 * peri,true);
            // 该命令可以以指定精度计算一个曲线的近似多边形
            // 得到的 conPoly 中的某一个向量中有多少组数据，便为几边形
            // 因为 conPoly 记录的是各个点的坐标，有多少个点，便是几边形
            // 所以我们可以通过 .size () 的命令来获取每一组中的元素个数，进而知道计算结果
            // approxPolyDP (input_curve,result_of_approx,accuracy,bool_is_closed)
            // input_curve: Input vector of a 2D point stored in std::vector or Mat 
            // result_of_approx: Result of the approximation. The type should match the type of the input curve.
            // accuracy: Parameter specifying the approximation accuracy. This is the maximum distance between the original curve and its approximation

            //* we can draw the whole contours,
            // drawContours (img,contours,i,Scalar (255,0,255),2);

            //* or we can only draw some points which are lined:
            drawContours (img,conPoly,i,Scalar (255,0,255),2);
            // cout << conPoly[i] << endl;
            // cout << contours[i].size () << endl;
            cout << conPoly[i].size () << endl;

            //todo by all the attempt above, we can draw a rectangle around the contours
            boundRect[i] = boundingRect (conPoly[i]);
            // The function calculates and returns the minimal up-right(直立) bounding rectangle for the specified point set.
            // 该函数计算并返回指定点集的最小直立边框
            // 该边框以矩形数据类型存储 [width x height from (x, y)] 
            //! 注意：计算矩形数据类型内部的数据为整数
            // 可以调用以下方法： .area () .size () .tl() .br () .width () .height ()
            // 也可以获取矩形初始点： .x 获取矩形起点的横坐标 .y 获取矩形起点的纵坐标
            // boundingRect (input_array)
            // input_array: Input gray-scale image or 2D point set(点集), stored in std::vector or Mat.
            rectangle (img,boundRect[i].tl (),boundRect[i].br (),Scalar (0,255,0),5);
            // boundRect[i].tl () returns the top-left corner
            // boundRect[i].br () returns the bottom-right corner

            int objCor = (int)conPoly[i].size ();

            if (objCor == 3) {objType = "Triangle";}
            else if (objCor == 4) 
            {
                float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
                if (aspRatio > 0.95 and aspRatio < 1.05) 
                // actually the aspRatio would not be exactly 1, and we think that is also a square
                {
                    objType = "Square";
                    cout << aspRatio << endl;
                }
                else {objType = "Rectangle";}
            }
            else if (objCor > 4) {objType = "Circle";}
            // cout << boundRect[i];
            // cout << boundRect[i].x << '\t' << boundRect[i].y << endl;
            putText (img,objType,{boundRect[i].x,boundRect[i].y - 5},FONT_HERSHEY_DUPLEX,1.5,Scalar (0,0,255),2);
            // here we can write the position in a simpler way {x,y}
            // or we can write it in a normal way Point (x,y)
            

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
    // getContours (input_img,draw_on_which_img)
    // the function will get the contours in input_img and then draw the contours in other img

    // imshow ("Image Canny",imgCanny);
    // imshow ("Image Dila",imgDila);
    imshow ("Image",img);
    waitKey(0);

    return 0;
}