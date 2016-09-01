#include "neuralNetwork.h"
void neuralNetwork::setNNparams(int inputSize){
	this->inputSize = inputSize;
	layerSizes.push_back(inputSize);
	layerSizes.push_back(255);
	layerSizes.push_back(128);
	layerSizes.push_back(1);

	//std::cout << "number of layers " << layerSizes.size() <<std::endl;

	CvTermCriteria criteria;
    criteria.max_iter = 100;
    criteria.epsilon = 0.00001f;
    criteria.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;

    this->params.train_method = CvANN_MLP_TrainParams::BACKPROP;
    this->params.bp_dw_scale = 0.1f;
    this->params.bp_moment_scale = 0.1f;
    this->params.term_crit = criteria;

	cv::Mat layers = cv::Mat(numberOfLayers(), 1, CV_32SC1);
	for(int i = 0; i<numberOfLayers(); ++i){
		std::cout << layerSizes[i] << std::endl;
		layers.row(i) = cv::Scalar(layerSizes[i]);
	}
	this->mlp.create(layers);

}
neuralNetwork::neuralNetwork(int inputSize){
	setNNparams(inputSize);

}

neuralNetwork::~neuralNetwork()
{
	//dtor
}
void neuralNetwork::trainNN(cv::Mat& trainingData, cv::Mat& trainingClasses){
	this->mlp.train(trainingData, trainingClasses, cv::Mat(), cv::Mat(), params);
}

cv::Mat neuralNetwork::predictNN(cv::Mat sample){
	cv::Mat result(1, 1, CV_32FC1);

	this->mlp.predict(sample, result);
	return result;
}

int neuralNetwork::numberOfLayers(){
	return 	layerSizes.size();
}
