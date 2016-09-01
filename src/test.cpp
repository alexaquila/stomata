#include "test.h"
test::test(std::vector<data> *datasets, int numberOfTrainingImages, int numberOfTrainingElements){
	this->datasets = datasets;
	this->numberOfTrainingImages = numberOfTrainingImages;
	numberOfTestImages = this->getNumberOfImages() - numberOfTrainingImages;
	assert (numberOfTestImages > 0);
	///SubImages have size 64x64
	this->sizeOfRect = 64;
	this->inputFeatures = new NNinputSampleQuarter(numberOfTrainingElements, cv::Size(this->sizeOfRect, this->sizeOfRect));
	this->numberOfTrainingElements = numberOfTrainingElements;
	this->NN = new neuralNetwork(this->inputFeatures->getNetworkInputSize());
}

test::~test(){

}

void test::startTesting( ){
	cout << "Generate training data." << endl;
	generateTrainingData();

	cv::Mat trainingClasses = this->inputFeatures->getClasses();
	cv::Mat trainingData = this->inputFeatures->getTransformedSamples();
	cout << "Training neural network." << endl;
	this->NN->trainNN(trainingData, trainingClasses);
	cout << "Finished training neural network." << endl;
	testData();
}

void test::testData(){
	for(int i=0; i< this->numberOfTestImages; ++i){
		data currentData = this->datasets->at(i+this->numberOfTrainingImages);
		cout << "testing number " << i <<endl;
		cv::Mat result = positiveMatchesMirrored(currentData);
		cv::Size imageSize = currentData.imageSize();
		cv::Mat mergeImage(imageSize.height, imageSize.width, CV_8UC3);
		for(int y=0; y<imageSize.height; ++y){
			for(int x=0; x<imageSize.width; ++x){
				uchar temp;
				if(result.at<float>(y,x) >0)
					temp  = 255;
				else
					temp  = 0;
				mergeImage.at<cv::Vec3b>(y,x)[0] = temp;
				mergeImage.at<cv::Vec3b>(y,x)[1] = 0;
				mergeImage.at<cv::Vec3b>(y,x)[2] = currentData.getImage().at<uchar>(y,x);
			}
		}
		string windowName = "Image ";
		cv::namedWindow(windowName+ currentData.name);
		for(int j = 0; j < currentData.numberOfStomata(); ++j)
			cv::circle(mergeImage, currentData.getCoordinate(j), maxDistance/2, CV_RGB(0, 255,0), 1);
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
			result.at<float>(y,x) = predicted.at<float>(0,0);
			}
	}
	return result;
}
/* Idea: generate with a probability of 0.5 a positive or negative trainingsdata.
 * For that, get a random trainingspicture and sample random elments, until a match in regard of the class is found.
 */
void test::generateTrainingData(){
	for(int i=0; i< this->numberOfTrainingElements; ++i){
		bool foundSample = false;
		//negative sample: -1 , positive sample (stomata found): 1
		std::uniform_int_distribution<int> classDistribution(0,1);
		int whichClass = classDistribution(generator);
		if(whichClass == 0)
			whichClass = -1;
	//	cout << " whichClass " << whichClass << endl;
		while(!foundSample){
			cv::Mat sampleImage;
			std::uniform_int_distribution<int> pictureDistribution(0,this->numberOfTrainingImages-1);
			int dataInt = pictureDistribution(generator);
///			cout << "Choose data number " <<dataInt << endl;
            data  currentData = datasets->at(dataInt);
            //getRotatedImage throws exception when the sample is part of the wrong class
            try{
				sampleImage = getRandomRotatedImage(currentData, whichClass);
			}
			catch(int e){
				// cout << "wrong class found "<< whichClass<<endl;
				continue;
			}
			//cv::normalize(sampleImage, sampleImage, 0, 255, cv::NORM_MINMAX, CV_8UC1);
			this->inputFeatures->addSample(sampleImage, whichClass);
			foundSample =true;
		}
	}
	cout << "Finished sampling classes"<<endl;
}

cv::Mat test::getRandomRotatedImage(data currentData, int whichClass){
	double angleInRad = M_PI / 180.0 * currentData.angle;
	double cropfactor = abs(cos(angleInRad)) + abs(sin(angleInRad));
	//get the size to crop a picture of desired size after rotation.
	int sizeBefRot 	= ceil(sizeOfRect*cropfactor);
	int proofsize  = sizeBefRot/cropfactor;
	//Assert even number
	if((sizeBefRot % 2)	!= 0)
		++sizeBefRot ;
	cv::Size imageSize = currentData.imageSize();
	std::uniform_int_distribution<int> xDistribution(sizeBefRot/2,imageSize.width-sizeBefRot/2);
	int x = xDistribution(generator);
	std::uniform_int_distribution<int> yDistribution(sizeBefRot/2,imageSize.height-sizeBefRot/2);
	int y = yDistribution(generator);
	cv::Point point(x,y);
	assert (proofsize == sizeOfRect);
	cv::Mat subImag = rotation::getSubImage(currentData.getImage(), point, sizeBefRot);
	subImag = rotation::rotateImageCropped(subImag, currentData.angle, cropfactor, this->sizeOfRect);
	assert (subImag.rows == sizeOfRect && subImag.cols == sizeOfRect );
	if(getClass(currentData, point) != whichClass)
		throw 0;
	return subImag;
}

int test::getClass(data currentData, cv::Point point){
	//0 equals no stomata in close vicinity
	int currentClass = -1;
	for(int i= 0; i<currentData.numberOfStomata(); ++i){
		cv::Point difference = point - currentData.getCoordinate(i);
		double distance = difference.x*difference.x + difference.y*difference.y;
		if (distance<= maxDistance*maxDistance){
		currentClass = 1;
			break;
		}
	}
	return currentClass;
}

int test::getNumberOfImages(){
	return datasets->size();
}



/*
cv::Mat test::positiveMatches(data currentData){
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
