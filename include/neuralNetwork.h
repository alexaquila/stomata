#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <iostream>
#include "opencv2/opencv.hpp"

class neuralNetwork
{
	public:
		neuralNetwork();
		virtual ~neuralNetwork();
	protected:
	private:
		void setNNparams();
		int numberOfLayers();
		std::vector<int> layerSizes;
		int inputSize = 256;
};

#endif // NEURALNETWORK_H
