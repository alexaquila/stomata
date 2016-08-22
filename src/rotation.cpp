#include "rotation.h"

rotation::rotation()
{
	//ctor
}

rotation::~rotation()
{
	//dtor
}

//Better idea: find a quadratic rectangle, which can be completely rotated in the middle of image
//Calculate postlrating for that, makes also rotations over 45^degrees comparable
double rotation::findRotAngle(cv::Mat image){
		double bestAngle = 0;
		double bestPostlRating = 0;

        int squareSize = std::min(image.cols, image.rows)/(cos(CV_PI/4)*2);
        if(squareSize%2 == 1)
			--squareSize;
		//sin(a)+cos(a) max, if a == 45 degrees
        std::cout <<image.cols <<" image.cols " <<image.rows <<" image.rows " <<squareSize <<" squareSize" << std::endl;
		cv::Point center(image.cols/2.0, image.rows/2.0);
		for (int alpha = -50; alpha < 50; alpha++) {
			double*sumColumns = new double[squareSize];
			for(int i=0; i<squareSize; ++i)
				sumColumns[i] = 0;
			cv::Mat rotImage;
			cv::Mat rot = cv::getRotationMatrix2D(center, alpha, 1.0);
			cv::warpAffine(image, rotImage, rot , image.size());

			cv::Rect rect(center - cv::Point(squareSize/2,squareSize/2),  center + cv::Point(squareSize/2,squareSize/2));
			cv::Mat temp = rotImage(rect);
			cv::GaussianBlur(rotImage, rotImage, cv::Size(3, 3), 0);

			uchar* img_d = temp.data;
	///		std::cout << squareSize << " squareSize " << temp.cols   << " temp.cols " << std::endl;
			assert (temp.cols == squareSize);
			for (int x = 0; x < squareSize; ++x) {
				// Traverse all rows // TODO: Enlarge this number to catch all data points?
				for (int y = 0; y < squareSize; ++y) {
					//std::cout << r <<" r " <<c <<" c " << center.x << "  center.x " << center.y << "  center.y "  << std::endl;
					sumColumns[x] += img_d[y * image.cols + x];
				}
				sumColumns[x] /= squareSize;
			}
			double currentPostlRating = calculatePostlRating(sumColumns, squareSize) - abs(alpha);

			if(currentPostlRating > bestPostlRating){
				bestPostlRating = currentPostlRating;
				bestAngle = alpha;
			}
			//std::cout <<  "currentPostlRating " << currentPostlRating << " alpha: " << alpha << " bestPostlRating: " << bestPostlRating<<std::endl;
		}
		return bestAngle;
}

double rotation::calculatePostlRating(double *sumColumns, int numberCols){
	double sum =0;
	for(int i=0; i<numberCols-1; i++){
		//std::cout << "current Col =  " << sumColumns[i] << std::endl;
		double temp = (sumColumns[i] - sumColumns[i+1]);
		sum += temp*temp;
	}
	//std::cout << "postl " << sum << std::endl;
	return sum;
}

/*
void rotation::rotateAroundCenter(int& out_r, int& out_c,
		const int in_r, const int in_c, const int rws, const int cls,
		const double cos_a, const double sin_a){
		// Rotate
		out_r = static_cast<int>((in_r - rws / 2)*cos_a + (in_c - cls / 2)*sin_a);
		out_c = static_cast<int>(-(in_r - rws / 2)*sin_a + (in_c - cls / 2)*cos_a);
		// Transform back
		out_r += rws / 2;
		out_c += cls / 2;
	}
*/


/*
double rotation::findRotAngle(cv::Mat image){
		double bestAngle = 0;
		double bestPostlRating = 0;
		uchar* img_d = image.data;
		for (int alpha = -45; alpha < 45; alpha++) {
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
	*/
