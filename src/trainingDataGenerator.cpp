#include "trainingDataGenerator.h"

trainingDataGenerator::trainingDataGenerator(std::vector<data> *datasets, int numberOfTrainingImages
	, int numberOfTrainingElements, int sizeOfRect)
	:trainingDataRandomGenerator(datasets, numberOfTrainingImages, numberOfTrainingElements,sizeOfRect){
	this->numberOfTrainingElements =0;
}

trainingDataGenerator::~trainingDataGenerator(){
	//dtor
}

NNinputSample * trainingDataGenerator::generateTrainingData(){
	std::vector<cv::Mat> *trainingImages = getTrainingImages();
	std::cout << "Number of training Images " << trainingImages->size() << std::endl;
	this->inputFeatures = new NNinputSampleQuarter(this->numberOfTrainingElements, cv::Size(this->sizeOfRect, this->sizeOfRect));
	for(int i = 0; i<trainingImages->size() ;i=i+2){
        this->inputFeatures->addSample(trainingImages->at(i), 1);
        this->inputFeatures->addSample(trainingImages->at(i+1), -1);
	}
	delete trainingImages;
	return this->inputFeatures;
}

std::vector<cv::Mat> *trainingDataGenerator::getTrainingImages(){
	std::vector<cv::Mat> * trainingImages= new std::vector<cv::Mat>();
	for(int i=0; i< this->numberOfTrainingImages; ++i){
		//get all possible input-images for current image
		data  currentData = datasets->at(i);
		double angleInRad = M_PI / 180.0 * currentData.angle;
		double cropfactor = abs(cos(angleInRad)) + abs(sin(angleInRad));
		int sizeBefRot 	= ceil(sizeOfRect*cropfactor);
		//Assert even number
		if((sizeBefRot % 2)	!= 0)
			++sizeBefRot ;
		cv::Size imageSize = currentData.imageSize();
		for(int coordInd=0;  coordInd < currentData.numberOfStomata(); ++coordInd){
            cv::Point currentStomata = currentData.getCoordinate(coordInd);
			if(currentStomata.x >=sizeBefRot/2 &&  currentStomata.x <= imageSize.width-sizeBefRot/2){
				if(currentStomata.y >=sizeBefRot/2 &&  currentStomata.y <= imageSize.height-sizeBefRot/2){
					//Firstly: find a positive match
					cv::Mat subImag = rotation::getSubImage(currentData.getImage(), currentStomata, sizeBefRot);
//	cv::namedWindow("1");
//	cv::imshow("1", subImag);
					subImag = rotation::rotateImageCropped(subImag, currentData.angle, cropfactor, this->sizeOfRect);

	//cv::namedWindow("2");
//	cv::imshow("2", subImag);

//	cv::waitKey(-1);

					assert (subImag.rows == sizeOfRect && subImag.cols == sizeOfRect );

					cv::normalize(subImag, subImag, 0, 255, cv::NORM_MINMAX, CV_32FC1);


					trainingImages->push_back(subImag);
					++this->numberOfTrainingElements;
					//Then, find a negative sample, which has at least a distance of this->minDistance to all ground-truth-coordinates
                    bool foundNegativeSample=false;
                    cv::Point point;
                    while(!foundNegativeSample){
						std::uniform_int_distribution<int> xDistribution(sizeBefRot/2,imageSize.width-sizeBefRot/2);
						int x = xDistribution(generator);
						std::uniform_int_distribution<int> yDistribution(sizeBefRot/2,imageSize.height-sizeBefRot/2);
						int y = yDistribution(generator);
						point = cv::Point(x,y);
						subImag = rotation::getSubImage(currentData.getImage(), point, sizeBefRot);
						subImag = rotation::rotateImageCropped(subImag, currentData.angle, cropfactor, this->sizeOfRect);
						foundNegativeSample = noStomataInVicinity(currentData, point);
                    }
					cv::normalize(subImag, subImag, 0, 255, cv::NORM_MINMAX, CV_32FC1);
					trainingImages->push_back(subImag);
					++this->numberOfTrainingElements;
				}
			}
		}
	}
	return trainingImages;
}


