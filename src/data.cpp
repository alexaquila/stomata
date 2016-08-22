#include "data.h"


	int data::numberOfStomata(){
		return this->coordinates.size();
	}

	cv::Point data::getCoordinate(int i){
		//if(i >=0 && i<coordinates.size())
			return coordinates[i];
		//else
		//	return -1;
	}
	double data::getRotationAngle(){
		return this->angle;
	}
	cv::Mat data::getImage(){
		return image.clone();
	}

	cv::Mat data::rotateImage(cv::Mat image,  double alpha){
		// get rotation matrix for rotating the image around its center

		cv::Point2f center(image.cols/2.0, image.rows/2.0);
		cv::Mat rot = cv::getRotationMatrix2D(center, alpha, 1.0);
		// determine bounding rectangle
		cv::Rect bbox = cv::RotatedRect(center,image.size(), alpha).boundingRect();
		// adjust transformation matrix
		rot.at<double>(0,2) += bbox.width/2.0 - center.x;
		rot.at<double>(1,2) += bbox.height/2.0 - center.y;
		cv::warpAffine(image, image, rot, bbox.size());
		return image;
	}

	void data::calc_best_rot_angle(){
		cv::Mat tempImage = this->getImage();
		applyBlur(tempImage);
		double angle = rotation::findRotAngle2(tempImage);
		this->angle = angle;
	}

	void data::applyBlur(cv::Mat image){
		cv::GaussianBlur(image, image, cv::Size(3, 3), 0);
		return;
	}

	cv::Size data::imageSize(){
		return this->image.size();
	}

data::data(cv::Mat image, std::vector<cv::Point> coordinates, char*name) :
			image(image), coordinates(coordinates), name(name) { }

data::~data()
{
	//dtor
}
