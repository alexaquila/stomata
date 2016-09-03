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
		cv::PCA *pca;
	private:
		std::vector<cv::Mat> images;
		cv::Mat pcaData;

};

#endif // NNinputSampleFisher_H
