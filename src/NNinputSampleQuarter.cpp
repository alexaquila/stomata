#include "NNinputSampleQuarter.h"

NNinputSampleQuarter::NNinputSampleQuarter(int numberOfTrainingElements, cv::Size  imageSize):NNinputSample(numberOfTrainingElements, imageSize){
	this->networkInputSize = (imageSize.width * imageSize.height)/16;
	this->trainingData = cv::Mat(numberOfTrainingElements, this->networkInputSize, CV_32FC1);


}

NNinputSampleQuarter::~NNinputSampleQuarter(){

}



void NNinputSampleQuarter::addSample(cv::Mat image, int whichClass){
	trainingClasses.at<float>(currentImageIndex,0) = whichClass;
	//trainingData.row(this->currentImageIndex)= this->transformInput(image);
	cv::Mat sample = this->transformInput(image);
	for(int j = 0; j < this->getNetworkInputSize();  ++j){
		trainingData.at<float>(this->currentImageIndex,j) =  sample.at<float>(0,j);
	}

/*		std::string windowName = "Image ";
		cv::namedWindow(windowName);

		cv::imshow(windowName, image);
		cv::waitKey(-1);
		cv::imshow(windowName, alteredImage);
		cv::waitKey(-1);
		cv::destroyWindow(windowName);
*/
//	uchar* img_d = alteredImage.data;
//	for(int j = 0; j < this->networkInputSize;  ++j)
//		trainingData.at<float>(currentImageIndex,j) =  img_d[j];

	++this->currentImageIndex;
}
cv::Mat NNinputSampleQuarter::transformInput(cv::Mat image){
	cv::Mat tempImage;
	cv::resize(image, tempImage, cv::Size(this->imageSize.width/4, this->imageSize.height/4));
	cv::Mat sample(1, this->getNetworkInputSize(), CV_32FC1);
	uchar* img_d = tempImage.data;
	for(int j = 0; j < this->getNetworkInputSize();  ++j){
		sample.at<float>(0,j) =  img_d[j];
	}
//	std::cout << "asdasd" << std::endl;
	return sample;
}
