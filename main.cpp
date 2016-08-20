#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <cmath>
#include <random>
#include <dirent.h>

#include "data.h"
#include "test.h"

int numberOfLayers = 3;
int sizeOfRect = 72;
string imagePath = "data/images/";
string locationPath = "data/locations/";
string rotationPath = "data/rotations/";
void copyChars(char *source, char *target){
	int i= 0;
	while(source[i] != '\0'){
		target[i] = source[i];
		++i;
	}
	target[i] = '\0';
}


void setRotationAngle(data * data,  string currentLocation){
	//Look, if rotation has already been calculated and stored in data/locations/
	ifstream location(currentLocation);
	if (location.is_open()){
		cout << "Previous calulation of rotation-angle found."<< endl;

		std::string line;
		std::getline(location, line);
		std::stringstream iss(line);
		cout << line << endl;
		double x = 0;
		if (!(iss >> data->rot_angle)){
			cout << " oh no, no parsing "<< endl;
		}
	}
	else{
		cout << "No previous calulation of rotation-angle found. Calculate Rotation."<< endl;

		data->calc_best_rot_angle();
		std::ofstream outfile (currentLocation);
		outfile << (double)data->rot_angle << std::endl;
		outfile.close();
	}
}

void showStomata(data data){
	string windowName = "Image ";
	cv::namedWindow(windowName);
	cv::Mat temp = data.getImage();
	for(int j = 0; j < data.numberOfStomata(); ++j)
		cv::circle(temp, data.getCoordinate(j), 20, CV_RGB(255,255,255),10);
	cv::imshow(windowName + data.name, temp);
	cv::waitKey(-1);
	cv::destroyWindow(data.name);
}

std::vector<data> getData(){
	std::vector<data> datasets;
	struct dirent *entry;
	DIR *dir = opendir(imagePath.c_str());
	if (dir != NULL){
		//Skip first 2 entries . & ..
		entry = readdir(dir);
		entry = readdir(dir);
		while ((entry = readdir(dir))){
			ifstream location(locationPath + entry->d_name + ".txt");
			//Add the picture only if there is ground-truth data
			if (location.is_open()){
				char *name = new char[128];
				copyChars(entry->d_name, name);
				cv::Mat image= cv::imread(imagePath + name, CV_LOAD_IMAGE_GRAYSCALE);
				std::vector<cv::Point> coordinates;
				std::string line;
				while (std::getline(location, line)){
					std::istringstream iss(line);
					int x, y;
					char c;
					if (!(iss >> x >> c >> y)){
						cout << " oh no, no parsing "<< endl;
						break;
					}
					coordinates.push_back(cv::Point(x,y));
				}
				data currentData = data(image, coordinates, name);

				setRotationAngle(&currentData,  rotationPath + entry->d_name + ".rot");
				//showStomata(currentData);
				datasets.push_back(currentData);
				location.close();
			}
		}
	}
	return datasets;
}


cv::Mat getSubImage(cv::Mat imag, cv::Point center, int size){
	cv::Rect rect(center - cv::Point(size/2, size/2),  center + cv::Point(size/2, size/2));
	cv::Mat temp= imag(rect);
	cout << temp.rows  << " rows and " << temp.cols << " cols."<< endl;
	return temp;
}


cv::Mat rotateImage(cv::Mat image,  double angle){
	cv::Point center(image.cols/2.0, image.rows/2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	// determine bounding rectangle
	// adjust transformation matrix
	//rot.at<double>(0,2) += bbox.width/2.0 - center.x;
	//rot.at<double>(1,2) += bbox.height/2.0 - center.y;
	cv::warpAffine(image, image, rot , image.size());

	return getSubImage(image, center, image.cols*(abs(cos(2*angle))));
}





int main(void){
	std::vector<data> datasets = getData();
	test testInstance = test(&datasets, 160);
//	showStomata(datasets[0]);
	//testInstance.startTesting();

	string windowName = "Image ";
	cv::namedWindow(windowName);
	cv::Mat temp = datasets[0].getImage();
	for(int j = 0; j < 1; ++j) //datasets[0].numberOfStomata(); ++j)
		cv::rectangle(temp, datasets[0].getCoordinate(j) - cv::Point(36, 36),  datasets[0].getCoordinate(j) + cv::Point(36, 36), CV_RGB(255,255,255),4);
	cv::imshow(windowName + datasets[0].name, temp);
	cv::waitKey(-1);
	cv::destroyWindow(datasets[0].name);
	int sizeBefRot 	= 100;
	cv::Mat miniMat = getSubImage(temp, datasets[0].getCoordinate(0), sizeBefRot);

	cv::imshow(windowName + datasets[0].name, miniMat);
	cv::waitKey(-1);

	cout << miniMat.rows  << " mrows and " << miniMat.cols << " cols."<< endl;

	cv::Mat miniminiMat = rotateImage(miniMat, datasets[0].rot_angle);

	cout << miniminiMat.rows  << " rmmows and " << miniminiMat.cols << " cols."<< endl;

	cv::imshow(windowName + datasets[0].name, miniminiMat);
	cv::waitKey(-1);



	cv::imshow(windowName + datasets[0].name, miniMat);
	cv::waitKey(-1);

	cv::destroyWindow(datasets[0].name);




	//for(unsigned int i = 0; i<datasets.size(); ++i)
	/*
	for(unsigned int i = 0; i<2; ++i){
		showStomata(datasets[i]);
		datasets[i].calc_best_rot_angle();
		datasets[i].rotate();
		showStomata(datasets[i]);
	}
	*/


	return EXIT_SUCCESS;
}

