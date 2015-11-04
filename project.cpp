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
void logTrans(float grayLevel){
	for(int i=0;i<matDst.cols;i++)
		for(int j=0;j<matDst.rows;j++){
			matDst.at<uchar>(j,i)=(int)255*log(matSrc.at<uchar>(j,i)/255.0+grayLevel);
			}
	//cv::pow(matSrc,gamma,matDst);
	imshow("result",matDst);
	waitKey(0);
	

}

void smoothing_filt(int n){
	int sum=0;
	n=sqrt(n);
	for(int i=0;i<matDst.cols;i++)
		for(int j=0;j<matDst.rows;j++){
			//if(i){
			for(int k=i-1;k<i+n-1;k++)
				for(int z=j-1;z<j+n-1;z++){
					sum+=matDst.at<uchar>(z,k);
				}
			//}
			matDst.at<uchar>(j,i)=sum/(n*n);
			sum=0;
		}
	imshow("Result",matDst);
	waitKey(0);

}	
			
void laplace(){
	int value;
	int sum=0;
	for(int i=0;i<matDst.cols;i++)
		for(int j=0;j<matDst.rows;j++){
		/*	value=matSrc.at<uchar>(j,i+1)+matSrc.at<uchar>(j,i-1)+matSrc.at<uchar>(j+1,i)+matSrc.at<uchar>(j-1,i)-4*matSrc.at<uchar>(j,i);
			if(value>0)
				matDst.at<uchar>(j,i)=matSrc.at<uchar>(j,i)+value;
			else if(value<0)
				matDst.at<uchar>(j,i)=matSrc.at<uchar>(j,i)-value;
		  	*/
		/*	if(i==0&&j){
				sum+=8*(int)matDst.at<uchar>(j,i);
				sum-=(int)matDst.at<uchar>(j-1,i);
				sum-=(int)matDst.at<uchar>(j-1,i+1);
				sum-=(int)matDst.at<uchar>(j+1,i);
				sum-=(int)matDst.at<uchar>(j+1,i);
				sum-=(int)matDst.at<uchar>(j+1,i+1);
				matDst.at<uchar>(j,i)=sum/6;
				sum=0;



			}
			if(i&&j==0){
				sum+=8*(int)matDst.at<uchar>(j,i);
				sum-=(int)matDst.at<uchar>(j,i-1);
				sum-=(int)matDst.at<uchar>(j,i+1);
				sum-=(int)matDst.at<uchar>(j+1,i-1);
				sum-=(int)matDst.at<uchar>(j+1,i);
				sum-=(int)matDst.at<uchar>(j+1,i+1);
				matDst.at<uchar>(j,i)=sum/6;
				sum=0;

			}
			if(i==0&&j==0){
				sum+=8*(int)matDst.at<uchar>(j,i);
				sum-=(int)matDst.at<uchar>(j,i+1);
				sum-=(int)matDst.at<uchar>(j+1,i);
				sum-=(int)matDst.at<uchar>(j+1,i+1);
				matDst.at<uchar>(j,i)=sum/4;
				sum=0;
				
			}*/




			for(int k=i-1;k<i+2;k++)
				for(int z=j-1;z<j+2;z++){
					
					if(z==j&&k==i)
						sum+=7*matDst.at<uchar>(z,k);

					else
						sum-=matDst.at<uchar>(z,k);
						
					
				
				}
			matDst.at<uchar>(j,i)=sum/9;
			sum=0;
			//cout<<(int)matDst.at<uchar>(-100,-100)<<endl;
			/*sum+=8*(int)matDst.at<uchar>(j,i);
			sum-=(int)matDst.at<uchar>(j-1,i-1);
			sum-=(int)matDst.at<uchar>(j-1,i);
			sum-=(int)matDst.at<uchar>(j-1,i+1);
			sum-=(int)matDst.at<uchar>(j,i-1);
			sum-=(int)matDst.at<uchar>(j,i+1);
			sum-=(int)matDst.at<uchar>(j+1,i-1);
			sum-=(int)matDst.at<uchar>(j+1,i);
			sum-=(int)matDst.at<uchar>(j+1,i+1);
			//if(sum>0)
			//	sum=-100000000;

			matDst.at<uchar>(j,i)=sum/9;
			sum=0;*/
				
		}
	imshow("Result",matDst);
	waitKey(0);
}	
			



void histogram_equ(int hisarray[],int total){
	double pr[256]={0.0};
	double ps[256]={0.0};
	double temp;
	int s[256]={0};
	int value;
	for(int i=0;i<256;i++){     //claculate probability
		pr[i]=(double)hisarray[i]/total;
		temp+=pr[i];
		s[i]=cvRound(temp*255); //(L-1)*pr
		cout<<s[i]<<endl;
		//cout<<hisarray[i]<<endl;
		ps[s[i]]+=pr[i];

	}
	for(int i=0;i<matSrc.cols;i++)
		for(int j=0;j<matSrc.rows;j++){
			value=(int)matSrc.at<uchar>(j,i);
			value=s[value];
			matDst.at<uchar>(j,i)=value;
			}
	imshow("histogram equalize",matDst);
	waitKey(0);
			//for(int k=0;k<256;k++){
			//	if(value==s[i])
			
	
				
			
}
void histogram(){
	int total=0,max=0;
	int hisarray[256]={0};
	Mat figure=Mat(Size(400,300),CV_8UC3);
	
	for(int i=0;i<matSrc.cols;i++)
		for(int j=0;j<matSrc.rows;j++){
			total++;
			hisarray[(int)matSrc.at<uchar>(j,i)]++;
			
			
			}
	
	
	for(int i=0;i<256;i++){
		if(hisarray[i]>max)
			max=hisarray[i];
//		cout<<hisarray[i]<<endl;
		}
	cout<<"max "<<max<<endl;
	int intensity;
	int scale=2;
	double bwidth=(double)figure.cols/256;
	double bheight=(double)figure.rows/max;
//	cout<<"   bbb"<<bwidth<<endl;
	for(int i=0;i<256;i++){
//		intensity=100*cvRound(hisarray[i]*255/max);
	//	rectangle(figure,Point(i*scale,255),Point((i+1)*scale,255-intensity),  CV_RGB(255,255,255));
		rectangle(figure,Point(i*bwidth,figure.rows),Point((i+1)*bwidth,figure.rows-hisarray[i]*bheight),  Scalar(0,255),-1,8,0);
		
		}
	imshow("graph",figure);
	waitKey(0);
	histogram_equ(hisarray,total);	
}

	


int main(){
	float gamma;
//	init("Fig0343(a)(skeleton_orig).tif");
//	cout<<"input gamma value : ";
//	cin>>gamma;
//	init("Fig0338(a)(blurry_moon).tif");
	init("Fig0343(a)(skeleton_orig).tif");
	
	powerLawTrans(1);
//	histogram();
//	smoothing_filt(25);
	laplace();
//	cout<<"input gamma value : ";
//	cin>>gamma;
//	init("Fig0343(a)(skeleton_orig).tif");
	//powerLawTrans(gamma);
//	logTrans(5);




}
