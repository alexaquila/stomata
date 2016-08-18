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
		double rot_angle = 0;
		data(cv::Mat image, std::vector<cv::Point> coordinates, char*name);
		virtual ~data();

		int numberOfStomata();
		cv::Point getCoordinate(int i);
		cv::Mat getImage();
		void rotateImage(double alpha);
		void calc_best_rot_angle();
		void rotate();

		static void  rotateAroundCenter(int& out_r, int& out_c,
			const int in_r, const int in_c, const int rws, const int cls,
			const double cos_a, const double sin_a);
		static double calculatePostlRating(double *sumColumns, int numberCols);
		static double findRotAngle(cv::Mat image);
		static void applyBlur(cv::Mat image);
	protected:
	private:
		std::vector<cv::Point> coordinates;
};

#endif // DATA_H
