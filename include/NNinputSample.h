#ifndef NNINPUTSAMPLE_H
#define NNINPUTSAMPLE_H
#include "opencv2/opencv.hpp"

/*  Generates input samples from an image for the neural network by dimensionally reduction (a 64*64 oucture has 4096 feature, hence lacking of samples is guaranteed).
 *  First: Principle Component Analysis
 *  Secon: Fisher linear dicriminant
 */
class NNinputSample{
	public:
		NNinputSample(int numberOfTrainingElements, cv::Size imageSize);
		virtual ~NNinputSample();
		virtual cv::Mat  getTransformedSamples();
		cv::Mat  getClasses();
		virtual void addSample(cv::Mat image, int whichClass);

		virtual cv::Mat transformInput(cv::Mat image);

		int getNetworkInputSize();
	protected:
		cv::Mat trainingData;
		cv::Mat trainingClasses;
		cv::Size imageSize;
		int networkInputSize;
		int currentImageIndex = 0;
		int getNumberOfTrainingElements();
			int numberOfTrainingElements;
	private:

};

#endif // NNINPUTSAMPLE_H
