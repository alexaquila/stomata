#ifndef NNINPUTSAMPLEFISHER_H
#define NNINPUTSAMPLEFISHER_H

#include <NNinputSample.h>

/*
 *	Fisher linear discriminant
 */
class NNinputSampleFisher : public NNinputSample{
	public:
		NNinputSampleFisher(int numberOfTrainingElements, cv::Size imageSize);
		virtual ~NNinputSampleFisher();
		void addSample(cv::Mat image, int whichClass);
		cv::Mat transformInput(cv::Mat image);
		cv::Mat transformInputMitAusgabe(cv::Mat image);
		cv::Mat getTransformedSamples();
	protected:
		cv::LDA *lda;
	private:
		std::vector<cv::Mat> images;
		cv::Mat ldaData;
};

#endif // NNinputSampleFisher_H
