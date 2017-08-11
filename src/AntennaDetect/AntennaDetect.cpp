/*This file is part of AntennaDetect.
 *
 *  Created on: 2016��10��22��
 *      Author: Zuber
 */
#include "AntennaDetect.h"
	 template <typename T>   
void filterSpecklesImpl(cv::Mat& img, vector<region> &points,int newVal, int maxSpeckleSize, int maxDiff, cv::Mat& _buf)   
            {   
                
                 using namespace cv;   
                 int width = img.cols, height = img.rows, npixels = width*height;   
                 size_t bufSize = npixels*(int)(sizeof(cv::Point_<short>) + sizeof(int) + sizeof(uchar)); //���������ֶ���  
                 if( !_buf.isContinuous() || !_buf.data || _buf.cols*_buf.rows*_buf.elemSize() < bufSize )   
                _buf.create(1, (int)bufSize, CV_8U);   
                uchar* buf = _buf.data;   
                int i, j, dstep = (int)(img.step/sizeof(T));   
                int* labels = (int*)buf;   
                buf += npixels*sizeof(labels[0]);   
                cv::Point_<short>* wbuf = (cv::Point_<short>*)buf;   
                buf += npixels*sizeof(wbuf[0]);   
                uchar* rtype = (uchar*)buf;   
                int curlabel = 0;   
            // clear out label assignments   
            memset(labels, 0, npixels*sizeof(labels[0]));   
            for( i = 0; i < height; i++ )   
            {   
                T* ds = img.ptr<T>(i);   
                int* ls = labels + width*i;   
                for( j = 0; j < width; j++ )   
                 {   
                    if( ds[j] != newVal ) // not a bad disparity   
                        {   
                            if( ls[j] ) // has a label, check for bad label   
                                {   
                                      
                                if( rtype[ls[j]] ) // small region, zero out disparity   
                                ds[j] = (T)newVal;   
                                }   
							 // no label, assign and propagate   
                            else   
                            {   
                                cv::Point_<short>* ws = wbuf; // initialize wavefront   
                                cv::Point_<short> p((short)j, (short)i); // current pixel   
                                curlabel++; // next label   
                                int count = 0; // current region size   
                                ls[j] = curlabel;  
								//Mat show1=Mat::zeros(height,width,CV_8UC1);
								region pointregion;
                            // wavefront propagation   
                            while( ws >= wbuf ) // wavefront not empty   
                                {   
                                    count++;   
                                    // put neighbors onto wavefront   
                                    T* dpp = &img.at<T>(p.y, p.x);   
                                    T dp = *dpp; //p(x,y)������ֵ  
                                    int* lpp = labels + width*p.y + p.x;   
                                    if( p.y < height-1 && !lpp[+width] && dpp[+dstep] != newVal && std::abs(dp - dpp[+dstep]) <= maxDiff )   
                                        {   
                                            lpp[+width] = curlabel;   
                                            *ws++ = cv::Point_<short>(p.x, p.y+1);  // show1.at<uchar>( p.y+1,p.x)=255;
											
											pointregion.Pionts.push_back(Point(p.x, p.y+1));
                                        }   
                                    if( p.y > 0 && !lpp[-width] && dpp[-dstep] != newVal && std::abs(dp - dpp[-dstep]) <= maxDiff )   
                                        {   
                                            lpp[-width] = curlabel;   
                                            *ws++ = cv::Point_<short>(p.x, p.y-1); //	show1.at<uchar>( p.y-1,p.x)=255;  
											pointregion.Pionts.push_back(Point(p.x, p.y-1));
                                        }   
                                    if( p.x < width-1 && !lpp[+1] && dpp[+1] != newVal && std::abs(dp - dpp[+1]) <= maxDiff )   
                                        {   
                                            lpp[+1] = curlabel;   
                                            *ws++ = cv::Point_<short>(p.x+1, p.y);   //	show1.at<uchar>( p.y,p.x+1)=255;
											pointregion.Pionts.push_back(Point(p.x+1, p.y));
                                        }   
                                    if( p.x > 0 && !lpp[-1] && dpp[-1] != newVal && std::abs(dp - dpp[-1]) <= maxDiff )   
                                        {   
                                            lpp[-1] = curlabel;   
                                            *ws++ = cv::Point_<short>(p.x-1, p.y);   	//show1.at<uchar>( p.y,p.x-1)=255;
											pointregion.Pionts.push_back(Point(p.x-1, p.y));
                                        }   
                                    // pop most recent and propagate   
                                    // NB: could try least recent, maybe better convergence   
                                    p = *--ws;   
                                }   
									
                                // assign label type   
                                if( count <= maxSpeckleSize ) // speckle region   
                                  {   
                                    rtype[ls[j]] = 1; // small region label   
                                    ds[j] = (T)newVal;   
                                   }   
                                else   
								{
                                   rtype[ls[j]] = 0; // large region label   
								   points.push_back(pointregion);
								//  imshow(" dd" ,show1);
								//	waitKey(0);
								}
                             }   
                        }   
                  }   
   
          }   
    }   



Detect_Wireline::Detect_Wireline()
	{
			//�ݶ���ȡ��ֵ
			 THRED_GRAD=20;
			//����ϸ��ȥ����ֵ
            THRED_REMOVE_DISORDER=0.5;
			
				//���������ֵ
			THRED_DEPTH_MEAN=50;
			 THRED_DEPTH_VAR=1;
			//�����ڴ�С

		    //����ϸ��ȥ�����ڴ�С
			 WINDOWSIZE_REMOVE_DISORDER=15;
			
	}
//�������ܣ��޸Ĳ���
void Detect_Wireline::change_THRED_GRAD(int numyouwantchange)
{
	THRED_GRAD=numyouwantchange;
}
 void Detect_Wireline::change_THRED_REMOVE_DISORDER(float numyouwantchange)
{
	THRED_REMOVE_DISORDER=numyouwantchange;
}

 void Detect_Wireline::change_WINDOWSIZE_REMOVE_DISORDER(int numyouwantchange)
{
	WINDOWSIZE_REMOVE_DISORDER=numyouwantchange;
}


//�������ܣ�������Ҷ�ͼ��ÿһ�����ص���sobel���Ӽ���ݶȣ��������ص���ݶ�С����ֵthredʱ����ɫ��ʾ����һͨ��Ϊ255��
 void Detect_Wireline::detect_grad(const Mat&grayinput,Mat&grad,float thred)
		{
			
				grad=grayinput.clone();
			Mat dst_x,dst_y,dst;

			Sobel(grayinput, dst_x, grayinput.depth(), 1, 0);
			Sobel(grayinput, dst_y, grayinput.depth(), 0, 1);
			convertScaleAbs(dst_x, dst_x);
			convertScaleAbs(dst_y, dst_y);
			addWeighted(dst_x, 0.5, dst_y, 0.5, 0, dst);

				
			for(int j=0;j<height;j++)
				for(int i=0;i<width;i++)
				{ 
					if(dst.at<uchar>(j,i)<thred)
					  {		 
                         grad.at<uchar>(j,i)=0;
					   }
                }
 }



 //�������ܣ� �����߶μ��
 //���룺�����ͼ��
 //���������߶���ʾͼ���߶��ϵ����е�
 void Detect_Wireline:: houghcheck(const Mat&input,Mat&output,vector<line_>& pointsofLines_k)
     {
         output=Mat::zeros(input.size(),CV_8UC1);

          Mat show;
          cvtColor(input,show,CV_GRAY2BGR);
     //�����߶μ��
         vector<Vec4i> lines;
           HoughLinesP(input, lines, 1, CV_PI/180, 10, 50, 3 );
           for( size_t i = 0; i < lines.size(); i++ )
           {
             Vec4i l = lines[i];
             //cout<<l<<endl; //���ÿ��ֱ�ߵ������յ�
             line( show, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255),1, CV_AA);
           }

         // imshow("show",show);

          //��ֱ�߻������ͼ���ϲ������Щֱ���ϵĵ�����

           putline_into_pic(output,lines,pointsofLines_k);
          // imwrite("output.png",output);
     imshow("output",output);
      }
 //�������ܣ�������ʼ�����꣬����ֱ�ߡ�����ֱ�������е���������
 //���룺��Ҫ�����Ƶ�ͼ��ֱ�ߵ���ʼ������
 //�����������ϵ�ͼ��ֱ�������е�����ꡣ
 void Detect_Wireline::putline_into_pic( Mat& pic,vector<Vec4i>& lines,vector<line_>& pointsofLines_k)
     {

         for(auto it=lines.begin();it<lines.end();it++)
         {
                  line_ pointsofLine_k;
                  vector<Point> pointofLine;
                  Vec4i P =*it;
                 int x0=	P[0],y0=P[1],x1=P[2],y1=P[3];
                 if (y0==y1)
                 {
                     int drawbegin=x0<x1?x0:x1;
                     int drawend=x0>x1?x0:x1;
                      for(int x=drawbegin;x<drawend;x++)
                      {
                      pic.at<uchar>(y0,x)=255;
                      pointofLine.push_back(Point(x,y0));
                      }
                     pointsofLine_k.PiontsofLine=pointofLine;
                     pointsofLine_k.k=0;
                     pointsofLine_k.b=0;
                 }
                 else if(x0==x1)
                 {
                     int drawbegin=y0<y1?y0:y1;
                     int drawend=y0>y1?y0:y1;
                      for(int y=drawbegin;y<drawend;y++)
                      {
                        pic.at<uchar>(y,x0)=255;
                        pointofLine.push_back(Point(x0,y));
                      }
                      pointsofLine_k.PiontsofLine=pointofLine;
                     pointsofLine_k.k=-1;
                     pointsofLine_k.b=0;
                 }
                 else //б��
                 {
                   float k=((float)(y1-y0))/((float)(x1-x0));
                   float b=y0-k*x0;

                   if(abs(k)<1)//����߱Ƚ�ˮƽ���ͺ��Ż�
                       {
                       int drawbegin=x0<x1?x0:x1;
                       int drawend=x0>x1?x0:x1;
                       for(int x=drawbegin;x<drawend;x++)
                          {
                            float y_=k*x+b;
                             int y=round(y_);
                             pic.at<uchar>(y,x)=255;
                             pointofLine.push_back(Point(x,y));
                          }
                       pointsofLine_k.PiontsofLine=pointofLine;
                       pointsofLine_k.k=k;
                       pointsofLine_k.b=b;
                       }
                   else//����߱Ƚϴ�ֱ�������Ż�
                     {
                          int drawbegin=y0<y1?y0:y1;
                           int drawend=y0>y1?y0:y1;

                             for(int y=drawbegin;y<drawend;y++)
                               {
                                  float x_=(y-b)/k;
                                  int x=round(x_);
                                pic.at<uchar>(y,x)=255;
                                 pointofLine.push_back(Point(x,y));
                               }
                         pointsofLine_k.PiontsofLine=pointofLine;
                           pointsofLine_k.k=k;
                           pointsofLine_k.b=b;
                     }
                 }
                 pointsofLines_k.push_back(pointsofLine_k);
         }
     }
//�������ܣ����ϸС����
void Detect_Wireline::detectsmallobject(const Mat&gradinput,Mat&outputwithsamllobject,int windowsize)
	  {
				outputwithsamllobject=gradinput.clone();
			
				for(int j=0;j<height;j++)
				for(int i=0;i<width;i++)
				{
						 if(gradinput.at<uchar>(j,i)==0) 
				     		{
							continue;//��������������أ�ֱ��������
							}
						  else
						   {

							int x_begin= i-windowsize/2>=0?windowsize/2:i;
							int x_end =width-i-1>windowsize/2?windowsize/2:width-i-1;
							int y_begin=j-windowsize/2>=0?windowsize/2:j;
							int y_end =height-j-1>windowsize/2?windowsize/2:height-j-1;
                            int lowtextrue_cnt=0;
                            int wincol=x_end+x_begin+1;
                            int winrow=y_end+y_begin+1;
                         //   Mat  wincheck=Mat::zeros(winrow,wincol,CV_8UC1);
							//ͳ������Щ������������Ϊ���ĵĴ�����  ����������վ�ı��أ�����ĳһ��ֵ������Ϊ�Ƿ�ϸС���塣
							for(int win_row=j-y_begin;win_row<=j+y_end;win_row++)
								for(int win_col=i-x_begin;win_col<=i+x_end;win_col++)
                                {
                             //      wincheck.at<uchar>(win_row-(j-y_begin),win_col-(i-x_begin))=gradinput.at<uchar>(win_row,win_col);
								   if(gradinput.at<uchar>(win_row,win_col)==0)//255
									   ++lowtextrue_cnt;
								}
//imshow("",wincheck);waitKey(0);
						  float lowtextrue_ratio=(float)lowtextrue_cnt/(float)((y_end+y_begin+1)*(x_begin+x_end+1));		
					  
								   if(lowtextrue_ratio<THRED_REMOVE_DISORDER)
                                   {
                                                outputwithsamllobject.at<uchar>(j,i)=0;//255
								  }
						}
				}

	     }
 void  Detect_Wireline::dispcheck( const vector<region> regions,const Mat&disp,Mat &output,vector<region>& regions_afterdispcheck,int windowsize,int Dmax)
 {
	 for(int i=0;i<regions.size();i++)
	    {	
			  vector<vector<bool>> flags;
			 for(int j=0;j<height;j++)
			 {
				  vector<bool> flag;
				for(int i=0;i<width;i++)
				   {
					  flag.push_back(false);
				   }
				flags.push_back(flag);
			 }
		
			 vector<Point> pixel_around_line;
			 int unvalid=0;
			 for(int j=0;j<regions[i].Pionts.size();j++)
			 {
				    int px=regions[i].Pionts[j].x;
				    int py=regions[i].Pionts[j].y;
			        int x_begin= px-windowsize/2>=0?windowsize/2:px;
					int x_end =width-px-1>windowsize/2?windowsize/2:width-px-1;
					int y_begin=py-windowsize/2>=0?windowsize/2:py;
					int y_end =height-py-1>windowsize/2?windowsize/2:height-py-1;
			            for(int win_row=py-y_begin;win_row<=py+y_end;win_row++)
						for(int win_col=px-x_begin;win_col<=px+x_end;win_col++)
						{
							//ȥ�������Ѿ�����pixel_around_line�ĵ��ֱ���ϵĵ�
						if(flags[win_row][win_col]==true)	
									continue;
						if(disp.at<uchar>(win_row,win_col)==0&&win_col>Dmax)
						{
							    ++unvalid;
								
						}      
							pixel_around_line.push_back(Point(win_col,win_row));
							flags[win_row][win_col]=true;
							// cout<<"win_row  "<<win_row<<"win_col  "<<win_col<<endl;

						}
			 }
			 float dispvalid_ratio=1.0f-((float)unvalid)/pixel_around_line.size();
			// //ֱ�߸�����Ч����ʱȽϸߣ�˵������߶θ�����������������ͼ��׼ȷ�����ǲ�Ҫ������ֱ�ߡ�
			//  cout<<"dispvalid_ratio  "<<dispvalid_ratio<<endl;
			// if(dispvalid_ratio>0.9)
			//	 continue;
		
			 //�����߶θ������򣨳����߶����أ����������ƽ��ֵ
			  float average=0;
			 for(vector<Point>::iterator it=pixel_around_line.begin();it!=pixel_around_line.end();it++)
			 {
				 if(disp.at<uchar>(it->y,it->x)==0)
					 continue;
			    average+=disp.at<uchar>(it->y,it->x);
			 }
	         average/=pixel_around_line.size();
		
			  //�����߶θ������򣨳����߶����أ���������ȷ���
			 float var=0;
			  for(vector<Point>::iterator it=pixel_around_line.begin();it!=pixel_around_line.end();it++)
			 {
				 if(disp.at<uchar>(it->y,it->x)==0)
					 continue;
			 var+=sqrt((disp.at<uchar>(it->y,it->x)-average)*(disp.at<uchar>(it->y,it->x)-average));
			 }
			  var/=pixel_around_line.size();
			 // cout<<"average  "<<average <<"var  "<<var<<endl;

			  //����ֻҪ�������߶Σ�����Χ������ȣ������Լ�����������������	
			  //  2 ��ȷ���Ƚϴ�
            if(var>THRED_DEPTH_VAR*sqrt(average)||dispvalid_ratio<0.9)
	 			regions_afterdispcheck.push_back(regions[i]);
	    }
		//���ֱ�߼����ϸ��ͼ
		   output=Mat::zeros(height,width,CV_8UC1);
		 for(int linenum=0;linenum<regions_afterdispcheck.size();linenum++)
		 {
			 for(vector<Point>::iterator it=regions_afterdispcheck[linenum].Pionts.begin();it!=regions_afterdispcheck[linenum].Pionts.end();it++)
			   output.at<uchar>(it->y,it->x)=255;	 
		 }
		
		
	}
 void  Detect_Wireline::dispcheck( const vector<region> regions,const Mat&disp,vector<cv::Point>& Points,vector<region>& regions_afterdispcheck,int windowsize,int Dmax)
 {
	 for(int i=0;i<regions.size();i++)
	    {	//Mat show=Mat::zeros(disp.size(),CV_8UC1);
			  vector<vector<bool>> flags;
			 for(int j=0;j<height;j++)
			 {
				  vector<bool> flag;
				for(int i=0;i<width;i++)
				   {
					  flag.push_back(false);
				   }
				flags.push_back(flag);
			 }
		
			 vector<Point> pixel_around_line;
			//vector<Point> pixel_around_line_oneside;
			//vector<Point> pixel_around_line_anotherside;
			
			 int unvalid=0;
			float LinePixelAvgColor=0;
			float OnesideNeighborAvgColor=0,OthersideNeighborAvgColor=0;
			int OnesideNeighborCnt=0,OthersideNeighborCnt=0;
			 for(int j=0;j<regions[i].Pionts.size();j++)
			 {
				    int px=regions[i].Pionts[j].x;
				    int py=regions[i].Pionts[j].y;
//show.at<uchar>(py,px)=255;
					LinePixelAvgColor+=graysrc.at<uchar>(py,px);//ͳ��ֱ���ϵ�ƽ���Ҷ�
			        	int x_begin= px-windowsize/2>=0?windowsize/2:px;
					int x_end =width-px-1>windowsize/2?windowsize/2:width-px-1;
					int y_begin=py-windowsize/2>=0?windowsize/2:py;
					int y_end =height-py-1>windowsize/2?windowsize/2:height-py-1;
			            for(int win_row=py-y_begin;win_row<=py+y_end;win_row++)
					{
							bool PutInThisSide=true;//�ֲ���ñ�־λ
						for(int win_col=px-x_begin;win_col<=px+x_end;win_col++)
						{
						if(win_col==px) PutInThisSide=false;//�ֽ���
							//ȥ�������Ѿ�����pixel_around_line�ĵ��ֱ���ϵĵ�
						if(flags[win_row][win_col]==true)//||win_col==px)	
									continue;
						if(disp.at<uchar>(win_row,win_col)==0&&win_col>Dmax)
						{
							    ++unvalid;
								
						}      
							
							if(PutInThisSide)//ͳ��ֱ��һ������ص�Ҷ�
								{
								OnesideNeighborAvgColor+=graysrc.at<uchar>(win_row,win_col);
								++OnesideNeighborCnt;
								}
							
							else //ͳ��ֱ����һ������ص�Ҷ�
								{
								OthersideNeighborAvgColor+=graysrc.at<uchar>(win_row,win_col);
								++OthersideNeighborCnt;
								}
								
							pixel_around_line.push_back(Point(win_col,win_row));//ֱ����Χ�����أ�����������
							flags[win_row][win_col]=true;
							// cout<<"win_row  "<<win_row<<"win_col  "<<win_col<<endl;

						}
			                 }
			}
//imshow("",show);waitKey(0);
			//LinePixelAvgColor/=regions[i].Pionts.size();
			//OnesideNeighborAvgColor/=OnesideNeighborCnt;
			//OthersideNeighborAvgColor/=OthersideNeighborCnt;
//			cout<<"LinePixelAvgColor "<<LinePixelAvgColor<<endl;
//cout<<OnesideNeighborAvgColor<<"  "<<OthersideNeighborAvgColor<<endl;
			 float dispvalid_ratio=1.0f-((float)unvalid)/pixel_around_line.size();
//cout<<pixel_around_line.size()<<endl;
			// //ֱ�߸�����Ч����ʱȽϸߣ�˵������߶θ�����������������ͼ��׼ȷ�����ǲ�Ҫ������ֱ�ߡ�
			//  cout<<"dispvalid_ratio  "<<dispvalid_ratio<<endl;
			
		
			 //�����߶θ������򣨳����߶����أ����������ƽ��ֵ
			  float average=0;
			 for(vector<Point>::iterator it=pixel_around_line.begin();it!=pixel_around_line.end();it++)
			 {
				 if(disp.at<uchar>(it->y,it->x)==0)
					 continue;
			    average+=disp.at<uchar>(it->y,it->x);
			 }
	         average/=pixel_around_line.size();
		
			  //�����߶θ������򣨳����߶����أ���������ȷ���
			 float var=0;
			  for(vector<Point>::iterator it=pixel_around_line.begin();it!=pixel_around_line.end();it++)
			 {
				 if(disp.at<uchar>(it->y,it->x)==0)
					 continue;
			 var+=sqrt((disp.at<uchar>(it->y,it->x)-average)*(disp.at<uchar>(it->y,it->x)-average));
			 }
			  var/=pixel_around_line.size();
			 // cout<<"average  "<<average <<"var  "<<var<<endl;

			  //����ֻҪ�������߶Σ�����Χ������Ⱦ�������������
				//1 ��ȷ���Ƚϴ�
				//2 �����Ч�ʸ�
            if(var>THRED_DEPTH_VAR*sqrt(average)||dispvalid_ratio<0.9)
	 			regions_afterdispcheck.push_back(regions[i]);
	    }
		
//���ֱ�߼����ϸ��
		  
		 for(int linenum=0;linenum<regions_afterdispcheck.size();linenum++)
		 
			 for(vector<Point>::iterator it=regions_afterdispcheck[linenum].Pionts.begin();it!=regions_afterdispcheck[linenum].Pionts.end();it++)	
			
					Points.push_back(*it);                    
	}
void  Detect_Wireline::SlopeCheck(const 	vector<region >& regions,vector<region >&AfterSlopeCheck)
{
    for(auto region:regions)
    {
       if(abs(CalLineSlope(region))>1) continue;
       AfterSlopeCheck.push_back(region);
    }

}
float Detect_Wireline::CalLineSlope(const region&Line)
{
  int linesize=Line.Pionts.size();
  vector<Point> checkPoints;
  vector<float> slopes;
  //��ֱ����ÿ�����������
  for(int i=0;i<linesize;i+=5)
  {
     checkPoints.push_back(Line.Pionts[i]);
  }
  //����б��
for(int i=1;i<checkPoints.size();i++)
{
    cv::Point slidePt=checkPoints[i];
    float slope=INT32_MAX;
    //�������̫��������
    if(abs(slidePt.y-checkPoints[0].y)+abs(slidePt.x-checkPoints[0].x)<3)
        continue;
    if(checkPoints[i].x==checkPoints[0].x)
        slope=1000;
     else
     slope = (float)(checkPoints[i].y-checkPoints[0].y)/(checkPoints[i].x-checkPoints[0].x);

   slopes.push_back(slope);
}
//���Ϊƽ��б��
return ((float)std::accumulate(slopes.begin(),slopes.end(),0))/slopes.size();
}
void  Detect_Wireline::depthcheck( const vector<region> regions,const Mat&depth,vector<vector<cv::Point>>& ALLPoints,vector<region>& regions_afterdepthcheck,int windowsize,int Dmax)
{
 for(int i=0;i<regions.size();i++)
        {                                                                                                                                                    //   Mat show=Mat::zeros(depth.size(),CV_8UC1);
			  vector<vector<bool>> flags;
			 for(int j=0;j<height;j++)
			 {
				  vector<bool> flag;
				for(int i=0;i<width;i++)
				   {
					  flag.push_back(false);
				   }
				flags.push_back(flag);
			 }
		
			 vector<Point> pixel_around_line;
			 int unvalid=0;
			 for(int j=0;j<regions[i].Pionts.size();j++)
			 {
				    int px=regions[i].Pionts[j].x;
				    int py=regions[i].Pionts[j].y;

                                                                                                                                                           //  show.at<uchar>(py,px)=255;
			        	int x_begin= px-windowsize/2>=0?windowsize/2:px;
					int x_end =width-px-1>windowsize/2?windowsize/2:width-px-1;
					int y_begin=py-windowsize/2>=0?windowsize/2:py;
					int y_end =height-py-1>windowsize/2?windowsize/2:height-py-1;
			            for(int win_row=py-y_begin;win_row<=py+y_end;win_row++)
                                    for(int win_col=px-x_begin;win_col<=px+x_end;win_col++)
                                    {
                                        //ȥ�������Ѿ�����pixel_around_line�ĵ��ֱ���ϵĵ�
                                    if(flags[win_row][win_col]==true)//||win_col==px)
                                                continue;
                                    if(depth.at<float>(win_row,win_col)==0&&win_col>Dmax)
                                    {
                                            ++unvalid;

                                    }
                                       if(win_col>Dmax)//
                                       {
                                            pixel_around_line.push_back(Point(win_col,win_row));//ֱ����Χ�����أ�����������
                                            flags[win_row][win_col]=true;
                                       }
                                        // cout<<"win_row  "<<win_row<<"win_col  "<<win_col<<endl
                                    }

			}
if(pixel_around_line.size()==0)  continue;//

			 float depthvalid_ratio=1.0f-((float)unvalid)/pixel_around_line.size();
// cout<<pixel_around_line.size()<<endl;
//imshow("",show);waitKey(0);
			// //ֱ�߸�����Ч����ʱȽϸߣ�˵������߶θ�����������������ͼ��׼ȷ�����ǲ�Ҫ������ֱ�ߡ�
           //   cout<<"depthvalid_ratio  "<<depthvalid_ratio<<endl;
			
		
			 //�����߶θ������򣨳����߶����أ����������ƽ��ֵ
			  float average=0;
			 for(vector<Point>::iterator it=pixel_around_line.begin();it!=pixel_around_line.end();it++)
			 {
				 if(depth.at<float>(it->y,it->x)==0)
					 continue;
			    average+=depth.at<float>(it->y,it->x);
			 }
	         average/=pixel_around_line.size();
		
			  //�����߶θ������򣨳����߶����أ���������ȷ���
			 float var=0;
			  for(vector<Point>::iterator it=pixel_around_line.begin();it!=pixel_around_line.end();it++)
			 {
				 if(depth.at<float>(it->y,it->x)==0)
					 continue;
			 var+=sqrt((depth.at<float>(it->y,it->x)-average)*(depth.at<float>(it->y,it->x)-average));
			 }
			  var/=pixel_around_line.size();
            //  cout<<"average  "<<average <<"var  "<<var<<endl;

			  //����ֻҪ�������߶Σ�����Χ������Ⱦ�������������
				//1 ��ȷ���Ƚϴ�
				//2 �����Ч�ʸ�
            if(var>THRED_DEPTH_VAR*sqrt(average)||depthvalid_ratio<0.9)
	 			regions_afterdepthcheck.push_back(regions[i]);
	    }
		
//���ֱ�߼����ϸ��
		  
		 for(int linenum=0;linenum<regions_afterdepthcheck.size();linenum++)
         {
             vector<cv::Point > Points_;
             for(vector<Point>::iterator it=regions_afterdepthcheck[linenum].Pionts.begin();it!=regions_afterdepthcheck[linenum].Pionts.end();it++)	{
                                       Points_.push_back(*it);
             }
              ALLPoints.push_back(Points_);
         }

}
  
void  Detect_Wireline::detect_depth(const cv::Mat &inputsrc,const cv::Mat &depth,vector<vector<cv::Point>>& Points)
{
	 width=inputsrc.cols;
	 height=inputsrc.rows;
	   graysrc=inputsrc.clone();
	 Mat input;
	if(inputsrc.channels()==3)	
		cvtColor(inputsrc,input,CV_BGR2GRAY);
	else
		input=inputsrc;

	//�ݶȼ��
	
		
			Mat grad;
			detect_grad(input,grad,20);
          // imshow("grad ",grad);
		//	imwrite("grad.png",grad);
	
		//�����ݶ�����
            change_THRED_REMOVE_DISORDER(0.7);
		Mat  smallobject1;
        detectsmallobject(grad,smallobject1,15);//15
                                                 //  imshow("smallobject ",smallobject1);
                                                    //imwrite("smallobject.png",smallobject1);
//vector <line_> a;
  //      houghcheck(grad,smallobject1,a);
	//��һ����ͨ������
	Mat  tmp;
	vector<region > regions ;

      filterSpecklesImpl<uchar>(smallobject1,regions,0,300,20,tmp);//150
  //б�ʼ��
    vector<region > regionsafterSLOPE;
    SlopeCheck(regions,regionsafterSLOPE);
// imshow("smallobject1",smallobject1);
	vector<region > regions_afterdisp ;
    depthcheck(regionsafterSLOPE,depth,Points,regions_afterdisp,10);//15
}
 
void  Detect_Wireline::detect(const cv::Mat &inputsrc,const cv::Mat &disp,vector<cv::Point>& Points)
 {
	width=inputsrc.cols;
	 height=inputsrc.rows;
	   graysrc=inputsrc.clone();
	 Mat input,disp8;
	if(inputsrc.channels()==3)	
		cvtColor(inputsrc,input,CV_BGR2GRAY);
	else
		input=inputsrc;

	//�ݶȼ��
	
		
			Mat grad;
			detect_grad(input,grad,20);
		//	imshow("grad ",grad);
		//	imwrite("grad.png",grad);
	
		//�����ݶ�����
			change_THRED_REMOVE_DISORDER(0.7);
		Mat  smallobject1;
		detectsmallobject(grad,smallobject1,15);//15
													//imshow("smallobject ",smallobject1);
													//imwrite("smallobject.png",smallobject1);

	//��һ����ͨ������
	Mat  tmp;
	vector<region > regions ;

      filterSpecklesImpl<uchar>(smallobject1,regions,0,200,20,tmp);//150


	//imshow("smallobject1",smallobject1);
	vector<region > regions_afterdisp ;
	dispcheck(regions,disp,Points,regions_afterdisp,15);//15

        /*Mat regionss=cv::Mat::zeros(height,width,CV_8UC1);

			 for(vector<Point>::iterator it=Points.begin();it!=Points.end();it++)
				{	regionss.at<uchar>(it->y,it->x)=255;
			
				}
        imshow("regionss",regionss);*/
   
 }


