#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

int main() {
    // start time at 6:00am
    std::time_t start_time = std::time(nullptr);
    std::tm start_tm = *std::localtime(&start_time);
    start_tm.tm_hour = 6;
    start_tm.tm_min = 0;
    start_tm.tm_sec = 0;
    start_time = std::mktime(&start_tm);

    // duration of each interval (in seconds)
    const int interval = 60 * 60; // 1 hour

    // initialize variables
    int duration = 0;
    int digitalvalue = 1;

    // loop through 24 hours
    for (int i = 0; i < 24; ++i) {
        // loop through each interval
        for (int j = 0; j < interval; ++j) {
            // check digital value
            if (digitalvalue == 1) {
                duration++;
            }

            // wait for 1 second
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // print duration for this hour
        std::cout << "Duration for hour " << i + 1 << ": " << duration << " seconds\n";

        // reset duration for next hour
        duration = 0;
    }

    return 0;
}
