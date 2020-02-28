
// Event struct
#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// Frame is not used yet.
class Frame{
public:
	Frame(){}
	Frame(const Mat &src, const string& name) { 
		mImage = src.clone();
		mImageName = name;
	}
	string mImageName;
	int mId;		// frame ID
	int mTimestamp;		// frame time stamp
	Mat mImage;
};
