#include "test.h"


test::test(std::vector<data> *datasets, int numberOfTrainingImages){

	this->datasets = datasets;
	this->numberOfTrainingImages = numberOfTrainingImages;
	numberOfTestImages = this->getNumberOfImages() - numberOfTrainingImages;
	assert (numberOfTestImages > 0);
	//SubImages have size 72x72
	this->sizeOfRect = 72;
}

test::~test(){

}

void test::startTesting(){
	cv::Mat trainingData = generateTrainingData(1000);


}

cv::Mat test::getSubImage(cv::Mat imag, cv::Point center, int size){
	cv::Rect rect(center - cv::Point(size/2, size/2),  center + cv::Point(size/2, size/2));
	cv::Mat temp= imag(rect);
	cout << temp.rows  << " rows and " << temp.cols << " cols."<< endl;
	return temp;
}

cv::Mat test::rotateImage(cv::Mat image,  double angle){
	cv::Point center(image.cols/2.0, image.rows/2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	cv::Mat rotImage;
	cv::warpAffine(image, rotImage, rot , image.size());
	//return cropped image
	int newSize =(int) rotImage.cols*(abs(cos(2*angle)));
	cout << newSize << " cols asdasdasd " << endl;
	assert (newSize

	 == sizeOfRect);
	return getSubImage(rotImage, center, rotImage.cols*(abs(cos(2*angle))));
}

cv::Mat test::getRotatedImage(data currentData, cv::Point point){
	cv::Mat temp = currentData.getImage();
	//get the size to crop a picture of desired size after rotation.
	int sizeBefRot 	= ceil (sizeOfRect/(abs(cos(2*currentData.rot_angle))));
	int proofsize  = sizeBefRot*(abs(cos(2*currentData.rot_angle)));
	//Assert even number
	if(sizeBefRot % 2 !=0)
		++sizeBefRot ;
	cout <<  currentData.rot_angle << " currentData.rot_angle " << sizeBefRot << " sizeBefRot " << proofsize << " proofsize ."<< endl;

	assert (proofsize == sizeOfRect);
	cv::Mat subImag = getSubImage(temp, point, sizeBefRot);

	cout << subImag.rows  << " mrows and " << subImag.cols << " cols."<< endl;

	subImag = rotateImage(subImag, currentData.rot_angle);

	cout << subImag.rows  << " rmmows and " << subImag.cols << " cols."<< endl;
	assert (subImag.rows == sizeOfRect && subImag.cols == sizeOfRect );
	return subImag;
}

/* Idea: generate with a probability of 0.5 a positive or negative trainingsdata.
 * For that, get a random trainingspicture and sample random elments, until a match in regard of the class is found.
 */
cv::Mat test::generateTrainingData(int numberOfTrainingElements){
	for(int i=0; i< numberOfTrainingElements; ++i){
		std::uniform_int_distribution<int> classDistribution(0,1);
		bool foundSample = false;
		int whichClass = classDistribution(generator);
		while(!foundSample){
			std::uniform_int_distribution<int> pictureDistribution(0,this->numberOfTrainingImages-1);
			int dataInt = pictureDistribution(generator);
			cout << "Choose data number " <<dataInt << endl;
            data  currentData = datasets->at(dataInt);

            cv::Size imageSize = currentData.imageSize();
			std::uniform_int_distribution<int> xDistribution(0,imageSize.width);
			int x = xDistribution(generator);
			std::uniform_int_distribution<int> yDistribution(0,imageSize.height);
			int y = yDistribution(generator);
			try{
				getRotatedImage(currentData, cv::Point(x, y));
			}
			catch(int e){
				continue;
			}

			if( whichClass== 0){


			}
			else{

			}
		}
	}





}


int test::getNumberOfImages(){
	return datasets->size();
}
