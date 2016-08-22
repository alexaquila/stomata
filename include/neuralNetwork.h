#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <iostream>
#include "opencv2/opencv.hpp"

class neuralNetwork
{
	public:
		neuralNetwork();
		virtual ~neuralNetwork();
		void trainNN(cv::Mat& trainingData, cv::Mat& trainingClasses);
		cv::Mat predictNN(cv::Mat sample);
	protected:
	private:
		void setNNparams();
		int numberOfLayers();
		std::vector<int> layerSizes;
		int inputSize = 256;
		CvANN_MLP mlp;
		CvANN_MLP_TrainParams params;

};

#endif // NEURALNETWORK_H
