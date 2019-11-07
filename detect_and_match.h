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
int minDistance = 5;
int maxTrackbar_minDistance = 20;
//-----------------------------------【全局函数声明部分】--------------------------------------
//		描述：全局函数声明
//-----------------------------------------------------------------------------------------------
void on_FeatureDetector(int, void*);
void ShowHelpText();
void Resize_Input_Img(Mat& Img);
//-----------------------------------【ShowHelpText( )函数】----------------------------------
//          描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
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

	//【7】进行亚像素角点检测
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 40, 0.001);
	cornerSubPix(Left_Corner_graysrcImage, Left_Corners, Size(5, 5), Size(-1, -1), criteria);
	cornerSubPix(Right_Corner_graysrcImage, Right_Corners, Size(5, 5), Size(-1, -1), criteria);
	vector<KeyPoint> Left_Keypoints, Right_Keypoints;

	//【8】将Point2f转化为KeyPoint类型
	KeyPoint::convert(Left_Corners, Left_Keypoints, 1, 1, 0, -1);
	KeyPoint::convert(Right_Corners, Right_Keypoints, 1, 1, 0, -1);

	//【9】用Sift算法进行特征向量生成，并用FLANN匹配器进行描述向量匹配
	Ptr<cv::xfeatures2d::SiftFeatureDetector> detector = cv::xfeatures2d::SiftFeatureDetector::create();
	Mat Left_Descriptors, Right_Descriptors;
	detector->detectAndCompute(Left_Corner_srcImage, Mat(), Left_Keypoints, Left_Descriptors, 1);
	detector->detectAndCompute(Right_Corner_srcImage, Mat(), Right_Keypoints, Right_Descriptors, 1);

	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(Left_Descriptors, Right_Descriptors, matches);

	//【10】计算单应性矩阵H，用RANSCA剔除误匹配点
	vector<Point2f> Left_Matches, Right_Matches;
	int Match_Points_Num = matches.size();
	for (int i = 0; i < Match_Points_Num; i++)//将得到的左右匹配结果点分别存入动态数组
	{
		Left_Matches.push_back(Left_Keypoints[matches[i].queryIdx].pt);
		Right_Matches.push_back(Right_Keypoints[matches[i].trainIdx].pt);
	}
	Mat H = findHomography(Left_Matches, Right_Matches, CV_RANSAC);
	vector<char> mask(Left_Matches.size());
	vector<Point2f> Left_Matches_Aftertrans;
	perspectiveTransform(Left_Matches, Left_Matches_Aftertrans, H);
	
	for (int i = 0; i < Right_Matches.size(); i++)//剔除误差范数大于5的点
	{
		if (norm(Right_Matches[i] - Left_Matches_Aftertrans[i]) <= 5)
		{
			mask[i] = 1;
		}
	}
	Mat img_matches;
	drawMatches(Left_Corner_srcImage, Left_Keypoints, Right_Corner_srcImage,Right_Keypoints, 
		matches, img_matches, Scalar::all(-1), Scalar::all(-1), mask, DrawMatchesFlags::DEFAULT);



	//Mat img_matches;
	//cv::drawMatches(Left_Corner_srcImage, Left_Keypoints,
	//	Right_Corner_srcImage, Right_Keypoints, good_matches,
	//	img_matches, Scalar::all(-1));
	imshow("【匹配结果】", img_matches);
	waitKey(0);
}

//-----------------------------------【Resize_Input_Img】--------------------------------
//		描述：将图像等比例缩小到最长边小于一定值
//-----------------------------------------------------------------------------------------------
void Resize_Input_Img(Mat& Img,int length)
{
	if (Img.cols> length || Img.rows>length)
	{
		int width, height;
		if (Img.cols > Img.rows)
			width = length, height = Img.rows * length / Img.cols;
		else
			height = length, width = Img.cols * length /Img.rows;
		resize(Img, Img, Size(width, height), 0, 0, INTER_LINEAR);
	}
}
