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
int minDistance = 10;
int maxTrackbar_minDistance = 20;
//-----------------------------------��ȫ�ֺ����������֡�--------------------------------------
//		������ȫ�ֺ�������
//-----------------------------------------------------------------------------------------------
static void on_FeatureDetector(int, void*);
static void ShowHelpText();
//-----------------------------------��main( )������--------------------------------------------
//		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
//-----------------------------------------------------------------------------------------------
int main()
{
	//�ı�console������ɫ
	system("color 2F");
	//��ʾ��ӭ��
	ShowHelpText();
	//����ͼƬ
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

	if (!Left_Img.data) { printf("Oh��no����ȡ��ͼ�����~�� \n"); waitKey(0); return false; }
	if (!Right_Img.data) { printf("Oh��no����ȡ��ͼ�����~�� \n"); waitKey(0); return false; }
	Left_Corner_srcImage = Left_Img.clone();
	Right_Corner_srcImage = Right_Img.clone();
	//��ԭͼ��ת��Ϊ�Ҷ�ͼ����ΪShi-Tomasi����Ҫ�Ҷ�ͼ����Ϊ����
	cvtColor(Left_Corner_srcImage, Left_Corner_graysrcImage, COLOR_BGR2GRAY);
	cvtColor(Right_Corner_srcImage, Right_Corner_graysrcImage, COLOR_BGR2GRAY);

	namedWindow("���ǵ�������", CV_WINDOW_AUTOSIZE);
	namedWindow("��ƥ������", CV_WINDOW_AUTOSIZE);
	//ѡ��Shi_Tomasi�ǵ����㷨�����Ļ�����
	createTrackbar("���ǵ���", "���ǵ�������", &g_maxCornerNumber, g_maxTrackbar_CornerNumber, on_FeatureDetector);
	createTrackbar("��С�ǵ���", "���ǵ�������", &minDistance, maxTrackbar_minDistance, on_FeatureDetector);
	on_FeatureDetector(0, 0);
	waitKey(0);
	return 0;
}

//-----------------------------------��ShowHelpText( )������----------------------------------
//          ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t�ǳ���л����OpenCV3������š�һ�飡\n");
	printf("\n\n\t\t\t��Ϊ����OpenCV3��ĵ�87������ʾ������\n");
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
	vector<KeyPoint> Left_Keypoints, Right_Keypoints;
	//��Point2fת��ΪKeyPoint����
	KeyPoint::convert(Left_Corners, Left_Keypoints, 1, 1, 0, -1);
	KeyPoint::convert(Right_Corners, Right_Keypoints, 1, 1, 0, -1);
	//������������������
	Ptr<cv::xfeatures2d::SurfDescriptorExtractor> extractor = cv::xfeatures2d::SurfDescriptorExtractor::create();
	Mat Left_Descriptors, Right_Descriptors;
	extractor->compute(Left_Corner_srcImage, Left_Keypoints, Left_Descriptors);
	extractor->compute(Right_Corner_srcImage, Right_Keypoints, Right_Descriptors);
	//��FLANNƥ����ƥ����������
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
	//�����ƺõ�ƥ��
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
	imshow("��ƥ������", img_matches);
	waitKey(0);
}






