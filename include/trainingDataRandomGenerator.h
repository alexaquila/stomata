#ifndef TRAININGDATARANDOMGENERATOR_H
#define TRAININGDATARANDOMGENERATOR_H
#include "rotation.h"
#include "data.h"
#include "NNinputSample.h"
#include "NNinputSampleQuarter.h"
#include "NNinputSamplePCA.h"
#include "opencv2/opencv.hpp"

class trainingDataRandomGenerator{
	public:
		trainingDataRandomGenerator(std::vector<data> *datasets,int numberOfTrainingImages, int numberOfTrainingElements, int sizeOfRect);
		virtual ~trainingDataRandomGenerator();

		virtual NNinputSample * generateTrainingData();
		int numberOfTrainingElements;

	protected:

		int numberOfTrainingImages;
		int sizeOfRect = 64;
		int getClass(data currentData, cv::Point point);

		//The maximum distance in pixel from the sample point to the ground-truth data, in which vicinity the sample count as stomata-class
		int maxDistance = 7;
		std::default_random_engine generator;
		std::vector<data> *datasets;
		NNinputSample * inputFeatures;


	private:
		cv::Mat getRotatedImage(data currentData, int whichClass);


};

#endif // TRAININGDATAGENERATOR_H
