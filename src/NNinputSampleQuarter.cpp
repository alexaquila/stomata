#include "NNinputSampleQuarter.h"

NNinputSampleQuarter::NNinputSampleQuarter(unsigned int numberOfTrainingElements, cv::Size  imageSize):NNinputSample(numberOfTrainingElements, imageSize){
	this->networkInputSize = (imageSize.width * imageSize.height)/16;
	this->trainingData = cv::Mat(numberOfTrainingElements, this->networkInputSize, CV_32FC1);


}

NNinputSampleQuarter::~NNinputSampleQuarter(){

}



void NNinputSampleQuarter::addSample(cv::Mat image, int whichClass){
	trainingClasses.at<float>(currentImageIndex,0) = whichClass;
	cv::Mat alteredImage = this->transformInput(image);
/*		std::string windowName = "Image ";
		cv::namedWindow(windowName);

		cv::imshow(windowName, image);
		cv::waitKey(-1);
		cv::imshow(windowName, alteredImage);
		cv::waitKey(-1);
		cv::destroyWindow(windowName);
*/
	uchar* img_d = alteredImage.data;
	for(int j = 0; j < this->networkInputSize;  ++j)
		trainingData.at<float>(currentImageIndex,j) =  img_d[j];

	++this->currentImageIndex;
}
cv::Mat NNinputSampleQuarter::transformInput(cv::Mat image){
	cv::Mat tempImage;
	cv::resize(image, tempImage, cv::Size(this->imageSize.width/4, this->imageSize.height/4));
		return tempImage;
}
