#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main ()
{
    //todo define an image
    Mat img (512,512,CV_8UC3,Scalar (255,255,255));
    // img (x,y,位深和层深(通道数)，颜色)
    // CV_8UC3 指的是 由cv产生的，位深为8位Unsigned，channel数量(层深/颜色通道)为3的图片
    // Scalar (a,b,b) 规定了各个色层(blue,green,red)的饱和度，在定义颜色时，我们需要定义一个Scalar来规定颜色
    
    //todo draw a round
    circle (img,Point (256,256),155,Scalar (0,69,255),FILLED);
    // 以 img (256,256) 为圆心，155为半径，Scalar(0,69,255)为颜色,圆周的粗细为10
    // circle (img,center,radius,color,thickness/is_filled)

    //todo draw a rectangle
    //* we can define a rectangle by an roi 
    // Rect roi (130,226,382-130,286-226);
    // rectangle (img,roi,Scalar (255,255,255),FILLED);
    
    //* or we can use two point to define it,
    //* the first point and the second point oppsite to the first one
    rectangle (img,Point (130,226),Point (382,286),Scalar (255,255,255),FILLED);
    // rectangle (img,point1,point2,color,thickness/is_filled)

    //todo draw a line
    //* a line is defined be two point
    line (img,Point (130,296),Point (382,296),Scalar (255,255,255),2);
    // line (img,start_point,end_point,color,thickness)

    //todo put some text in the img
    putText (img,"Hello,world!",Point (160,270),FONT_HERSHEY_DUPLEX,1,Scalar (0,69,255),3);
    // putText (img,text,displacement,font,font_scalar,color,thickness)
    // the displacement is the Bottom-left corner of the text string in the image

    imshow ("Image",img);

    waitKey (0);

    return 0;
}