#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include<iostream>
#include<math.h>
using namespace std;
using namespace cv;

Mat matSrc,matDst,matLs,matadd,matsdst,matldst,matsmooth,matgsmooth,matunsharp,matProduct,matLaplace,matresult,matHistogramEqu,matgsharp,matend,matPro,matPron,mataddFst,mataddSnd;


void init(String str){
	matSrc=imread(str,0);
	matDst=Mat(matSrc.size(),CV_8UC1);
	matldst=Mat(matSrc.size(),0);
	matLs=Mat(matSrc.size(),0);
	matadd=Mat(matSrc.size(),CV_8UC1);
	matsdst=Mat(matSrc.size(),CV_8UC1);
	matsmooth=Mat(matSrc.size(),CV_8UC1);
	matgsmooth=Mat(matSrc.size(),CV_8UC1);
	matunsharp=Mat(matSrc.size(),CV_8UC1);
	matProduct=Mat(matSrc.size(),CV_8UC1);
	matLaplace=Mat(matSrc.size(),CV_8UC1);
	matresult=Mat(matSrc.size(),CV_8UC1);
	matHistogramEqu=Mat(matSrc.size(),CV_8UC1);
	matgsharp=Mat(matSrc.size(),CV_8UC1);
	matend=Mat(matSrc.size(),CV_8UC1);
	matPro=Mat(matSrc.size(),CV_64FC1);
	matPron=Mat(matSrc.size(),CV_8UC1);
	mataddFst=Mat(matSrc.size(),CV_8UC1);
	mataddSnd=Mat(matSrc.size(),CV_8UC1);
	imshow("input",matSrc);
	waitKey(0);


}
void showImage(String str,Mat mat){
	imshow(str,mat);
	waitKey(0);
}



void productMatrix(Mat mata, Mat matb,Mat matout,int flag){
	int cols=mata.cols;
	int rows=mata.rows;
	double sum;
	if(flag==0){
	for(int i=0;i<cols;i++){
		for(int j=0;j<rows;j++){
			for(int k=0;k<cols;k++){
				sum+=mata.at<uchar>(i,k)*matb.at<uchar>(k,j);
			}
			matout.at<uchar>(i,j)=sum;
			sum=0;
		}
	}
	}
	if(flag==1){
	for(int i=0;i<cols;i++){
		for(int j=0;j<rows;j++){
			for(int k=0;k<cols;k++){
				sum+=mata.at<double>(i,k)*matb.at<double>(k,j);
			}
			matout.at<double>(i,j)=sum;
			sum=0;
		}
	}
	}
}

void normalize(Mat matin,Mat matout){
	int cols=matin.cols;
	int rows=matin.rows;
	double max=0,min=1000,value,scale;
	for(int i=0;i<cols;i++){
		for(int j=0;j<rows;j++){
			value=matin.at<double>(j,i);
			//cout<<value<<endl;
			if(value>max)
				max=value;
			if(value<min)
				min=value;
			}
	}
	for(int i=0;i<cols;i++){
		for(int j=0;j<rows;j++){
			scale=(matin.at<double>(j,i)-min)*255.0/(max-min);
			matout.at<uchar>(j,i)=scale;
		}
	}
	cout<<"max="<<max<<endl;
	cout<<"min="<<min<<endl;
}


void powerLawTrans(float gamma,Mat matin,Mat matout){    //RGB type is uchar
	for(int i=0;i<matDst.cols;i++)
		for(int j=0;j<matDst.rows;j++){
			matout.at<uchar>(j,i)=255*std::pow(matin.at<uchar>(j,i)/255.0,gamma);
		}
//	imshow("powerLaw",matout);
//	waitKey(0);
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

void smoothing_filt(int n,Mat matIn,Mat matOut){
	int sum=0;
	for(int i=1;i<matDst.cols;i++)
		for(int j=1;j<matDst.rows;j++){
			for(int k=i-1;k<i+n-1;k++)
				for(int z=j-1;z<j+n-1;z++){
					sum+=matIn.at<uchar>(z,k);
				}
			matOut.at<uchar>(j,i)=sum/(n*n);
			sum=0;
		}
	imshow("smooth Result",matOut);
	waitKey(0);

}	

void laplace(){
	int value=0;
	double sum=0;
	double scale=0;
	int max=0,min=0;
	int index=0;
	int sumarray[matDst.cols*matDst.rows];
	for(int i=1;i<matDst.cols-1;i++)
		for(int j=1;j<matDst.rows-1;j++){

			for(int k=i-1;k<i+2;k++)
				for(int z=j-1;z<j+2;z++){

					if(z==j&&k==i)
						sum+=8*matSrc.at<uchar>(z,k);

					else
						sum-=matSrc.at<uchar>(z,k);


				}
			if(sum>max)
				max=sum;
			if(sum<min)
				min=sum;
			if(sum>255)
				sum=255;
			else if(sum<0)
				sum=0;
			matLaplace.at<uchar>(j,i)=sum;

			sumarray[index++]=sum;

			scale=0;
			sum=0;

		}
	index=0;
	for(int i=1;i<matDst.cols-1;i++)
		for(int j=1;j<matDst.rows-1;j++){
			scale=(sumarray[index++]-min)*255/(max-min);	
			matDst.at<uchar>(j,i)=scale;
		}


	cout<<max<<endl<<min<<endl;
	imshow("Result",matDst);
	waitKey(0);
}

void add_sharpening(Mat mat1,Mat mat2,Mat matout){
	int sum;
	for(int i=0;i<matDst.cols;i++)
		for(int j=0;j<matDst.rows;j++){
			sum=mat1.at<uchar>(j,i)+mat2.at<uchar>(j,i);
			if(sum>255)
				sum=255;
			else if(sum<0)
				sum=0;
			matout.at<uchar>(j,i)=sum;
		}
}




void laplace_sharpening(Mat matin,Mat matout){
	int value=0;
	int sum=0;
	int result1,result2;
	for(int i=0;i<matDst.cols;i++)
		for(int j=0;j<matDst.rows;j++){
			value=matin.at<uchar>(j,i+1)+matin.at<uchar>(j,i-1)+matin.at<uchar>(j+1,i)+matin.at<uchar>(j-1,i)-4*matin.at<uchar>(j,i);
			result1=matin.at<uchar>(j,i)-value;
			if(result1>255)
				result1=255;
			else if(result1<0)
				result1=0;

			matout.at<uchar>(j,i)=result1;
		
		}
	imshow("laplace sharp",matout);
	waitKey(0);

}


void sobel_gradient(Mat matSrc,Mat matsdst){
	int sum1=0,sum2=0;
	int value;
	int scale=0;
	int min=0, max=0;
	int index=0;
	for(int i=1;i<matDst.cols-1;i++)
		for(int j=1;j<matDst.rows-1;j++){
			sum1+=matSrc.at<uchar>(j+1,i-1);
			sum1+=2*matSrc.at<uchar>(j+1,i);
			sum1+=matSrc.at<uchar>(j+1,i+1);
			sum1-=matSrc.at<uchar>(j-1,i-1);
			sum1-=2*matSrc.at<uchar>(j-1,i);
			sum1-=matSrc.at<uchar>(j-1,i+1);
			sum1=abs(sum1);
			sum2+=matSrc.at<uchar>(j-1,i+1);
			sum2+=2*matSrc.at<uchar>(j,i+1);
			sum2+=matSrc.at<uchar>(j+1,i+1);
			sum2-=matSrc.at<uchar>(j-1,i-1);
			sum2-=2*matSrc.at<uchar>(j,i-1);
			sum2-=matSrc.at<uchar>(j+1,i-1);
			sum2=abs(sum2);
			value=sum1+sum2;
			sum1=0;
			sum2=0;
			if(value>255)
				value=255;
			else if(value<0)
				value=0;

			matsdst.at<uchar>(j,i)=value;
		}
	index=0;




}

void unsharp(float k,Mat matIn,Mat matOut){
	
	smoothing_filt(5,matSrc,matsmooth);
	int value;
	for(int i=1;i<matDst.cols-1;i++)
		for(int j=1;j<matDst.rows-1;j++){
			
			value=matIn.at<uchar>(j,i)+k*(matsmooth.at<uchar>(j,i));
			if(value>255)
				value=255;
			else if(value<0)
				value=0;
			matOut.at<uchar>(j,i)=value;
		}

}


void product(Mat mat1,Mat mat2,Mat matOut){
	int value=0;
	int index;
	int cols=mat1.cols;
	int rows=mat1.rows;
	for(int i=0;i<cols;i++)
		for(int j=0;j<rows;j++){
			matOut.at<double>(j,i)=mat1.at<uchar>(j,i)*mat2.at<uchar>(j,i);

	}
	normalize(matOut,matPron);
	
}	







void histogram_equ(int hisarray[],int total,Mat matin,Mat matout){
	Mat figure=Mat(Size(400,300),CV_8UC3);
	double pr[256]={0.0};
	double ps[256]={0.0};
	int hisEquArray[256]={0};
	double temp;
	int s[256]={0};
	int value;
	int max=0;
	for(int i=0;i<256;i++){     //claculate probability
		pr[i]=(double)hisarray[i]/total;
		temp+=pr[i];
		s[i]=cvRound(temp*255); //(L-1)*pr
		//	cout<<s[i]<<endl;
		//cout<<hisarray[i]<<endl;
		ps[s[i]]+=pr[i];

	}
	for(int i=0;i<matin.cols;i++)
		for(int j=0;j<matin.rows;j++){
			value=(int)matin.at<uchar>(j,i);
			value=s[value];
			matout.at<uchar>(j,i)=value;
		}
	
	for(int i=0;i<matin.cols;i++)
		for(int j=0;j<matin.rows;j++){
			
			hisEquArray[(int)matout.at<uchar>(j,i)]++;

		}
	for(int i=0;i<256;i++){
		if(hisEquArray[i]>max)
			max=hisEquArray[i];
		//		cout<<hisarray[i]<<endl;
	}
	//cout<<"max "<<max<<endl;
	int intensity;
	int scale=2;
	double bwidth=(double)figure.cols/256;
	double bheight=(double)figure.rows/max;
	for(int i=0;i<256;i++){
		rectangle(figure,Point(i*bwidth,figure.rows),Point((i+1)*bwidth,figure.rows-hisEquArray[i]*bheight),  Scalar(0,255),-1,8,0);

	}
	imshow("histogram equalize graph",figure);
	waitKey(0);
	imshow("histogram equalize",matout);
	waitKey(0);




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
		rectangle(figure,Point(i*bwidth,figure.rows),Point((i+1)*bwidth,figure.rows-hisarray[i]*bheight),  Scalar(0,255),-1,8,0);

	}
	imshow("histogram graph",figure);
	waitKey(0);
	histogram_equ(hisarray,total,matSrc,matHistogramEqu);	
}
void skeleton(){
	init("Fig0343(a)(skeleton_orig).tif");
	//init("p2test.tif");
	//powerLawTrans(1);
//	histogram();
	smoothing_filt(5,matSrc,matsmooth);
	laplace();
	laplace_sharpening(matSrc,matLs);
	add_sharpening(matSrc,matDst,mataddFst);
	sobel_gradient(matSrc,matsdst);
	showImage("gradient image",matsdst);
	//productMatrix(matLs,matsdst,matPron,0);
	//normalize(matPro,matPron);
	product(matLs,matsdst,matPro);
	showImage("Product image",matPron);
	add_sharpening(matSrc,matPron,mataddSnd);
	showImage("add second",mataddSnd);
	powerLawTrans(0.58,mataddSnd,matresult);
	showImage("result",matresult);
	/*smoothing_filt(5,matsdst,matgsmooth);
	showImage("gradient smooth image",matgsmooth);
	laplace_sharpening(matgsmooth,matgsharp);
	unsharp(1,matgsmooth,matunsharp);
	showImage("unsharp",matunsharp);
	laplace_sharpening(matunsharp,matgsharp);
	powerLawTrans(0.75,matgsharp,matresult);
	laplace_sharpening(matresult,matend);*/
	//imshow("end",matresult);
	//waitKey('0');
	

}

void test(){
	init("p2test.tif");
	laplace_sharpening(matSrc,matLs);
	sobel_gradient(matLs,matsdst);
	showImage("gradient image",matsdst);
	smoothing_filt(5,matsdst,matgsmooth);
	unsharp(1,matgsmooth,matunsharp);
	powerLawTrans(0.6,matunsharp,matresult);
	imwrite("testresult.tif",matresult);

	
	





}


int main(){
	float gamma;
	//	init("Fig0343(a)(skeleton_orig).tif");
	//	cout<<"input gamma value : ";
	//	cin>>gamma;
//	init("Fig0316(2)(2nd_from_top).tif");
/*	init("Fig0343(a)(skeleton_orig).tif");
	//init("p2test.tif");
	//powerLawTrans(1);
//	histogram();
	smoothing_filt(5,matSrc,matsmooth);
	laplace();
	laplace_sharpening(matSrc,matLs);
	add_sharpening();
	sobel_gradient(matSrc,matsdst);
	showImage("gradient image",matsdst);
	smoothing_filt(5,matsdst,matgsmooth);
	//sobel_gradient(matsmooth,matgsmooth);
	showImage("gradient smooth image",matgsmooth);
	laplace_sharpening(matgsmooth,matgsharp);
	unsharp(1,matgsmooth,matunsharp);
	showImage("unsharp",matunsharp);
	//product(matLs,matgsmooth,matProduct);
	laplace_sharpening(matunsharp,matgsharp);
	powerLawTrans(0.75,matgsharp,matresult);
	laplace_sharpening(matresult,matend);
	showImage("end",matresult);*/

	//	add_sharpening();
	//	cout<<"input gamma value : ";
	//	cin>>gamma;
	//	init("Fig0343(a)(skeleton_orig).tif");
	//powerLawTrans(gamma);
	//	logTrans(5);
	skeleton();
//	test();



}
