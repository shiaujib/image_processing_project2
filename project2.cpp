#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include<iostream>
#include<math.h>
using namespace std;
using namespace cv;

Mat matSrc,matDst;


void init(String str){
	matSrc=imread(str,0);
	matDst=Mat(matSrc.size(),CV_8UC1);
	imshow("input",matSrc);
	waitKey(0);
	
	
}

void powerLawTrans(float gamma){    //RGB type is uchar
	for(int i=0;i<matDst.cols;i++)
		for(int j=0;j<matDst.rows;j++){
			matDst.at<uchar>(j,i)=255*std::pow(matSrc.at<uchar>(j,i)/255.0,gamma);
			}
	imshow("result",matDst);
	waitKey(0);
}

void histogram(){
	int total,max=0;
	int hisarray[256]={0};
	Mat figure=Mat(Size(400,300),CV_8UC3);
	
	for(int i=0;i<matSrc.cols;i++)
		for(int j=0;j<matSrc.rows;j++){
			total+=(int)matSrc.at<uchar>(j,i);
			hisarray[(int)matSrc.at<uchar>(j,i)]++;
			
			}
	
	for(int i=0;i<256;i++){
		if(hisarray[i]>max)
			max=hisarray[i];
		cout<<hisarray[i]<<endl;
		}
	cout<<"max "<<max<<endl;
	int intensity;
	int scale=2;
	double bwidth=(double)figure.cols/256;
	double bheight=(double)figure.rows/max;
	cout<<"   bbb"<<bwidth<<endl;
	for(int i=0;i<256;i++){
//		intensity=100*cvRound(hisarray[i]*255/max);
	//	rectangle(figure,Point(i*scale,255),Point((i+1)*scale,255-intensity),  CV_RGB(255,255,255));
		rectangle(figure,Point(i*bwidth,figure.rows),Point((i+1)*bwidth,figure.rows-hisarray[i]*bheight),  Scalar(0,255),-1,8,0);
		
		}
	imshow("graph",figure);
	waitKey(0);	
}	


int main(){
	float gamma;
//	init("Fig0343(a)(skeleton_orig).tif");
//	cout<<"input gamma value : ";
//	cin>>gamma;
//	init("Fig0343(a)(skeleton_orig).tif");
	init("lena.jpg");
	//powerLawTrans(gamma);
	histogram();




}
