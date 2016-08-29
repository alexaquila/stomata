#ifndef NNINPUTSAMPLE_H
#define NNINPUTSAMPLE_H
#include "opencv2/opencv.hpp"

/*  Generates input samples from an image for the neural network by dimensionally reduction (a 64*64 oucture has 4096 feature, hence lacking of samples is guaranteed).
 *  First: Principle Component Analysis
 *  Secon: Fisher linear dicriminant
 */
class NNinputSample
{
	public:
		NNinputSample(cv::Mat image);
		virtual ~NNinputSample();
	protected:
	private:
		cv::Mat image;
};

#endif // NNINPUTSAMPLE_H
