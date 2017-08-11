#ifndef ANTENNADETECT_
#define ANTENNADETECT_
/*This file is part of AntennaDetect.
 *
 *  Created on: 2016��10��22��
 *      Author: Zuber
 */
#include<iostream>
#include<opencv2/opencv.hpp>
#include <algorithm>
#include<cmath>
#include<vector>
using namespace std;
using namespace cv;

	struct region
	{
	 vector<Point> Pionts;
	 float neighbor_region_value_avg;
	 float neighbor_region_value_var;
	};
    class line_
    {
    public:
        vector<Point> PiontsofLine;
        float k;
        float b;
    };
	class Detect_Wireline
	{
	public:
		Detect_Wireline();

		 //�������ܣ�������Ҷ�ͼ��ÿһ�����ص���sobel���Ӽ���ݶȣ��������ص���ݶȴ�����ֵthredʱ����ɫ��ʾ����һͨ��Ϊ255��
		 void detect_grad(const Mat&grayinput,Mat&grad,float thred);
		
         //�������ܣ� �����߶μ��
         //���룺�����ͼ��
         //���������߶���ʾͼ���߶��ϵ����е�
         void houghcheck(const Mat&input,Mat&output,vector<line_>& pointsofLines_k);

         //�������ܣ�������ʼ�����꣬����ֱ�ߡ�����ֱ�������е���������
         //���룺��Ҫ�����Ƶ�ͼ��ֱ�ߵ���ʼ������
         //�����������ϵ�ͼ��ֱ�������е�����ꡣ
         void putline_into_pic( Mat& pic,vector<Vec4i>& lines,vector<line_>& pointsofLines_k);


		//�������ܣ����ݶ�ͼ����ȥ�������ݶ�����
		//����:�ݶ�ͼ�������Ҷ�ͼ����ⴰ�ڴ�С��
		//�����ȥ�����ͼ��
        void detectsmallobject(const Mat&gradinput,Mat&outputwithsamllobject,int windowsize);

        //�������ܣ������ѡ���ߵ�б��
        float CalLineSlope(const region&);
        //�������ܣ�б�ʼ��,�ų�б�ʲ�����Ҫ�����
        void  SlopeCheck(const 	vector<region >& regions,vector<region >&AfterSlopeCheck);

		//�������ܣ����������Ϣ���ֱ�ߣ���һЩ��Χ��ȷ���Ƚ�С������Ⱦ�ֵ�Ƚ�С��ֱ��ȥ������Ϊ�������ߺܿ���������Ӱ��
		//���룺������ֱ�ߡ���ⴰ�ڴ�С���Ӳ��������ֵ�����ͼ
		//���: ����ͼ��
        void  dispcheck( const vector<region> regions,const Mat&disp,Mat &out,vector<region>& regions_afterdispcheck,int windoesize=15,int Dmax=100);
        void  dispcheck( const vector<region> regions,const Mat&disp,vector<cv::Point>& Points,vector<region>& regions_afterdispcheck,int windowsize=15,int Dmax=100);
       void  depthcheck( const vector<region> regions,const Mat&depth,vector<vector<cv::Point>>& Points,vector<region>& regions_afterdispcheck,int windowsize=15,int Dmax=100);
		
		void  detect(const cv::Mat &inputsrc,const cv::Mat &disp,vector<cv::Point>& Points);
        void  detect_depth(const cv::Mat &inputsrc,const cv::Mat &depth,vector<vector<cv::Point>>& Points);
		 int width;
		 int height;
		 vector<Point> Allpoint_afterhough;
	     Mat graysrc;
		//�������ܣ���������ȡ��
		inline int round(double r)  
		{  
			return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);  
		} 
		//�޸Ĳ���
		 void change_THRED_GRAD(int numyouwantchange);
		 void change_THRED_REMOVE_DISORDER(float numyouwantchange);
	    
		void change_WINDOWSIZE_REMOVE_DISORDER(int numyouwantchange);

		private:
			//�ݶ���ȡ��ֵ
			int THRED_GRAD;
			//����ϸ��ȥ����ֵ
			float THRED_REMOVE_DISORDER;
			//���������ֵ
			int THRED_DEPTH_MEAN;
			int THRED_DEPTH_VAR;
		    //����ϸ��ȥ�����ڴ�С
			int WINDOWSIZE_REMOVE_DISORDER;
	};
#endif
