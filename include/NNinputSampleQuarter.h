#ifndef NNINPUTSAMPLEQUARTER_H
#define NNINPUTSAMPLEQUARTER_H

#include <NNinputSample.h>



class NNinputSampleQuarter : public NNinputSample
{
	public:
		NNinputSampleQuarter(unsigned int numberOfTrainingElements, cv::Size  imageSize);
		virtual ~NNinputSampleQuarter();
		void addSample(cv::Mat image, int whichClass);
		cv::Mat transformInput(cv::Mat image);
	protected:
	private:
};

#endif // NNINPUTSAMPLEQUARTER_H
