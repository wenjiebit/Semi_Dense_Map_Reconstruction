/******************************************************************************************************
������ʵ����������ά��ȥ���㷨��һ���ǻ�����ά������ȥ���㷨����һ���ǻ����Ӳ�ͼ����ͨ�������ȥ���㷨��
���ߣ��챣��
ʱ�䣺2017/4/13

������ Point3D_Clusterʵ�ֵ��ǻ�����ά������ȥ���㷨
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

//��ά���ܶȾ���ȥ��
class Point3D_Cluster:public Denoise3DPoint
{
	
public:
    Point3D_Cluster(cam cam_, float Radiusratio_, double MinPtsInWindow_);

	~Point3D_Cluster(void);
	//�Ƿ��Ǻ��ĵ㣨���ܶȴ󣩣�
	bool IsCorePoint(Point_3D &P, vector<vector<Point_3D>>& All3DPoint,deque<Point_3D*>&NeighborPointdeque);

	inline float CaculatePointDistance(const Point_3D& P1,const Point_3D& P2)
	{
		return sqrt(    (P1.Point3D.val[0]-P2.Point3D.val[0])*(P1.Point3D.val[0]-P2.Point3D.val[0])+
						(P1.Point3D.val[1]-P2.Point3D.val[1])*(P1.Point3D.val[1]-P2.Point3D.val[1])+
						(P1.Point3D.val[2]-P2.Point3D.val[2])*(P1.Point3D.val[2]-P2.Point3D.val[2]) );
	}
	inline float CaculateDepthResolution(int d)
	{
		float bf = Cam_->bf;
		return (bf / d - bf / (d + 1));
	}
    void Get3dPoint(vector< vector<Point_3D> >&All3DPoint);//��ȡ��ά��
    void runcluster(vector<vector<Point_3D>>&All3DPoint);
    void Denose3DPoint(  const cv::Mat&left,const cv::Mat& disp);
    cv::Mat showEveryCluster3DPiont(const vector<vector<Point_3D>>&All3DPoint);
    int classnum;
	bool showAllcluster;
    cv::Mat allcluster;

private:
	float  Radiusratio;//�뾶����
	double MinPtsInWindow;//��������
};
//ȥ��



}
