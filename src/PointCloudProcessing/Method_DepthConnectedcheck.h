/******************************************************************************************************
������ʵ����������ά��ȥ���㷨��һ���ǻ�����ά������ȥ���㷨����һ���ǻ����Ӳ�ͼ����ͨ�������ȥ���㷨��
���ߣ��챣��
ʱ�䣺2017/4/13

������ DepthConnectedcheckʵ�ֵ��ǻ����Ӳ�ͼ����ͨ�������ȥ���㷨
*******************************************************************************************************/
#pragma once
#include<vector>
#include<deque>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<highgui.h>
#include<math.h>
#include"Denoise3DPoint.h"
using namespace std;
namespace Point3D_Processing
{
	class DepthConnectedcheck:public Denoise3DPoint
	{
	public:
        DepthConnectedcheck(cam cam_,int minsize,int maxgap);
		template <typename T>
        void filterSpecklesImpl(cv::Mat& img, int newVal, int maxSpeckleSize, int maxDiff, cv::Mat& _buf);
        void Denose3DPoint(const cv::Mat& left,const cv::Mat&disp);
		void GetNoise3DPoint();//��ȡ������ͼ
		int minsize;//��ͨ�����ٰ������صĸ���
		int maxgap;//
		cv::Mat buf_;
	};

}
