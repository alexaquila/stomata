#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <random>
#include <assert.h>
#include "data.h"

#include "trainingDataGenerator.h"
#include "trainingDataRandomGenerator.h"

#include "neuralNetwork.h"
#include "NNinputSample.h"
#include "NNinputSampleQuarter.h"
#include "NNinputSamplePCA.h"

#include "opencv2/opencv.hpp"

class test{
	public:
		int numberOfTestImages;
		int numberOfTrainingImages;
		test(std::vector<data> *datasets, int numberOfTrainingImages, int  numberOfTrainingElements);
		virtual ~test();

		void startTesting();
		int getNumberOfImages();



	protected:
	private:
		cv::Mat  getComponents(cv::Mat matrix);
		int sizeOfRect;

		neuralNetwork* NN;
		std::vector<data> *datasets;

		cv::Mat positiveMatches(data currentData);
		cv::Mat positiveMatchesMirrored(data currentData);
		void testData();
		NNinputSample * inputFeatures;

		int numberOfTrainingElements;


		int currentTrainingImage = 0;
		int currentTrainingStomata =0;

		trainingDataRandomGenerator * trainGen;
};
#endif // TEST_H
