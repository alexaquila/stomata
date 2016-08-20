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
		int numberOfTestElements;
		int numberOfTrainingElements = 10;
		test(std::vector<data> *datasets, int numberOfTrainingElements);
		virtual ~test();

		void startTesting();
		int getNumberOfElements();
		cv::Mat generateTrainingData(int numberOfTrainingElements);

	protected:
	private:
		std::default_random_engine generator;
		neuralNetwork NN;
		std::vector<data> *datasets;
};

#endif // TEST_H
