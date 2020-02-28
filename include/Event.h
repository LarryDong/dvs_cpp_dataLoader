
#pragma once
#include <iostream>
#include <iomanip>

using namespace std;

class Event{
public:
	
	Event(){}
	Event(float time, int x, int y, bool polarity):u(x), v(y), p(polarity), ts(time){}
	~Event(){	
		// need to be implemented if an event with pointer/class is deleted.
	}

	void showInfo() {
		cout << "Ts: " << fixed << setprecision(8) << ts
			 << ", pos: (" << u << ", " << v << "), polarity: " << ((p == true) ? 1 : 0) << endl;
	}

	Event& operator=(const Event& e){
		this->ts = e.ts;
		this->u = e.u;
		this->v = e.v;
		this->p = e.p;
	}

	int u, v;
	bool p;		// polarity, 0 for off, 1 for on.
	float ts;	// time stamp
};

