#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <random>
#include <assert.h>
#include "data.h"
#include "neuralNetwork.h"


class test{
	public:
		int numberOfTestImages;
		int numberOfTrainingImages= 10;
		test(std::vector<data> *datasets, int numberOfTrainingImages);
		virtual ~test();

		void startTesting(int numberOfTrainingElements);
		int getNumberOfImages();
		cv::Mat generateTrainingData(int numberOfTrainingElements, cv::Mat& testClass);

	protected:
	private:
		int networkInputSize = 256;
		int sizeOfRect;
		std::default_random_engine generator;
		neuralNetwork NN;
		std::vector<data> *datasets;
		//THe maximal distance in pixel from the sample point to the ground-truth data, in which vicinity the sample count as stomata-class
		int maxDistance = 20;
		cv::Mat getSubImage(cv::Mat imag, cv::Point center, int size);
		cv::Mat rotateImageCropped(cv::Mat image,  double angle, double cropfactor);
		cv::Mat rotateImage(cv::Mat image, double angle);
		cv::Mat getRotatedImage(data currentData, int whichClass);

		cv::Mat positiveMatches(data currentData);

		int getClass(data currentData, cv::Point point);
		void testTestData();
};
#endif // TEST_H
