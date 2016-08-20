#include "neuralNetwork.h"
void neuralNetwork::setNNparams(){
	layerSizes.push_back(inputSize);
	layerSizes.push_back(10);
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
	for(unsigned int i = 0; i<numberOfLayers(); ++i){
		std::cout << layerSizes[i] << std::endl;
		layers.row(i) = cv::Scalar(layerSizes[i]);
	}
	this->mlp.create(layers);

}
neuralNetwork::neuralNetwork(){
	setNNparams();

}

neuralNetwork::~neuralNetwork()
{
	//dtor
}
void neuralNetwork::trainNN(cv::Mat& trainingData, cv::Mat& trainingClasses){
	this->mlp.train(trainingData, trainingClasses, cv::Mat(), cv::Mat(), params);
}
int neuralNetwork::numberOfLayers(){
	return 	layerSizes.size();
}
