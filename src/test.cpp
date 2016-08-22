#include "test.h"
test::test(std::vector<data> *datasets, int numberOfTrainingImages){

	this->datasets = datasets;
	this->numberOfTrainingImages = numberOfTrainingImages;
	numberOfTestImages = this->getNumberOfImages() - numberOfTrainingImages;
	assert (numberOfTestImages > 0);
	//SubImages have size 72x72
	this->sizeOfRect = 64;
}

test::~test(){

}

void test::startTesting(int numberOfTrainingElements ){
	cv::Mat trainingClasses(numberOfTrainingElements,1,CV_32FC1);
	cv::Mat trainingData = generateTrainingData(numberOfTrainingElements, trainingClasses);
	this->NN.trainNN(trainingData, trainingClasses);

}

/* Idea: generate with a probability of 0.5 a positive or negative trainingsdata.
 * For that, get a random trainingspicture and sample random elments, until a match in regard of the class is found.
 */
cv::Mat test::generateTrainingData(int numberOfTrainingElements, cv::Mat& trainingClass){
	cv::Mat trainingData(numberOfTrainingElements, this->networkInputSize, CV_32FC1);
	for(int i=0; i< numberOfTrainingElements; ++i){
		std::uniform_int_distribution<int> classDistribution(0,1);
		bool foundSample = false;
		int whichClass = classDistribution(generator);
		while(!foundSample){
			cv::Mat sampleImage;
			std::uniform_int_distribution<int> pictureDistribution(0,this->numberOfTrainingImages-1);
			int dataInt = pictureDistribution(generator);
///			cout << "Choose data number " <<dataInt << endl;
            data  currentData = datasets->at(dataInt);
            //getRotatedImage throws exception when the sample is part of the wrong class
            try{
				sampleImage = getRotatedImage(currentData, whichClass);
			}
			catch(int e){
				// cout << "wrong class found "<< whichClass<<endl;
				continue;
			}
			cout << "class found "<< whichClass <<endl;
			trainingClass.at<float>(i,0) = whichClass;
			string windowName = "Image ";
/*
			cv::namedWindow(windowName);
			cv::Mat temp = rotateImage(currentData.getImage() , currentData.angle);
			for(int j = 0; j < currentData.numberOfStomata(); ++j)
				cv::circle(temp, currentData.getCoordinate(j), 20, CV_RGB(255,255,255),10);

			cv::imshow(windowName + currentData.name, temp);
			cv::waitKey(-1);


			cv::imshow(windowName + currentData.name, sampleImage);
			cv::waitKey(-1);
			cv::destroyWindow(windowName + currentData.name);
*/
			// Do stuff with sample
			// Here: make picture smallsmall
			cv::resize(sampleImage, sampleImage, cv::Size(this->networkInputSize/4, this->networkInputSize/4));
			uchar* img_d = sampleImage.data;
			for(int j = 0; j < this->networkInputSize;  ++j){
				trainingData.at<float>(i,j) =  img_d[j];
			}
			//
			foundSample =true;
		}
	}
	cout << "Finished sampling classes"<<endl;
	return trainingData;
}

int test::getClass(data currentData, cv::Point point){
	//0 equals no stomata in close vicinity
	int currentClass = 0;
	for(int i= 0; i<currentData.numberOfStomata(); ++i){
		cv::Point difference = point - currentData.getCoordinate(i);
		double distance = difference.x*difference.x + difference.y*difference.y;
		if (distance<= maxDistance*maxDistance){
		currentClass = 1;
			break;
		}
	}
	return currentClass;
}
cv::Mat test::getSubImage(cv::Mat imag, cv::Point center, int size){
	cv::Rect rect(center - cv::Point(size/2, size/2),  center + cv::Point(size/2, size/2));
	cv::Mat temp= imag(rect);
	assert (temp.rows  == size);
	return temp;
}

cv::Mat test::rotateImage(cv::Mat image,  double angle){
	cv::Point center(image.cols/2.0, image.rows/2.0);
	cv::Mat rotImage;
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	cv::warpAffine(image, rotImage, rot , image.size());
	return rotImage;
}



cv::Mat test::rotateImageCropped(cv::Mat image,  double angle, double cropfactor){
	cv::Point center(image.cols/2.0, image.rows/2.0);

	cv::Mat rotImage = rotateImage(image, angle);
	//return cropped image
	int newSize =rotImage.cols/cropfactor;
	//cout << newSize << " newsize cols asdasdasd " << rotImage.cols << " rotImage.colscols  "<< endl;
	assert (newSize >= sizeOfRect && newSize < sizeOfRect+1 );
	return getSubImage(rotImage, center, sizeOfRect);
}

cv::Mat test::getRotatedImage(data currentData, int whichClass){
	double angleInRad = M_PI / 180.0 * currentData.angle;
	double cropfactor = 2*abs(cos(angleInRad)) + 2*abs(sin(angleInRad));
	//get the size to crop a picture of desired size after rotation.
	int sizeBefRot 	= ceil(sizeOfRect*cropfactor);
	int proofsize  = sizeBefRot/cropfactor;
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
//cout <<  currentData.angle << " currentData.rot_angle " <<  angleInRad << " angleInRad " << sizeBefRot << " sizeBefRot " << proofsize << " proofsize ."<< endl;
	assert (proofsize == sizeOfRect);
//cout <<  x << " x " <<  y << " y " << sizeBefRot << " sizeBefRot "<< imageSize.width << " imageSize.width "<< imageSize.height << " imageSize.height " << endl;
	cv::Mat subImag = getSubImage(currentData.getImage(), point, sizeBefRot);
//cout << subImag.rows  << " mrows and " << subImag.cols << " cols."<< endl;
	subImag = rotateImageCropped(subImag, currentData.angle, cropfactor);
///	cout << subImag.rows  << " rmmows and " << subImag.cols << " cols."<< endl;
	assert (subImag.rows == sizeOfRect && subImag.cols == sizeOfRect );
	if(getClass(currentData, point) != whichClass)
		throw 0;
	return subImag;
}




int test::getNumberOfImages(){
	return datasets->size();
}
