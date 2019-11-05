#include<stdio.h>
#include<opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include<windows.h>
#include<iostream>
using namespace std;
using namespace cv;
//-----------------------------------【全局变量声明部分】--------------------------------------
//		描述：全局变量声明
//-----------------------------------------------------------------------------------------------
//Shi_Tomasi相关变量
Mat Left_Corner_srcImage, Left_Corner_graysrcImage;
Mat Right_Corner_srcImage, Right_Corner_graysrcImage;
int g_maxCornerNumber = 300;
int g_maxTrackbar_CornerNumber = 500;
int minDistance = 10;
int maxTrackbar_minDistance = 20;
//-----------------------------------【全局函数声明部分】--------------------------------------
//		描述：全局函数声明
//-----------------------------------------------------------------------------------------------
static void on_FeatureDetector(int, void*);
static void ShowHelpText();
//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-----------------------------------------------------------------------------------------------
int main()
{
	//改变console字体颜色
	system("color 2F");
	//显示欢迎语
	ShowHelpText();
	//读入图片
	Mat Left_Img = imread("jihetijubu.jpg");
	Mat Right_Img = imread("jiheti.jpg");
	if (Left_Img.cols> 720|| Left_Img.rows>720)
	{
		int width, height;
		if (Left_Img.cols > Left_Img.rows)
			width = 720, height = Left_Img.rows * 720 / Left_Img.cols;
		else
			height = 720, width = Left_Img.cols * 720 / Left_Img.rows;
		resize(Left_Img, Left_Img, Size(width, height), 0, 0, INTER_LINEAR);
	}
	if (Right_Img.cols> 720 || Right_Img.rows>720)
	{
		int width, height;
		if (Right_Img.cols > Right_Img.rows)
			width = 720, height = Right_Img.rows * 720 / Right_Img.cols;
		else
			height = 720, width = Right_Img.cols * 720 / Right_Img.rows;
		resize(Right_Img, Right_Img, Size(width, height), 0, 0, INTER_LINEAR);
	}

	if (!Left_Img.data) { printf("Oh，no，读取左图像错误~！ \n"); waitKey(0); return false; }
	if (!Right_Img.data) { printf("Oh，no，读取右图像错误~！ \n"); waitKey(0); return false; }
	Left_Corner_srcImage = Left_Img.clone();
	Right_Corner_srcImage = Right_Img.clone();
	//将原图像转化为灰度图像，因为Shi-Tomasi都需要灰度图像作为输入
	cvtColor(Left_Corner_srcImage, Left_Corner_graysrcImage, COLOR_BGR2GRAY);
	cvtColor(Right_Corner_srcImage, Right_Corner_graysrcImage, COLOR_BGR2GRAY);

	namedWindow("【角点检测结果左】", CV_WINDOW_AUTOSIZE);
	namedWindow("【匹配结果】", CV_WINDOW_AUTOSIZE);
	//选择Shi_Tomasi角点检测算法参数的滑动条
	createTrackbar("最大角点数", "【角点检测结果左】", &g_maxCornerNumber, g_maxTrackbar_CornerNumber, on_FeatureDetector);
	createTrackbar("最小角点间距", "【角点检测结果左】", &minDistance, maxTrackbar_minDistance, on_FeatureDetector);
	on_FeatureDetector(0, 0);
	waitKey(0);
	return 0;
}

//-----------------------------------【ShowHelpText( )函数】----------------------------------
//          描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n");
	printf("\n\n\t\t\t此为本书OpenCV3版的第87个配套示例程序\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
	//输出一些帮助信息
	printf("\n\n\n\t欢迎来到【Shi-Tomasi角点检测】示例程序\n");
	printf("\n\t请调整滑动条观察图像效果\n\n");
}

//-----------------------------------【on_FeatureDetector( )函数】--------------------------------
//		描述：特征提取的回调函数
//-----------------------------------------------------------------------------------------------
void on_FeatureDetector(int, void*)
{
	//-------------Shi-Tomasi角点检测部分---------------------------------------------------------------
	//【1】对变量小于等于1时的处理
	if (g_maxCornerNumber <= 1) { g_maxCornerNumber = 1; }

	//【2】Shi-Tomasi算法（goodFeaturesToTrack函数）的参数准备
	vector<Point2f> Left_Corners, Right_Corners;
	double qualityLevel = 0.01;//角点检测可接受的最小特征值
	int blockSize = 3;//计算导数自相关矩阵时指定的邻域范围
	double k = 0.04;//权重系数
	Mat Left_Copy = Left_Corner_srcImage.clone();	//复制源图像到一个临时变量中，作为感兴趣区域
	Mat Right_Copy = Right_Corner_srcImage.clone();

	//【3】进行Shi-Tomasi角点检测
	goodFeaturesToTrack(Left_Corner_graysrcImage,//输入图像
		Left_Corners,//检测到的角点的输出向量
		g_maxCornerNumber,//角点的最大数量
		qualityLevel,//角点检测可接受的最小特征值
		minDistance,//角点之间的最小距离
		Mat(),//感兴趣区域
		blockSize,//计算导数自相关矩阵时指定的邻域范围
		false,//不使用Harris角点检测
		k);//权重系数

	goodFeaturesToTrack(Right_Corner_graysrcImage,//输入图像
		Right_Corners,//检测到的角点的输出向量
		g_maxCornerNumber,//角点的最大数量
		qualityLevel,//角点检测可接受的最小特征值
		minDistance,//角点之间的最小距离
		Mat(),//感兴趣区域
		blockSize,//计算导数自相关矩阵时指定的邻域范围
		false,//不使用Harris角点检测
		k);//权重系数


	//【4】输出文字信息
	cout << "\t>此次检测到的左图角点数量为：" << Left_Corners.size() << endl;
	cout << "\t>此次检测到的右图角点数量为：" << Right_Corners.size() << endl;
	//【5】绘制检测到的角点
	int r = 4;
	for (int i = 0; i < Left_Corners.size(); i++)
	{
		//绘制出角点
		circle(Left_Copy, Left_Corners[i], r, Scalar(0, 255, 0), -1, 8, 0);
	}
	for (int i = 0; i < Right_Corners.size(); i++)
	{
		//绘制出角点
		circle(Right_Copy, Right_Corners[i], r, Scalar(0, 255, 0), -1, 8, 0);
	}
	//【6】显示（更新）窗口
	imshow("【角点检测结果左】", Left_Copy);
	imshow("【角点检测结果右】", Right_Copy);
	vector<KeyPoint> Left_Keypoints, Right_Keypoints;
	//将Point2f转化为KeyPoint类型
	KeyPoint::convert(Left_Corners, Left_Keypoints, 1, 1, 0, -1);
	KeyPoint::convert(Right_Corners, Right_Keypoints, 1, 1, 0, -1);
	//生产特征向量描述子
	Ptr<cv::xfeatures2d::SurfDescriptorExtractor> extractor = cv::xfeatures2d::SurfDescriptorExtractor::create();
	Mat Left_Descriptors, Right_Descriptors;
	extractor->compute(Left_Corner_srcImage, Left_Keypoints, Left_Descriptors);
	extractor->compute(Right_Corner_srcImage, Right_Keypoints, Right_Descriptors);
	//用FLANN匹配器匹配描述向量
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(Left_Descriptors, Right_Descriptors, matches);

	double max_dist = 0; double min_dist = 100;
	for (int i = 0; i < Left_Descriptors.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > min_dist) max_dist = dist;
	}
	cout << "Max distance:" << max_dist << endl;
	cout << "Min distance:" << min_dist << endl;
	vector<DMatch> good_matches;
	//仅绘制好的匹配
	for (int i = 0; i < Left_Descriptors.rows; i++)
	{
			if (matches[i].distance < 2.5* min_dist)
			{
				good_matches.push_back(matches[i]);
			}
		
	}
	Mat img_matches;
	cv::drawMatches(Left_Corner_srcImage, Left_Keypoints,
		Right_Corner_srcImage, Right_Keypoints, good_matches,
		img_matches, Scalar::all(-1));
	imshow("【匹配结果】", img_matches);
	waitKey(0);
}






