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
////-----------------------------------��main( )������--------------------------------------------
////		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
////-----------------------------------------------------------------------------------------------
//int main()
//{
//	//�ı�console������ɫ
//	system("color 2F");
//	//����ͼƬ
//	Mat Left_Img = imread("left.jpg");
//	Mat Right_Img = imread("right.jpg");
//	if (!Left_Img.data) { printf("Oh��no����ȡ��ͼ�����~�� \n"); waitKey(0); return false; }
//	if (!Right_Img.data) { printf("Oh��no����ȡ��ͼ�����~�� \n"); waitKey(0); return false; }
//	//��ͼ��ȱ�����С�����С��720
//	Resize_Input_Img(Left_Img, 720);
//	Resize_Input_Img(Right_Img, 720);
//	namedWindow("���ǵ�������", CV_WINDOW_NORMAL);
//	namedWindow("��ƥ������", CV_WINDOW_NORMAL);
//	MySV callback = MySV(Left_Img, Right_Img);
//	//��ʾ��ӭ��
//	callback.ShowHelpText();
//	//ѡ��Shi_Tomasi�ǵ����㷨�����Ļ�����
//	createTrackbar("���ǵ���", "���ǵ�������", &callback.g_maxCornerNumber, callback.g_maxTrackbar_CornerNumber, callback.on_FeatureDetector);
//	createTrackbar("��С�ǵ���", "���ǵ�������", &callback.minDistance, callback.maxTrackbar_minDistance, callback.on_FeatureDetector);
//	callback.on_FeatureDetector(0, 0);
//	waitKey(0);
//	return 0;
//}