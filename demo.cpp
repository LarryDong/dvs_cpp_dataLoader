
#include <iostream>
#include <Event.h>
#include <EventStream.h>
#include <Frame.h>
#include <FrameStream.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <pthread.h>

using namespace std;

void* thread_viewer(void* argv);

class dataStream{		// define data stream to convey both stream together, for viewer.
public:
	dataStream(){}
	dataStream(EventStream* e, FrameStream* f, bool onlyEvent=false, int n=1000){
		es = e;
		fs = f;
		mbEventOnly = onlyEvent;
		mnShowEventNumber = n;
	}
	EventStream* es;
	FrameStream* fs;

	bool mbEventOnly;		// show event only, not with image.
	int mnShowEventNumber;	// valid when mbEventOnly is true.
};



int main(int argc, char** argv){
	if(argc!=2)
		cout << "Please input the dataset folder." << endl;

	string datasetPath = string(argv[1]);
	int timeScale = 5;
	EventStream* es = new EventStream(datasetPath+"/events.txt", timeScale);
	FrameStream* fs = new FrameStream(datasetPath, timeScale);
	dataStream* ds = new dataStream(es, fs, false);

	cout << "--> init 3 thread..." << endl;
	pthread_t id1, id2, id3;
	int err1 = pthread_create(&id1, NULL, EventStream::handler_startEventStream, es);	// event stream thread.
	if (ds->mbEventOnly == false)
		int err2 = pthread_create(&id2, NULL, FrameStream::handler_startFrameStream, fs);
	int err3 = pthread_create(&id3, NULL, thread_viewer, (void*) ds);
	cout << "<-- init 3 thread succeed." << endl;

	// if(err1!=0 || err2!=0 || err3!=0){		// check thread init succeed.
	// 	cout << "Error when begin two threads." << endl;
	// 	while(1);
	// }

	while(1);		// main loop
	return 0;
}


void* thread_viewer(void* argv){
	cout << "--> Begin the viewer..." << endl;

	dataStream* ds = (dataStream*) argv;
	FrameStream *fs = ds->fs;
	EventStream *es = ds->es;

	Mat src = Mat::zeros(Size(240, 180), CV_8UC1), srcColor;
	vector<Event> vEvent;
	bool showFlag = false;

	while(1){
		if(ds->mbEventOnly){		// only show events.
			if(es->mDataEvents.size()>ds->mnShowEventNumber){	// process after 1000 events;
				es->mDataEventsMutex.lock();
				vEvent.swap(es->mDataEvents);
				es->mDataEvents.clear();
				es->mDataEventsMutex.unlock();
				showFlag = true;
			}
		}
		else{		// show events and frames.
			if(fs->mbNewFrame){			// process when get a new frame.
				fs->mbNewFrame = false;
				// load src frome image 1 or image 2. Use two images to avoid lock.
				if(fs->mnNewImageId==1){
					fs->mImage1Mutex.lock();
					src=fs->mImage1.clone();
					fs->mImage1Mutex.unlock();
				}
				else if(fs->mnNewImageId=2){
					fs->mImage2Mutex.lock();
					src=fs->mImage2.clone();
					fs->mImage2Mutex.unlock();
				}
				else
					cout << "Error. No new image." << endl;
				
				// save events during the two frames.
				es->mDataEventsMutex.lock();
				vEvent.swap(es->mDataEvents);
				es->mDataEvents.clear();
				es->mDataEventsMutex.unlock();
				showFlag = true;
			}
		}
		
		if(showFlag){
			Mat srcColor;
			showFlag = false;
			cvtColor(src, srcColor, COLOR_GRAY2BGR);

			for (int i = 0; i < vEvent.size(); ++i){	// draw all events.
				Event e = vEvent[i];
				uchar *p = srcColor.ptr<uchar>(e.v, e.u);
				if (e.p == true)		// positive to red.
					p[2]=255;
				else
					p[0]=255;
			}
			vEvent.clear();		// clear all events during the two frames

			imshow("events", srcColor);
			waitKey(1);		// fresh the window
		}
	}
}
