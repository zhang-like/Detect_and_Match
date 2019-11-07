//#include<stdio.h>
//#include<opencv2/opencv.hpp>
//#include <opencv2/xfeatures2d/nonfree.hpp>
//#include<windows.h>
//#include<iostream>
//#include"oop.h"
//using namespace std;
//using namespace cv;
//using namespace StereoVision;
//
//
////-----------------------------------【main( )函数】--------------------------------------------
////		描述：控制台应用程序的入口函数，我们的程序从这里开始
////-----------------------------------------------------------------------------------------------
//int main()
//{
//	//改变console字体颜色
//	system("color 2F");
//	//读入图片
//	Mat Left_Img = imread("left.jpg");
//	Mat Right_Img = imread("right.jpg");
//	if (!Left_Img.data) { printf("Oh，no，读取左图像错误~！ \n"); waitKey(0); return false; }
//	if (!Right_Img.data) { printf("Oh，no，读取右图像错误~！ \n"); waitKey(0); return false; }
//	//将图像等比例缩小到最长边小于720
//	Resize_Input_Img(Left_Img, 720);
//	Resize_Input_Img(Right_Img, 720);
//	namedWindow("【角点检测结果左】", CV_WINDOW_NORMAL);
//	namedWindow("【匹配结果】", CV_WINDOW_NORMAL);
//	MySV callback = MySV(Left_Img, Right_Img);
//	//显示欢迎语
//	callback.ShowHelpText();
//	//选择Shi_Tomasi角点检测算法参数的滑动条
//	createTrackbar("最大角点数", "【角点检测结果左】", &callback.g_maxCornerNumber, callback.g_maxTrackbar_CornerNumber, callback.on_FeatureDetector);
//	createTrackbar("最小角点间距", "【角点检测结果左】", &callback.minDistance, callback.maxTrackbar_minDistance, callback.on_FeatureDetector);
//	callback.on_FeatureDetector(0, 0);
//	waitKey(0);
//	return 0;
//}