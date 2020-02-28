
#pragma once

#include <iostream>
// #include <Frame.h>
#include <fstream>
#include <string>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <mutex>

using namespace std;

class FrameStream{
public:
	FrameStream(const string &path, int timeScale = 1);
	bool generateFrame();
	void startFrameStream();		// main function.
	static void * handler_startFrameStream(void *tmp){		// used to begin the thread.
		cout << "--> Begin frame thread." << endl;
		FrameStream *p = (FrameStream*) tmp;
		p->startFrameStream();
	}
	
	// To sync the time stamp from system. Not implemented yet.
	bool setFrameStartTime(chrono::system_clock::time_point t){;} 

	bool mbNewFrame;			// a new frame comes out
	cv::Mat mImage1, mImage2;	// output image, 1 or 2. If another thread is reading 1, then write to 2.
	int mnNewImageId;			// record which image is the newest.
	mutex mImage1Mutex, mImage2Mutex;

private:
	bool checkTimeForFrame();	// check when to generate a new frame.
	bool loadNextFrame();		// load a frame from file.

	// parameters for load a frame.
	ifstream mImagesInfo;		// file where to load images information.
	float mNextFrameTime;
	string mFramePath;
	string mNextFrameName;
	
	// frame stram parameters.
	int mnTimeScale;		// time scale to slowdonw. For example, 10 means 10 times slower than real-time.
	chrono::system_clock::time_point mtStart;

	// image buffer is used when load image from files.
	cv::Mat mImageBuffer;	// image buffer is used to load image from file.
	bool mbBufferNew;
	mutex mBufferMutex;
};

