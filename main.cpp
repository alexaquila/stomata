#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <cmath>
#include <random>
#include <dirent.h>

#include "opencv2/opencv.hpp"
#include "data.h"


using namespace std;

int numberOfLayers = 3;
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
	ifstream location(locationPath + entry->d_name + ".txt");
	if (location.is_open()){



	}
	else{


	}
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

				setRotationAngle(currentData,  rotationPath + entry->d_name + ".txt");
				showStomata(currentData);
				datasets.push_back(currentData);
				location.close();
			}
		}
	}
	return datasets;
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

int main(void){
	std::vector<data> datasets = getData();

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

