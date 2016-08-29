#ifndef ROTATION_H
#define ROTATION_H


#include <math.h>
#include <iostream>
#include "opencv2/opencv.hpp"

class rotation{
	public:
		rotation();
		virtual ~rotation();
		static double findRotAngle(cv::Mat image);
		static double findRotAngle2(cv::Mat image);

		static double calculatePostlRating(double *sumColumns, int numberCols);
		static void rotateAroundCenter(int& out_r, int& out_c,
			const int in_r, const int in_c, const int rws, const int cls,
			const double cos_a, const double sin_a);

		static cv::Mat rotateImageCropped(cv::Mat image,  double angle, double cropfactor, int sizeOfRect);
		static cv::Mat rotateImage(cv::Mat image, double angle);

		static cv::Mat getSubImage(cv::Mat imag, cv::Point center, int size);
		static cv::Mat getSubImageMirrored(cv::Mat imag, cv::Point center, int size);

	protected:
	private:
};

#endif // ROTATION_H
