#include "test.h"

test::test(std::vector<data> *datasets, int numberOfTrainingElements){

	this->datasets = datasets;
	this->numberOfTrainingElements = numberOfTrainingElements;
	numberOfTestElements = this->getNumberOfElements() - numberOfTrainingElements;
	assert (numberOfTestElements > 0);
}

test::~test(){

}

void test::startTesting(){





}
int test::getNumberOfElements(){
	return datasets->size();
}
