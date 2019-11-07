#include<stdio.h>
#include<opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include<windows.h>
#include<iostream>
using namespace std;
using namespace cv;

//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------
//		������ȫ�ֱ�������
//-----------------------------------------------------------------------------------------------
//Shi_Tomasi��ر���
Mat Left_Corner_srcImage, Left_Corner_graysrcImage;
Mat Right_Corner_srcImage, Right_Corner_graysrcImage;
int g_maxCornerNumber = 300;
int g_maxTrackbar_CornerNumber = 500;
int minDistance = 5;
int maxTrackbar_minDistance = 20;
//-----------------------------------��ȫ�ֺ����������֡�--------------------------------------
//		������ȫ�ֺ�������
//-----------------------------------------------------------------------------------------------
void on_FeatureDetector(int, void*);
void ShowHelpText();
void Resize_Input_Img(Mat& Img);
//-----------------------------------��ShowHelpText( )������----------------------------------
//          ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
	//���һЩ������Ϣ
	printf("\n\n\n\t��ӭ������Shi-Tomasi�ǵ��⡿ʾ������\n");
	printf("\n\t������������۲�ͼ��Ч��\n\n");
}

//-----------------------------------��on_FeatureDetector( )������--------------------------------
//		������������ȡ�Ļص�����
//-----------------------------------------------------------------------------------------------
void on_FeatureDetector(int, void*)
{
	//-------------Shi-Tomasi�ǵ��ⲿ��---------------------------------------------------------------
	//��1���Ա���С�ڵ���1ʱ�Ĵ���
	if (g_maxCornerNumber <= 1) { g_maxCornerNumber = 1; }

	//��2��Shi-Tomasi�㷨��goodFeaturesToTrack�������Ĳ���׼��
	vector<Point2f> Left_Corners, Right_Corners;
	double qualityLevel = 0.01;//�ǵ���ɽ��ܵ���С����ֵ
	int blockSize = 3;//���㵼������ؾ���ʱָ��������Χ
	double k = 0.04;//Ȩ��ϵ��
	Mat Left_Copy = Left_Corner_srcImage.clone();	//����Դͼ��һ����ʱ�����У���Ϊ����Ȥ����
	Mat Right_Copy = Right_Corner_srcImage.clone();

	//��3������Shi-Tomasi�ǵ���
	goodFeaturesToTrack(Left_Corner_graysrcImage,//����ͼ��
		Left_Corners,//��⵽�Ľǵ���������
		g_maxCornerNumber,//�ǵ���������
		qualityLevel,//�ǵ���ɽ��ܵ���С����ֵ
		minDistance,//�ǵ�֮�����С����
		Mat(),//����Ȥ����
		blockSize,//���㵼������ؾ���ʱָ��������Χ
		false,//��ʹ��Harris�ǵ���
		k);//Ȩ��ϵ��

	goodFeaturesToTrack(Right_Corner_graysrcImage,//����ͼ��
		Right_Corners,//��⵽�Ľǵ���������
		g_maxCornerNumber,//�ǵ���������
		qualityLevel,//�ǵ���ɽ��ܵ���С����ֵ
		minDistance,//�ǵ�֮�����С����
		Mat(),//����Ȥ����
		blockSize,//���㵼������ؾ���ʱָ��������Χ
		false,//��ʹ��Harris�ǵ���
		k);//Ȩ��ϵ��
    //��4�����������Ϣ
	cout << "\t>�˴μ�⵽����ͼ�ǵ�����Ϊ��" << Left_Corners.size() << endl;
	cout << "\t>�˴μ�⵽����ͼ�ǵ�����Ϊ��" << Right_Corners.size() << endl;

	//��5�����Ƽ�⵽�Ľǵ�
	int r = 4;
	for (int i = 0; i < Left_Corners.size(); i++)
	{
		//���Ƴ��ǵ�
		circle(Left_Copy, Left_Corners[i], r, Scalar(0, 255, 0), -1, 8, 0);
	}
	for (int i = 0; i < Right_Corners.size(); i++)
	{
		//���Ƴ��ǵ�
		circle(Right_Copy, Right_Corners[i], r, Scalar(0, 255, 0), -1, 8, 0);
	}
	//��6����ʾ�����£�����
	imshow("���ǵ�������", Left_Copy);
	imshow("���ǵ������ҡ�", Right_Copy);

	//��7�����������ؽǵ���
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 40, 0.001);
	cornerSubPix(Left_Corner_graysrcImage, Left_Corners, Size(5, 5), Size(-1, -1), criteria);
	cornerSubPix(Right_Corner_graysrcImage, Right_Corners, Size(5, 5), Size(-1, -1), criteria);
	vector<KeyPoint> Left_Keypoints, Right_Keypoints;

	//��8����Point2fת��ΪKeyPoint����
	KeyPoint::convert(Left_Corners, Left_Keypoints, 1, 1, 0, -1);
	KeyPoint::convert(Right_Corners, Right_Keypoints, 1, 1, 0, -1);

	//��9����Sift�㷨���������������ɣ�����FLANNƥ����������������ƥ��
	Ptr<cv::xfeatures2d::SiftFeatureDetector> detector = cv::xfeatures2d::SiftFeatureDetector::create();
	Mat Left_Descriptors, Right_Descriptors;
	detector->detectAndCompute(Left_Corner_srcImage, Mat(), Left_Keypoints, Left_Descriptors, 1);
	detector->detectAndCompute(Right_Corner_srcImage, Mat(), Right_Keypoints, Right_Descriptors, 1);

	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(Left_Descriptors, Right_Descriptors, matches);

	//��10�����㵥Ӧ�Ծ���H����RANSCA�޳���ƥ���
	vector<Point2f> Left_Matches, Right_Matches;
	int Match_Points_Num = matches.size();
	for (int i = 0; i < Match_Points_Num; i++)//���õ�������ƥ������ֱ���붯̬����
	{
		Left_Matches.push_back(Left_Keypoints[matches[i].queryIdx].pt);
		Right_Matches.push_back(Right_Keypoints[matches[i].trainIdx].pt);
	}
	Mat H = findHomography(Left_Matches, Right_Matches, CV_RANSAC);
	vector<char> mask(Left_Matches.size());
	vector<Point2f> Left_Matches_Aftertrans;
	perspectiveTransform(Left_Matches, Left_Matches_Aftertrans, H);
	
	for (int i = 0; i < Right_Matches.size(); i++)//�޳���������5�ĵ�
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
	imshow("��ƥ������", img_matches);
	waitKey(0);
}

//-----------------------------------��Resize_Input_Img��--------------------------------
//		��������ͼ��ȱ�����С�����С��һ��ֵ
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
