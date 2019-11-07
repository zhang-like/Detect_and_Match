#include<stdio.h>
#include<opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include<windows.h>
#include<iostream>
#include"detect_and_match.h"
using namespace std;
using namespace cv;
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
	if (!Left_Img.data) { printf("Oh��no����ȡ��ͼ�����~�� \n"); waitKey(0); return false; }
	if (!Right_Img.data) { printf("Oh��no����ȡ��ͼ�����~�� \n"); waitKey(0); return false; }
	//��ͼ��ȱ�����С�����С��720
	Resize_Input_Img(Left_Img, 720);
	Resize_Input_Img(Right_Img, 720);
	Left_Corner_srcImage = Left_Img.clone();
	Right_Corner_srcImage = Right_Img.clone();
	//��ԭͼ��ת��Ϊ�Ҷ�ͼ����ΪShi-Tomasi����Ҫ�Ҷ�ͼ����Ϊ����
	cvtColor(Left_Corner_srcImage, Left_Corner_graysrcImage, COLOR_BGR2GRAY);
	cvtColor(Right_Corner_srcImage, Right_Corner_graysrcImage, COLOR_BGR2GRAY);
	namedWindow("���ǵ�������", CV_WINDOW_NORMAL);
	namedWindow("��ƥ������", CV_WINDOW_NORMAL);
	//ѡ��Shi_Tomasi�ǵ����㷨�����Ļ�����
	createTrackbar("���ǵ���", "���ǵ�������", &g_maxCornerNumber, g_maxTrackbar_CornerNumber, on_FeatureDetector);
	createTrackbar("��С�ǵ���", "���ǵ�������", &minDistance, maxTrackbar_minDistance, on_FeatureDetector);
	on_FeatureDetector(0, 0);
	waitKey(0);
	return 0;
}
