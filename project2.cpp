#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
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
	//cv::pow(matSrc,gamma,matDst);
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


int main(){
	float gamma;
//	init("Fig0343(a)(skeleton_orig).tif");
	cout<<"input gamma value : ";
	cin>>gamma;
	init("Fig0343(a)(skeleton_orig).tif");
	//powerLawTrans(gamma);
	logTrans(5);




}
