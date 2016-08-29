#include "NNinputSample.h"

NNinputSample::NNinputSample(cv::Mat image){
	this->image = image;
}

NNinputSample::~NNinputSample()
{
	//dtor
}
