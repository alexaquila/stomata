#include "NNinputSamplePCA.h"

NNinputSamplePCA::NNinputSamplePCA(int numberOfTrainingElements, cv::Size  imageSize):NNinputSample(numberOfTrainingElements, imageSize){
	this->pcaData = cv::Mat(numberOfTrainingElements, imageSize.width * imageSize.height /16, CV_32FC1);
	this->networkInputSize = 255;
}

NNinputSamplePCA::~NNinputSamplePCA()//
{
	//dtor
}

void NNinputSamplePCA::addSample(cv::Mat image, int whichClass){
	trainingClasses.at<float>(currentImageIndex,0) = whichClass;
	cv::Mat tempImage;
	this->images.push_back(image);
	cv::resize(image, tempImage, cv::Size(this->imageSize.width/4, this->imageSize.height/4));

	tempImage.clone().reshape(1, 1).convertTo(this->pcaData.row(currentImageIndex), CV_32FC1);
	++this->currentImageIndex;
}

cv::Mat NNinputSamplePCA::getTransformedSamples(){
	std::cout << "Perform PCA. " << std::endl;
	this->pca = new cv::PCA(this->pcaData, cv::Mat(), CV_PCA_DATA_AS_ROW, this->networkInputSize);
	//cv::Mat compressed.create(this->getNumberOfTrainingElements(), numberOfComponents, CV_32FC1);
    this->trainingData = cv::Mat(this->getNumberOfTrainingElements(), this->networkInputSize, CV_32FC1);
	//std::cout << "trainingData " << trainingData.cols << " , " << trainingData.rows<< std::endl;

	for( int i = 0; i < this->getNumberOfTrainingElements(); ++i){
    //    pca->project(pcaData.row(i), this->trainingData.row(i));
		cv::Mat test = pcaData.row(i).reshape(0, images[0].rows);
		cv::normalize(test, test, 0, 255, cv::NORM_MINMAX, CV_8UC1);

	//	imshow("1.1",test );

//cv::Mat rec;
//		pca->backProject(this->trainingData.row(i), rec);

//		imshow("2", rec.reshape(0, images[0].rows));
		cv::normalize(test, test, 0, 255, cv::NORM_MINMAX, CV_8UC1);

		this->trainingData.row(i) = this->transformInput(images[i]);


//	imshow("2", images[i]);
cv::Mat rec2;

		pca->backProject(this->trainingData.row(i), rec2);
		rec2 = rec2.reshape(0, images[0].rows);
		cv::normalize(rec2, rec2, 0, 255, cv::NORM_MINMAX, CV_8UC1);

	//	imshow("3", rec2);


	   // cv::waitKey(0);
        // and measure the error
     //   printf("%d. diff = %g\n", i, norm(pcaData.row(i), rec, cv::NORM_L2));
    }
	return this->trainingData;

}


cv::Mat NNinputSamplePCA::transformInput(cv::Mat image){
	cv::Mat tempImage;
	cv::resize(image, tempImage, cv::Size(this->imageSize.width/4, this->imageSize.height/4));
	tempImage.clone().reshape(1, 1).convertTo(tempImage, CV_32FC1, 1, 0);
	cv::Mat retData = cv::Mat(1, this->networkInputSize, CV_32FC1);
	pca->project(tempImage, retData);
	//std::cout << " retData " <<  retData.cols << " , " << retData.rows<< std::endl;

	return retData;
}



/*
	cv::Mat mean = pca.mean.clone();
    cv::Mat eigenvalues = pca.eigenvalues.clone();
    cv::Mat eigenvectors = pca.eigenvectors.clone();
	std::cout << "Mean " << mean.cols << " , " << mean.rows<< std::endl;

	std::cout << " images[0] " <<  images[0].cols << " , " <<  images[0].rows<< std::endl;

	cv::Mat imag = mean.reshape(1, images[0].rows);

	std::cout << " imag " <<  imag.cols << " , " << imag.rows<< std::endl;

	cv::normalize(imag, imag, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	imshow("avg", imag);

	imag= pca.eigenvectors.row(0);
	cv::normalize(imag, imag, 0, 255, cv::NORM_MINMAX, CV_8UC1);

	imshow("pc1", imag.reshape(1, images[0].rows));

    cv::waitKey(0);
*/
