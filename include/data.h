#ifndef DATA_H
#define DATA_H

#include <iostream>

#include "opencv2/opencv.hpp"
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

		static void  rotateAroundCenter(int& out_r, int& out_c,
			const int in_r, const int in_c, const int rws, const int cls,
			const double cos_a, const double sin_a);
		static double calculatePostlRating(double *sumColumns, int numberCols);
		static double findRotAngle(cv::Mat image);
		static void applyBlur(cv::Mat image);
		static cv::Mat rotateImage(cv::Mat image, double alpha);

	protected:
	private:
		std::vector<cv::Point> coordinates;

};

#endif // DATA_H
