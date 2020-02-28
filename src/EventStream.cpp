
#include <iostream>
#include <EventStream.h>
#include <Event.h>

using namespace std;



// construct function
EventStream::EventStream(const string& file, int timeScale):mFilePath(file){
	mData.open(mFilePath, ios::in);
	if(!mData.is_open())
		cout << "Cannot load file." << endl;
	mnTimeScale = timeScale; // set time scale, to slow down the speed. default 1.
	mbNewEvent = false;		// default, no new events.
}


void EventStream::startEventStream(){
	mtStart = chrono::system_clock::now();
	loadEvent();
	while(1){
		if(checkTimeForEvent()){	// if it's time to generate a frame
			generateEvent();
		}
		else{
			;		// just wait.
		}
	}
	
}

// load the next event.
bool EventStream::loadEvent(){
	if(mData.eof())
		return false;
	float ts;
	int u, v, p;
	mData >> ts >> u >> v >> p;
	if(p!=0 && p!=1)		// a simple way to check validation
		return false;

	mNextEventMutex.lock();
	mNextEvent.ts = ts;
	mNextEvent.u = u;
	mNextEvent.v = v;
	mNextEvent.p = (p == 1) ? true : false;
	mNextEventMutex.unlock();
	return true;
}

bool EventStream::generateEvent(){
	mNextEventMutex.lock();
	mEvent = mNextEvent;	// generate the event from just loaded.
	mbNewEvent = true;	 	// set the flag. Tell others a new event is coming.
	// save event to dataEvents or buffer.
	if(mDataEventsMutex.try_lock()){	// dataEvents is not reading by others. Just save new events.
		if(mBufferEvents.size()!=0){
			// if dataEvents is valid, and buffer not empty, means dataEvents has been readed and should be updated by buffer.
			mDataEvents.swap(mBufferEvents);	// swap the vector instead of copy.
			mBufferEvents.clear();		// clear the buffer.
		}
		mDataEvents.push_back(mEvent);	// push new event
		mDataEventsMutex.unlock();
	}
	else{
		mBufferEvents.push_back(mEvent);	// if dataEvents is locked(by other threads), save to buffer fisrt
	}
	mNextEventMutex.unlock();
	loadEvent();		// load the next one.
}

bool EventStream::checkTimeForEvent(){
	auto time_curr = chrono::system_clock::now();
	auto us= chrono::duration_cast<chrono::microseconds>(time_curr - mtStart);
	long tmp = (long)(mNextEvent.ts * 1000000) * mnTimeScale;
	if ((long)us.count() < tmp)	{
		return false;
	}
	else{
		return true;
	}
}

