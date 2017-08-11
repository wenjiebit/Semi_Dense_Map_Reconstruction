/******************************************************************************************************
������ʵ����������ά��ȥ���㷨��һ���ǻ�����ά������ȥ���㷨����һ���ǻ����Ӳ�ͼ����ͨ�������ȥ���㷨��
���ߣ��챣��
ʱ�䣺2017/4/13
Denoise3DPoint ����
Point3D_Cluster��DepthConnectedcheck�������������࣬�ֱ�ʵ�ֲ�ͬ�㷨��
�����������ͼ �Ӳ�ͼ ������� �������
�����ԭ������Ӳ�ͼ  ȥ�����Ӳ�ͼ  ����ͼ
*******************************************************************************************************/
#pragma once
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<highgui.h>
#include<math.h>
using namespace std;
namespace Point3D_Processing
{
	enum _Pointproperty { havenotprocess = 0, isborderpoint = 1, iscorepoint = 2 };
	class Point_3D
	{

	public:
		Point_3D(const cv::Point& Point2D_, const cv::Scalar& Point3D_);
		cv::Point Point2D;
		cv::Scalar Point3D;
		_Pointproperty Pointproperty;
		int classID;
	};
	class cam
	{
	public:
		cam(float bf_, float f_, float cx_, float cy_, int width_, int height_) :bf(bf_), f(f_), cx(cx_), cy(cy_), width(width_), height(height_) {}
		const float bf;
		const float f;
		const float cx;
		const float cy;
		const int width;
		const int height;
	};
	class Denoise3DPoint
	{
	public:
        Denoise3DPoint(cam cam_);
		~Denoise3DPoint();
		cam *Cam_;
		void disptodepth(const cv::Mat&disp, float bf, cv::Mat& depth);
        void getstrongtexture(const cv::Mat&left, const cv::Mat&disp, cv::Mat&text);//��ȡǿ��������
		cv::Mat GetFalseColorDenoisedisp();//��ȡȥ��ͼ
		cv::Mat GetFalseColorNoisedisp();//��ȡ������ͼ
		cv::Mat GetFalseColorOriginaldisp();//��ȡԭ����������ͼ

        virtual void Denose3DPoint(const cv::Mat&left,const cv::Mat &disp_) = 0;//ȥ�봦��  �����ӿ�
		cv::Mat left;
		cv::Mat disp;
		cv::Mat SemiDenseDisp;//ǿ�������ͼ
		cv::Mat NoiseDisp;//������ͼ
		cv::Mat DeNoiseDisp;//ȥ����ͼ
	};
	void GenerateFalseMap(cv::Mat &src, cv::Mat &disp);
    void dispTodepthShow(const cv::Mat&Depthfloat,float bf,cv::Mat &Depthuchar);
	
}
