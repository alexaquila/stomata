#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <random>
#include <assert.h>
#include "data.h"
#include "rotation.h"
#include "neuralNetwork.h"
#include "NNinputSample.h"
#include "NNinputSampleQuarter.h"
class test{
	public:
		int numberOfTestImages;
		int numberOfTrainingImages= 10;
		test(std::vector<data> *datasets, int numberOfTrainingImagesint, int  numberOfTrainingElements);
		virtual ~test();

		void startTesting();
		int getNumberOfImages();
		void generateTrainingData();

	protected:
	private:
		//int networkInputSize = 256;
		int sizeOfRect;
		std::default_random_engine generator;
		neuralNetwork* NN;
		std::vector<data> *datasets;
		//The maximal distance in pixel from the sample point to the ground-truth data, in which vicinity the sample count as stomata-class
		int maxDistance = 5;

		cv::Mat getRotatedImage(data currentData, int whichClass);

		cv::Mat positiveMatches(data currentData);
		cv::Mat positiveMatchesMirrored(data currentData);
		int getClass(data currentData, cv::Point point);
		void testData();
		NNinputSample * inputFeatures;


		int numberOfTrainingElements;
};
#endif // TEST_H
