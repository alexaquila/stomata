#ifndef ROTATION_H
#define ROTATION_H

#include "opencv2/opencv.hpp"
#include <math.h>
#include <iostream>     // std::cout
//#include <algorithm>    // std::min
class rotation
{
	public:
		rotation();
		virtual ~rotation();
		static double findRotAngle(cv::Mat image);
				static double findRotAngle2(cv::Mat image);

		static double calculatePostlRating(double *sumColumns, int numberCols);
		static void rotateAroundCenter(int& out_r, int& out_c,
			const int in_r, const int in_c, const int rws, const int cls,
			const double cos_a, const double sin_a);

	protected:
	private:
};

#endif // ROTATION_H
