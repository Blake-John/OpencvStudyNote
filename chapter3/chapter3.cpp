#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main ()
{
    string path = "/home/blake/桌面/Opencv_learn/imgs/vscode1.png";
    Mat img = imread (path);
    Mat imgResize, imgCrop;

    //todo 1.重新设置图片大小
    cout << img.size () << endl;
    resize (img,imgResize,Size (640,480));
    // Size (a,b)参数会将图片变换成所要求的长宽，这样可能会导致比例改变
    resize (img,imgResize,Size (),0.5,0.5);
    // 当传入的 Size ()为空时，往后的两个参数分别表示长宽的缩放倍数/比例

    //todo 2.裁切图片
    //* 裁切图片需要定义一个矩形数据类型
    Rect roi (100,100,150,150);
    // roi (x,y,width,height)
    // x,y 为裁切的起点，从左上开始数，weight 为从x向右的宽度，height 为从y向下的高度
    
    imgCrop = img (roi);
    // img直接传入一个矩形数据类型就会自动依据这个矩形数据类型进行裁切，类似切片处理

    imshow ("Image",img);
    imshow ("Image Resize",imgResize);
    imshow ("Image Crop",imgCrop);

    waitKey (0);

    return 0;
}