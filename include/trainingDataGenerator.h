#ifndef TRAININGDATAGENERATOR_H
#define TRAININGDATAGENERATOR_H

#include "trainingDataRandomGenerator.h"

class trainingDataGenerator: public trainingDataRandomGenerator{
	public:
		trainingDataGenerator(std::vector<data> *datasets, int numberOfTrainingImages,  int numberOfTrainingElements, int sizeOfRect);
		virtual ~trainingDataGenerator();

		virtual NNinputSample * generateTrainingData();

	protected:
		std::vector<cv::Mat> * getTrainingImages();
		//The minimum distance in pixel from the sample point to the ground-truth data, in which vicinity the sample does NOT count as negatìve sample
		int minDistance = 32;

		bool noStomataInVicinity(data currentData, cv::Point point);



	private:
};

#endif // TRAININGDATAGENERATOR_H
