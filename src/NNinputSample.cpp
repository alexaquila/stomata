#include "NNinputSample.h"

NNinputSample::NNinputSample(unsigned int numberOfTrainingElements, cv::Size imageSize){
	this->imageSize = imageSize;
	//this->networkInputSize = imageSize.width *imageSize.height;
	this->trainingClasses =  cv::Mat(numberOfTrainingElements,1,CV_32FC1);

}

NNinputSample::~NNinputSample(){
	//dtor
}

cv::Mat NNinputSample::getTransformedSamples(){
	return this->trainingData;
}

cv::Mat NNinputSample::getClasses(){
	return this->trainingClasses;
}

void NNinputSample::addSample(cv::Mat image, int whichClass){

	trainingClasses.at<float>(currentImageIndex,0) = whichClass;
	uchar* img_d = image.data;
	for(int j = 0; j < this->networkInputSize;  ++j)
		trainingData.at<float>(currentImageIndex,j) =  img_d[j];
	++this->currentImageIndex;
}

cv::Mat NNinputSample::transformInput(cv::Mat image){
	return image;
}
int NNinputSample::getNetworkInputSize(){
	return this->networkInputSize;

}
