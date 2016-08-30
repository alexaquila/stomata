#ifndef NNINPUTSAMPLEPCA_H
#define NNINPUTSAMPLEPCA_H

#include <NNinputSample.h>

/*
 *	Implements the Principal Component Analysis
 *
 */
class NNinputSamplePCA : public NNinputSample{
	public:
		NNinputSamplePCA(int numberOfTrainingElements, cv::Size imageSize);
		virtual ~NNinputSamplePCA();
		void addSample(cv::Mat image, int whichClass);
		cv::Mat transformInput(cv::Mat image);
		cv::Mat getTransformedSamples();
	protected:
		cv::PCA *pca;
	private:
		std::vector<cv::Mat> images;
		cv::Mat pcaData;

};

#endif // NNINPUTSAMPLEPCA_H
