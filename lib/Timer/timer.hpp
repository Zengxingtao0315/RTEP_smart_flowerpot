#include <iostream>
#include <thread>
#include <chrono>

class Timer {
	bool clear = false;

public:
	void setTimeout(auto function, int delay);
	void setInterval(auto function, int interval);
	void stop();

};