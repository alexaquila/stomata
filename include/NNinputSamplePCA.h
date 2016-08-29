#ifndef NNINPUTSAMPLEPCA_H
#define NNINPUTSAMPLEPCA_H

#include <NNinputSample.h>

/*
 *	Implements the Principal Component Analysis
 *
 */
class NNinputSamplePCA : public NNinputSample{
	public:
		NNinputSamplePCA(unsigned int numberOfTrainingElements, cv::Size imageSize);
		virtual ~NNinputSamplePCA();
		void addSample(cv::Mat image, int whichClass);
		cv::Mat transformInput(cv::Mat image);
	protected:
	private:
};

#endif // NNINPUTSAMPLEPCA_H
