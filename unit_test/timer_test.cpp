#include "../Timer/timer.hpp"

#include <gtest/gtest.h>
#include <chrono>


using namespace std;

TEST(TimerTest, setTimeout) {
    Timer timer;
    auto start = chrono::high_resolution_clock::now();
    bool flag = false;
    timer.setTimeout([&]() {
        flag = true;
    }, 1000);
    timer.stop();
    auto end = chrono::high_resolution_clock::now();
    EXPECT_FALSE(flag);
    EXPECT_TRUE(chrono::duration_cast<chrono::milliseconds>(end - start).count() < 1000);
}

TEST(TimerTest, setInterval) {
    Timer timer;
    auto start = chrono::high_resolution_clock::now();
    int count = 0;
    timer.setInterval([&]() {
        count++;
    }, 100);
    this_thread::sleep_for(chrono::milliseconds(500));
    timer.stop();
    auto end = chrono::high_resolution_clock::now();
    EXPECT_EQ(5, count);
    EXPECT_TRUE(chrono::duration_cast<chrono::milliseconds>(end - start).count() >= 500);
}

TEST(TimerTest, stop) {
    Timer timer;
    bool flag = false;
    timer.setTimeout([&]() {
        flag = true;
    }, 1000);
    timer.stop();
    this_thread::sleep_for(chrono::milliseconds(500));
    EXPECT_FALSE(flag);
}
