#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <random>
#include <assert.h>
#include "data.h"
#include "neuralNetwork.h"


class test
{
	public:
		int numberOfTestImages;
		int numberOfTrainingImages= 10;
		test(std::vector<data> *datasets, int numberOfTrainingImages);
		virtual ~test();

		void startTesting();
		int getNumberOfImages();
		cv::Mat generateTrainingData(int numberOfTrainingElements);

	protected:
	private:
		int sizeOfRect;
		std::default_random_engine generator;
		neuralNetwork NN;
		std::vector<data> *datasets;
		int distance = 10;
		cv::Mat getSubImage(cv::Mat imag, cv::Point center, int size);
		cv::Mat rotateImage(cv::Mat image,  double angle);
		cv::Mat getRotatedImage(data currentData, int whichClass);
};

#endif // TEST_H
