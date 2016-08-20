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
	cv::Mat trainingData = generateTrainingData(1000);




}
/* Idea: generate with a probability of 0.5 a positive or negative trainingsdata.
 * For that, get a random trainingspicture and sample random elments, until a match in regard of the class is found.
 */
cv::Mat test::generateTrainingData(int numberOfTrainingElements){
	for(int i=0; i< numberOfTrainingElements; ++i){
		std::uniform_int_distribution<int> classDistribution(0,1);
		bool foundSample = false;
		int whichClass = classDistribution(generator);
		while(!foundSample){
			std::uniform_int_distribution<int> pictureDistribution(0,numberOfTrainingElements-1);


			if( whichClass== 0){


			}
			else{

			}
		}
	}





}


int test::getNumberOfElements(){
	return datasets->size();
}
