#ifndef NNINPUTSAMPLEPCA_H
#define NNINPUTSAMPLEPCA_H

#include <NNinputSample.h>


class NNinputSamplePCA : public NNinputSample{
	public:
		NNinputSamplePCA(unsigned int numberOfTrainingElements, cv::Size imageSize);
		virtual ~NNinputSamplePCA();
	protected:
	private:
};

#endif // NNINPUTSAMPLEPCA_H
