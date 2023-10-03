#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


// // 导入图片
// int main ()
// {
//     string path = "/home/blake/桌面/Opencv_learn/imgs/vscode1.png" ; // 定义一个 string 变量用来存储图片的路径
//     Mat img = imread (path); // Mat 是由opencv引入的矩阵数据类型，用于处理所有的图像
//     imshow ("Image",img); // 用于展示图像
//     waitKey (0); //imshow能展示图像,但是会立刻关闭，需要给他一个延迟，这里写 0 意味着 无穷

//     return 0;
// }


// 导入视频
// 与导入图片有一点不同，因为视频是有许多图片组成的
// 我们需要遍历所有的图像或所有帧，并一一显示
// int main ()
// {
//     string path = "/home/blake/桌面/Opencv_learn/videos/ssh_introduction.mp4" ;
//     VideoCapture cap (path); // 说明使用视频模块
//     Mat img;

//     while (true)
//     {
//         cap.read (img); // 利用将cap中存储的视频读取，并存储在img中

//         imshow ("Image",img);
//         waitKey (1); // 该处的单位为ms
//     }
//     return 0;

// }

// 网络摄像头
int main ()
{
    VideoCapture cap (0); // 将视频替换为摄像头的id，电脑默认摄像头为0
    Mat img;

    while (true)
    {
        cap.read (img); // 利用将cap中获取的的视频读取，并存储在img中

        imshow ("Image",img);
        waitKey (20); // 该处的单位为ms
    

    }
    return 0;

}