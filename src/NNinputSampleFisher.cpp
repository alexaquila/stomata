#include "NNinputSampleFisher.h"

NNinputSampleFisher::NNinputSampleFisher(int numberOfTrainingElements, cv::Size  imageSize):NNinputSample(numberOfTrainingElements, imageSize){
	this->reduceFactor = 4;
	this->ldaData = cv::Mat(numberOfTrainingElements, imageSize.width * imageSize.height /(this->reduceFactor*this->reduceFactor), CV_32FC1);

	this->networkInputSize = 128;
}

NNinputSampleFisher::~NNinputSampleFisher()//
{
	//dtor
}

void NNinputSampleFisher::addSample(cv::Mat image, int whichClass){
	trainingClasses.at<float>(currentImageIndex,0) = whichClass;
	cv::Mat tempImage;
	this->images.push_back(image);
	cv::resize(image, tempImage, cv::Size(this->imageSize.width/this->reduceFactor, this->imageSize.height/this->reduceFactor));
//	if(currentImageIndex%2 ==0)
		tempImage.clone().reshape(1, 1).convertTo(this->ldaData.row(currentImageIndex), CV_32FC1);
	++this->currentImageIndex;
}

cv::Mat NNinputSampleFisher::getTransformedSamples(){
	std::cout << "Perform Fisher. " << std::endl;

//	cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();
  //  model->train(this->pcaData, this->trainingClasses);

	//this->pca = new cv::PCA(this->pcaData, cv::Mat(), CV_PCA_DATA_AS_ROW, this->networkInputSize);

	this->lda = new cv::LDA(this->ldaData, this->trainingClasses, this->networkInputSize);


    this->trainingData = cv::Mat(this->getNumberOfTrainingElements(), this->networkInputSize, CV_32FC1);

	for( int i = 0; i < this->getNumberOfTrainingElements(); ++i)
		this->trainingData.row(i) = this->transformInput(images[i]);
	return this->trainingData;
}

cv::Mat NNinputSampleFisher::transformInput(cv::Mat image){
	cv::Mat tempImage;
	cv::resize(image, tempImage, cv::Size(this->imageSize.width/this->reduceFactor, this->imageSize.height/this->reduceFactor));
	tempImage.clone().reshape(1, 1).convertTo(tempImage, CV_32FC1);
	cv::Mat retData; // = cv::Mat(1, this->networkInputSize, CV_32FC1);
	retData = this->lda->project(tempImage);
	//std::cout << " retData " <<  retData.cols << " , " << retData.rows<< std::endl;
	return retData;
}

cv::Mat NNinputSampleFisher::transformInputMitAusgabe(cv::Mat image){

	cv::Mat retData = cv::Mat(1, this->networkInputSize, CV_32FC1);

	return retData;
}

/*
	cv::Mat mean = pca.mean.clone();
    cv::Mat eigenvalues = pca.eigenvalues.clone();
    cv::Mat eigenvectors = pca.eigenvectors.clone();
	std::cout << "Mean " << mean.cols << " , " << mean.rows<< std::endl;

	std::cout << " images[0] " <<  images[0].cols << " , " <<  images[0].rows<< std::endl;

	cv::Mat imag = mean.reshape(1, images[0].rows);

	std::cout << " imag " <<  imag.cols << " , " << imag.rows<< std::endl;

	cv::normalize(imag, imag, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	imshow("avg", imag);

	imag= pca.eigenvectors.row(0);
	cv::normalize(imag, imag, 0, 255, cv::NORM_MINMAX, CV_8UC1);

	imshow("pc1", imag.reshape(1, images[0].rows));

    cv::waitKey(0);
*/
