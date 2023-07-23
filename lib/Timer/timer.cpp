
#include "timer.hpp"

using namespace std;
<<<<<<< HEAD
void Timer::setTimeout(auto function, int delay) {
	this->clear = false;
	std::thread t([=]() {
		if (this->clear) return;
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		if (this->clear) return;
		function();
=======

Timer::Timer() : stop_flag_(false) {}
Timer::~Timer() {
	stop();
}

template<typename F, typename... Args>
void Timer::setTimeout_ms(F&& f, int delay_ms, Args&&... args) {
	clear();
	std::thread t([=]() {
		if (stop_flag_.load()) return;
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
		if (stop_flag_.load()) return;
		std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
>>>>>>> a8684b4589533e7a54dacd86e0764595828524fb
	});
	t.detach();
}

<<<<<<< HEAD
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
=======
template<typename F, typename... Args>
void Timer::setInterval_ms(F&& f, int interval_ms, Args&&... args) {
	clear();
	std::thread t([=]() {
		while (!stop_flag_.load()) {
			std::unique_lock<std::mutex> lock(mutex_);
			if (cv_.wait_for(lock, std::chrono::milliseconds(interval_ms),
							 [=]() { return stop_flag_.load(); })) {
				return;
			}
			std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
		}
	});
	thread_ = std::move(t);
}

void Timer::stop() {
	stop_flag_.store(true);
	if (thread_.joinable()) {
		thread_.join();
	}
}













DelayedTimer::DelayedTimer() : running(false) {}

void DelayedTimer::setTimeout(F&& function, int delay, Args&&... args) {
        std::unique_lock<std::mutex> lock(mutex_);

        if (running_) {
            condition_.wait(lock, [=]{ return !running_; });
        }

        running_ = true;
        std::thread t([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            std::invoke(function, std::forward<Args>(args)...);
            std::unique_lock<std::mutex> lock(mutex_);
            running_ = false;
            condition_.notify_one();
        });
        t.detach();
>>>>>>> a8684b4589533e7a54dacd86e0764595828524fb
}