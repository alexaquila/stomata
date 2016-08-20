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
	assert (temp.rows  == size);
	return temp;
}

cv::Mat test::rotateImage(cv::Mat image,  double angle){
	cv::Point center(image.cols/2.0, image.rows/2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	cv::Mat rotImage;
	cv::warpAffine(image, rotImage, rot , image.size());
	//return cropped image
	int newSize =(rotImage.cols*(abs(cos(angle*M_PI / 180.0))));
///cout << newSize << " newsize cols asdasdasd " << rotImage.cols << " rotImage.colscols  "<< endl;
	assert (newSize >= sizeOfRect);
	return getSubImage(rotImage, center, sizeOfRect);
}

cv::Mat test::getRotatedImage(data currentData, int whichClass){
	double angleInRad = M_PI / 180.0 * currentData.rot_angle;
	//get the size to crop a picture of desired size after rotation.
	int sizeBefRot 	= ceil (sizeOfRect/(abs(cos(angleInRad))));
int proofsize  = sizeBefRot*(abs(cos(angleInRad)));
	//Assert even number
	if((sizeBefRot % 2)	!= 0)
		++sizeBefRot ;
	cv::Size imageSize = currentData.imageSize();
	std::uniform_int_distribution<int> xDistribution(sizeBefRot/2,imageSize.width-sizeBefRot/2);
	int x = xDistribution(generator);
	std::uniform_int_distribution<int> yDistribution(sizeBefRot/2,imageSize.height-sizeBefRot/2);
	int y = yDistribution(generator);
	cv::Point point(x,y);
///	if(sizeBefRot/2 >point.x || sizeBefRot/2 >point.y || sizeBefRot/2 >currentData.imageSize().width-point.x || sizeBefRot/2 >currentData.imageSize().height-point.y )
///		cout << "oh no" << endl;
///cout <<  currentData.rot_angle << " currentData.rot_angle " <<  angleInRad << " angleInRad " << sizeBefRot << " sizeBefRot " << proofsize << " proofsize ."<< endl;
///	assert (proofsize == sizeOfRect);
///cout <<  x << " x " <<  y << " y " << sizeBefRot << " sizeBefRot " << endl;
	cv::Mat subImag = getSubImage(currentData.getImage(), point, sizeBefRot);
///cout << subImag.rows  << " mrows and " << subImag.cols << " cols."<< endl;
	subImag = rotateImage(subImag, currentData.rot_angle);
///	cout << subImag.rows  << " rmmows and " << subImag.cols << " cols."<< endl;
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
///			cout << "Choose data number " <<dataInt << endl;
            data  currentData = datasets->at(dataInt);
            //getRotatedImage throws exception when the sample is part of the wrong class
            try{
				getRotatedImage(currentData, whichClass);
			}
			catch(int e){
				cout << "wrong class found"<<endl;
				continue;
			}
			foundSample =true;
		}
	}
	cout << "Finished sampling classes"<<endl;
}


int test::getNumberOfImages(){
	return datasets->size();
}
