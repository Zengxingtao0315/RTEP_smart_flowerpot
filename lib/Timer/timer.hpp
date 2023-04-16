#include <iostream>
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>

class Timer {
public:
    Timer();
    ~Timer();

    template<typename F, typename... Args>
    void setTimeout_ms(F&& f, int delay_ms, Args&&... args);

    template<typename F, typename... Args>
    void setInterval_ms(F&& f, int interval_ms, Args&&... args);

    void stop();

private:
    void clear(){
        stop_flag_.store(false);
    };

    std::atomic<bool> stop_flag_;
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

class DelayedTimer {
public:
    DelayedTimer();
    void setTimeout(std::function<void()> function, int delay);

private:
    bool running;
    std::mutex mutex;
    std::condition_variable condition;
};