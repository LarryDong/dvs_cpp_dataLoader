#include <iostream>
#include <FrameStream.h>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

FrameStream::FrameStream(const string &path, int timeScale){
	mImagesInfo.open(path+"/images.txt", ios::in);
	if(!mImagesInfo.is_open())
		cout << "Cannot find 'images.txt'." << endl;
	mnTimeScale = timeScale;
	mbNewFrame = false;
	mFramePath = path;		// save absolut path.
}

void FrameStream::startFrameStream(){
	mtStart = chrono::system_clock::now();	// set start time.
	loadNextFrame();	// first load next frame.

	mImage1 = mImageBuffer;	// init the vales.
	mImage2 = mImageBuffer;

	while(1){			// begin
		if(checkTimeForFrame())		// if it's the time to generate a frame
			generateFrame();
		else
			;
	}
}


bool FrameStream::loadNextFrame(){
	if(mImagesInfo.eof())
		return false;
	float ts;
	string imageName;
	mImagesInfo >> ts >> imageName;		// read timestamp and image name from images.txt
	// cout << "Next frame. Time " << ts << ", name: " << imageName << endl;
	mNextFrameTime = ts;
	mNextFrameName = imageName;
	string pathName = mFramePath + "/" + mNextFrameName;
	Mat tmp = imread(pathName, 0);		// read file.

	mBufferMutex.lock();
	mImageBuffer = tmp;		// since use "Mat A=B", not copied data, so move the imageBuffer to a new tmp image.
	mBufferMutex.unlock();
	
	if(mImageBuffer.empty()){
		cout << "Cannot load image from: " << pathName << endl;
		return false;
	}
	return true;
}


bool FrameStream::generateFrame(){
	if (mImage1Mutex.try_lock()){	// try to write in image1 if not reading by others.
		mImage1 = mImageBuffer;		// not copy all the image data. So cannot directly change mImageBuffer then.
		mImage1Mutex.unlock();
		mbNewFrame = true;
		mnNewImageId = 1;
	}
	else{
		if(mImage2Mutex.try_lock()){	// otherwise save to image2.
			mImage2 = mImageBuffer;
			mImage2Mutex.unlock();
			mbNewFrame = true;
			mnNewImageId = 2;
		}
		else
			mnNewImageId=-1;
	}
	loadNextFrame();		// every time a frame generated, load the next.
}


bool FrameStream::checkTimeForFrame(){
	auto time_curr = chrono::system_clock::now();
	auto us= chrono::duration_cast<chrono::microseconds>(time_curr - mtStart);
	long tmp = (long)(mNextFrameTime * 1e6) * mnTimeScale;
	if ((long)us.count() < tmp)	{
		return false;
	}
	else{
		return true;
	}
}

