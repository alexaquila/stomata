#include "trainingDataRandomGenerator.h"

trainingDataRandomGenerator::trainingDataRandomGenerator(std::vector<data> *datasets, int numberOfTrainingImages,  int numberOfTrainingElements, int sizeOfRect){
	this->datasets = datasets;
	this->numberOfTrainingElements = numberOfTrainingElements;
	this->numberOfTrainingImages = numberOfTrainingImages;
	this->inputFeatures = inputFeatures;
}

trainingDataRandomGenerator::~trainingDataRandomGenerator()
{
	//dtor
}
/* Idea: generate with a probability of 0.5 a positive or negative trainingsdata.
 * For that, get a random trainingspicture and sample random elments, until a match in regard of the class is found.
 */
NNinputSample * trainingDataRandomGenerator::generateTrainingData(){
	this->inputFeatures = new NNinputSampleQuarter(this->numberOfTrainingElements, cv::Size(this->sizeOfRect, this->sizeOfRect));

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
				sampleImage = getRotatedImage(currentData, whichClass);
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
	return inputFeatures;

}

cv::Mat trainingDataRandomGenerator::getRotatedImage(data currentData, int whichClass){
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

int trainingDataRandomGenerator::getClass(data currentData, cv::Point point){
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



