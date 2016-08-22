#ifndef DATA_H
#define DATA_H

#include <iostream>

#include "opencv2/opencv.hpp"
#include "rotation.h"
using namespace std;

class data
{
	public:
		cv::Mat image;
		char * name;
		data(cv::Mat image, std::vector<cv::Point> coordinates, char*name);
		virtual ~data();
		double angle = 0;

		int numberOfStomata();
		cv::Point getCoordinate(int i);
		cv::Mat getImage();
		double getRotationAngle();

		void calc_best_rot_angle();
	//	void rotate();
		cv::Size imageSize();

		static void applyBlur(cv::Mat image);
		static cv::Mat rotateImage(cv::Mat image, double alpha);

	protected:
	private:
		std::vector<cv::Point> coordinates;

};

#endif // DATA_H
