#include "../Function/GetTime.hpp"
#include <gtest/gtest.h>
#include <chrono>



TEST(SunlightDurationRecorderTest, TestGetSunlightDurationInHours) {
  SunlightDurationRecorder sdr;

  // Test that the function returns 0 when initialized.
  EXPECT_EQ(sdr.getSunlightDurationInHours(true), 0.0);

  // Test that the function returns the correct sunlight duration in hours.
  // This assumes that the device running the test has been initialized for at least 1 hour.
  float duration = sdr.getSunlightDurationInHours(false);
  EXPECT_GT(duration, 0.0);
  EXPECT_LE(duration, 24.0);  // The duration should not exceed 24 hours.
}