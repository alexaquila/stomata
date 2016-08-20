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
		return this->rot_angle;
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
		double angle = findRotAngle(tempImage);
		this->rot_angle = angle;
	}

/*	void data::rotate(){
		// Clone constant Mat
		cv::Mat tempImage = this->getImage();
		// Preprocessing
		applyBlur(tempImage);
		double alpha = findRotAngle(tempImage);
		//std::cout << "perfect angle " << alpha << std::endl;

		rotateImage(alpha);
	}
*/
	void data::rotateAroundCenter(int& out_r, int& out_c,
		const int in_r, const int in_c, const int rws, const int cls,
		const double cos_a, const double sin_a){
		// Rotate
		out_r = static_cast<int>((in_r - rws / 2)*cos_a + (in_c - cls / 2)*sin_a);
		out_c = static_cast<int>(-(in_r - rws / 2)*sin_a + (in_c - cls / 2)*cos_a);
		// Transform back
		out_r += rws / 2;
		out_c += cls / 2;
	}

	double data::calculatePostlRating(double *sumColumns, int numberCols){
		double sum =0;
		for(int i=0; i<numberCols-1; i++){
			double temp = (sumColumns[i] - sumColumns[i+1]);
			sum += temp*temp;
		}
		//cout << "postl " << sum << endl;
		return sum;
	}

	double data::findRotAngle(cv::Mat image){
		cout << "Rotation" << endl;
		double bestAngle = 0;
		double bestPostlRating = 0;
		uchar* img_d = image.data;
		for (int alpha = -50; alpha < 50; alpha++) {
			//Init
			double*sumColumns = new double[image.cols];
			for(int i=0; i<image.cols; ++i)
				sumColumns[i] = 0;
			double angle = (double)alpha / 180 * CV_PI;
			double cos_a = cos(angle); // Conversion deg -> rad
			double sin_a = sin(angle); // Conversion deg -> rad
			for (int c = 0; c < image.cols; ++c) {
				int i = 0;
				// Traverse all rows // TODO: Enlarge this number to catch all data points?
				for (int r = 0; r < image.rows; ++r) {
					int x = r; int y = c;
					rotateAroundCenter(x, y, r, c, image.rows, image.cols, cos_a, sin_a);
					// Dont count pixel out of bounds
					if ((x >= 0) && (x < image.rows) && (y >= 0) && (y < image.cols)) {
						sumColumns[c] += img_d[x * image.cols + y];
						//p[c][alpha + MINMAX_ROT] += img_d[crds[0] * img.cols + crds[1]];// (double)img.at<uchar>(crds[0], crds[1]);
						i++;
					}
				}
				// Normalise by pixel count
				if (i != 0)
					sumColumns[c] /= i; //p[c][alpha + MINMAX_ROT] /= i;

			}
			double currentPostlRating = calculatePostlRating(sumColumns, image.cols);
			if(currentPostlRating > bestPostlRating){
				bestPostlRating = currentPostlRating;
				bestAngle = alpha;
			}
		}
		return bestAngle;
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
