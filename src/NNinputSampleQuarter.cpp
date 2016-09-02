#include "NNinputSampleQuarter.h"

NNinputSampleQuarter::NNinputSampleQuarter(int numberOfTrainingElements, cv::Size  imageSize):NNinputSample(numberOfTrainingElements, imageSize){
	this->reduceFactor = 4;
	this->networkInputSize = (imageSize.width * imageSize.height)/(this->reduceFactor*this->reduceFactor);
	this->trainingData = cv::Mat(numberOfTrainingElements, this->networkInputSize, CV_32FC1);
}

NNinputSampleQuarter::~NNinputSampleQuarter(){

}

void NNinputSampleQuarter::addSample(cv::Mat image, int whichClass){
	trainingClasses.at<float>(currentImageIndex,0) = whichClass;
	//trainingData.row(this->currentImageIndex)= this->transformInput(image);
	cv::Mat sample = this->transformInput(image);
	for(int j = 0; j < this->getNetworkInputSize();  ++j)
		trainingData.at<float>(this->currentImageIndex,j) =  sample.at<float>(0,j);

	++this->currentImageIndex;
}

cv::Mat NNinputSampleQuarter::transformInput(cv::Mat image){
	cv::Mat tempImage;
	cv::resize(image, tempImage, cv::Size(this->imageSize.width/this->reduceFactor, this->imageSize.height/this->reduceFactor));
//	cv::imshow("Image", tempImage);
//	cv::waitKey(-1);
	cv::Mat sample;
	tempImage.clone().reshape(1, 1).convertTo(sample, CV_32FC1);
	return sample;
}
