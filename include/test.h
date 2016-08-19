#ifndef TEST_H
#define TEST_H

#include <iostream>
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
	protected:
	private:
		neuralNetwork NN;
		std::vector<data> *datasets;
};

#endif // TEST_H
