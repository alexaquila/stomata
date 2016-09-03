#include "test.h"
test::test(std::vector<data> *datasets, int numberOfTrainingImages, int numberOfTrainingElements){
	this->datasets = datasets;
	this->numberOfTrainingImages = numberOfTrainingImages;
	numberOfTestImages = this->getNumberOfImages() - numberOfTrainingImages;
	assert (numberOfTestImages > 0);
	///SubImages have size 64x64
	this->sizeOfRect = 64;
	this->trainGen = new trainingDataGenerator(datasets, numberOfTrainingImages, numberOfTrainingElements, sizeOfRect);
	this->numberOfTrainingElements = numberOfTrainingElements;
}

test::~test(){
	delete trainGen;
	delete NN;
	delete datasets;
}

void test::startTesting( ){
	cout << "Generate training data." << endl;
	this->inputFeatures = this->trainGen->generateTrainingData();
	cv::Mat trainingClasses = this->inputFeatures->getClasses();
	cv::Mat trainingData = this->inputFeatures->getTransformedSamples();
	cout << "Training neural network." << endl;
	this->NN = new neuralNetwork(this->inputFeatures->getNetworkInputSize());
	this->NN->trainNN(trainingData, trainingClasses);
	cout << "Finished training neural network." << endl;
	testData();
}

void test::testData(){
	for(int i=0; i< this->numberOfTestImages; ++i){
		data currentData = this->datasets->at(i+this->numberOfTrainingImages);
		cv::Size imageSize = currentData.imageSize();
		cout << "testing number " << i <<endl;
		cv::Mat result = positiveMatchesMirrored(currentData);
		cv::Mat mergeImage(imageSize.height, imageSize.width, CV_8UC3);
		for(int y=0; y<imageSize.height; ++y){
			for(int x=0; x<imageSize.width; ++x){
				uchar temp;
				if(result.at<float>(y,x) >0)
					temp  = 255;
				else
					temp  = 0;
				uchar temp2;
				mergeImage.at<cv::Vec3b>(y,x)[0] = temp;
				mergeImage.at<cv::Vec3b>(y,x)[1] = 0;
				mergeImage.at<cv::Vec3b>(y,x)[2] = currentData.getImage().at<uchar>(y,x);
			}
		}
		string windowName = "Image ";
		cv::namedWindow(windowName+ currentData.name);
		for(int j = 0; j < currentData.numberOfStomata(); ++j)
			cv::circle(mergeImage, currentData.getCoordinate(j), 10 , CV_RGB(0, 255,0), 1);
		cv::imshow(windowName + currentData.name, mergeImage);
		cv::waitKey(-1);
		cv::destroyWindow(windowName + currentData.name);
	}
}

//Tests all the pixels, mirrors partially the original picture, if parts of the subImage (which is taken as sample) lie outside the original picture
cv::Mat test::positiveMatchesMirrored(data currentData){

	cv::Size imageSize = currentData.imageSize();
    cv::Mat result(imageSize.height, imageSize.width, CV_32F);
	double angleInRad = M_PI / 180.0 * currentData.angle;
	double cropfactor = abs(cos(angleInRad)) + abs(sin(angleInRad));
	//get the size to crop a picture of desired size after rotation.
	int sizeBefRot 	= ceil(sizeOfRect*cropfactor);
	//Assert even number
	if((sizeBefRot % 2)	!= 0)
		++sizeBefRot ;
	for(int y=0; y<imageSize.height; ++y){
		cout << "pixelstuff " << y << endl;
		for(int x=0; x<imageSize.width; ++x){
			cv::Point point(x,y);
			cv::Mat sampleImage = rotation::getSubImageMirrored(currentData.getImage(), point, sizeBefRot);
			sampleImage = rotation::rotateImageCropped(sampleImage, currentData.angle, cropfactor, this->sizeOfRect);
			cv::Mat sample = this->inputFeatures->transformInput(sampleImage);
			//cv::Mat sample = this->inputFeatures->transformInputMitAusgabe(sampleImage);
			//cout << "sample  " << sample.cols << " , " <<sample.rows  << endl;
			cv::Mat predicted = NN->predictNN(sample);
			//cout << "predicted.at<float>(0,0) " << predicted.at<float>(0,0) << endl;
			result.at<float>(y,x) = predicted.at<float>(0,0);
			}
	}
	return result;
}

int test::getNumberOfImages(){
	return datasets->size();
}
/*
cv::Mat trainingDataGenerator::positiveMatches(data currentData){
	cv::Size imageSize = currentData.imageSize();
    cv::Mat result(imageSize.height, imageSize.width, CV_32F);
	for(int y=0; y<imageSize.height; ++y){
		for(int x=0; x<imageSize.width; ++x){
			result.at<float>(y,x) =0;
		}
	}
	double angleInRad = M_PI / 180.0 * currentData.angle;
	double cropfactor = abs(cos(angleInRad)) + abs(sin(angleInRad));
	//get the size to crop a picture of desired size after rotation.
	int sizeBefRot 	= ceil(sizeOfRect*cropfactor);
	//Assert even number
	if((sizeBefRot % 2)	!= 0)
		++sizeBefRot ;
	//cout << "cropfactor " << cropfactor <<endl;
	for(int y=sizeBefRot/2; y<imageSize.height-sizeBefRot/2; ++y){
		//cout << "Point y " << y <<endl;
		for(int x=sizeBefRot/2; x<imageSize.width-sizeBefRot/2; ++x){
			cv::Point point(x,y);
			cv::Mat sampleImage = rotation::getSubImage(currentData.getImage(), point, sizeBefRot);
			sampleImage = rotation::rotateImageCropped(sampleImage, currentData.angle, cropfactor, this->sizeOfRect);
			cv::resize(sampleImage, sampleImage, cv::Size(this->sizeOfRect/4, this->sizeOfRect/4));
			cv::Mat sample(1, this->networkInputSize, CV_32FC1);
			uchar* img_d = sampleImage.data;
			for(int j = 0; j < this->networkInputSize;  ++j){
				sample.at<float>(0,j) =  img_d[j];
			}
			cv::Mat predicted = NN.predictNN(sample);
			result.at<float>(y,x) = predicted.at<float>(0,0);
			}
	}
	return result;
}
*/
