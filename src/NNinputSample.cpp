#include "NNinputSample.h"

NNinputSample::NNinputSample(int numberOfTrainingElements, cv::Size imageSize){
	this->imageSize = imageSize;
	//this->networkInputSize = imageSize.width *imageSize.height;
	this->trainingClasses =  cv::Mat(numberOfTrainingElements,1,CV_32FC1);
	this->	numberOfTrainingElements = numberOfTrainingElements;

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
	cv::Mat ret;
	uchar* img_d = image.data;
	for(int j = 0; j < this->networkInputSize;  ++j)
		ret.at<float>(0,j) =  img_d[j];
	return ret;
}


cv::Mat NNinputSample::transformInputMitAusgabe(cv::Mat image){
	cv::Mat ret;
	uchar* img_d = image.data;
	for(int j = 0; j < this->networkInputSize;  ++j)
		ret.at<float>(0,j) =  img_d[j];
	return ret;
}

int NNinputSample::getNetworkInputSize(){
	return this->networkInputSize;

}
int NNinputSample::getNumberOfTrainingElements(){
	return this->numberOfTrainingElements;
}


void NNinputSample::setNumberOfTrainingElements(int numberOfTrainingElements){
	this->	numberOfTrainingElements = numberOfTrainingElements;
	this->trainingClasses =  cv::Mat(numberOfTrainingElements,1,CV_32FC1);
}
