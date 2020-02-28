
#pragma once

#include <iostream>
#include <Event.h>
#include <fstream>
#include <string>
#include <chrono>
#include <mutex>
#include <vector>
using namespace std;

class EventStream{
public:

	EventStream(const string &path, int timeScale = 1);

	bool generateEvent();		// generate event to mEvent and set mbNewEvent flag.
	void startEventStream();	// start events-stream.
	static void * handler_startEventStream(void *tmp){	// this function is used to begin thread.
		EventStream *p=(EventStream*) tmp;
		p->startEventStream();
	}

	// get start time for FB camera. Not used now.
	chrono::system_clock::time_point getEventStartTime(){return mtStart;}	

	// generated event
	Event mEvent;
	bool mbNewEvent;
	vector<Event> mDataEvents;	// generated events are saved in mDataEvents.
	mutex mDataEventsMutex;

	
private:
	bool loadEvent();		// load event to mNextEvent;
	bool checkTimeForEvent();	// check whether the time to generate an event. Return true if time is late.

	// loaded event. Not generated yet, just load to stream from file
	Event mNextEvent;		// use next event to check the time.
	mutex mNextEventMutex;		// used when loading or reading the a new Event.
	vector<Event> mBufferEvents;	// if mDataEventsMutex is reading by other threads, save to buffer first.
	
	// stream parameters.
	int mnTimeScale;		// use time scale to slow down the event speed.
	const string mFilePath;
	ifstream  mData;
	chrono::system_clock::time_point mtStart;
};
