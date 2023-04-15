
#include "timer.hpp"

using namespace std;
void Timer::setTimeout(auto function, int delay) {
	this->clear = false;
	std::thread t([=]() {
		if (this->clear) return;
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		if (this->clear) return;
		function();
	});
	t.detach();
}

void Timer::setInterval(auto function, int interval) {
	this->clear = false;
	std::thread t([=]() {
		while (true) {
			if (this->clear) return;
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
			if (this->clear) return;
			function();
		}
	});
	t.detach();
}

void Timer::stop() {
	this->clear = true;
}