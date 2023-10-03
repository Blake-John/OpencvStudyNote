#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main ()
{
    //! 在c++中，每次输出所用到的变量都需要在前面声明

    string path = "/home/blake/桌面/Opencv_learn/imgs/vscode1.png";
    Mat img = imread (path);
    Mat imgGray, imgBlur, imgCanny, imgDila, imgEro; // 声明图像类型


    //todo 1.转换图像色彩
    cvtColor (img,imgGray,COLOR_BGR2GRAY);
    // COLOR_BGR2GRAY 指将图片从bgr转化成gray，from bgr to gray
    //! opencv 中特别约定为bgr而非rgb
    // cvtColor (input,output,convertion_type)
    // 将 img 按 convertion_type 的规则进行转化，并输出为 imgGray

    //todo 2.高斯模糊
    GaussianBlur (img,imgBlur,Size (7,7),5,5);
    // Size (7,7) 规定了内核的大小，即通过高斯模糊后输出的图像内核为7*7
    //todo 在配置完环境后更改最后的参数并查看其不同的效果，写出其作用

    //todo 3.边缘检测
    //* 常用的边缘检测器是坎尼边缘检测器
    //* 在使用 Canny Edge Detector 之前常常将图像进行一定的模糊处理
    Canny (imgBlur,imgCanny,50,150);
    // 对于 Canny ，后面的两个参数为两个阈值，可以以此来说明检测的强度和范围
    // 可以反向类比于 Ps 中边缘羽化的阈值

    //todo 4.扩大和侵蚀
    //* 在边缘检测时，通常会因为边缘太粗或太细而效果不好
    //* 这时就可以用 dilate 来增加边缘的粗细，用 erode 来侵蚀边缘，减小其粗细

    Mat kernel = getStructuringElement (MORPH_RECT,Size (5,5));
    // 通过 Mat 定义一个内核kernel变量，利用getStructuringElement方法来生成一个以 Size 为大小的结构元素
    // 其中 morph 是 变体 的意思，意味着可以改变其形状 
    // MORPH_RECT 定义了该结构元素为矩形rectangular
    // 该结构元素可以作为 dilate 的 内核，Size 越大，膨胀得越多，Size越小，膨胀得越少
    //! 应该注意，Size中的大小应该只用奇数
    dilate (imgCanny,imgDila,kernel);
    // 第三个参数kernel规定了扩大的强度，应该用一个矩形结构元素作为其内核参数
    erode (imgDila,imgEro,kernel);
    // 第三个参数kernel规定了其侵蚀的强度，这里本同样应该定义一个矩形结构元素作为其内核参数


    imshow ("Image",img);
    imshow ("Image Gray",imgGray);
    imshow ("Image Blur",imgBlur);
    imshow ("Image Canny",imgCanny);
    imshow ("Image Dilation",imgDila);
    imshow ("Image Erode",imgEro);
    waitKey (0);

    return 0;
}