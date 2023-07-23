#include "../Function/sensor.hpp"
#include <gtest/gtest.h>
#include <chrono>


// Test fixture for the Sensor class
class SensorTest : public ::testing::Test {
protected:
    Sensor sensor;
    
    void SetUp() override {
        sensor = Sensor(1, 2);
    }
};

// Test the readDigitalValue() method
TEST_F(SensorTest, ReadDigitalValue) {
    // Assume that the digital sensor is connected to pin 1 and is currently low
    // Simulate a high signal on the pin
    DEV.gpio1 = HIGH;
    
    // Read the digital value from the sensor
    UWORD value = sensor.readDigitalValue();
    
    // Expect the value to be HIGH (since we simulated a high signal)
    EXPECT_EQ(value, HIGH);
}

// Test the readDHTdata() method
TEST_F(SensorTest, ReadDHTdata) {
    // Assume that the DHT sensor is connected to pin 2
    // Simulate a response from the sensor
    DEV.dht_temperature = 25.0;
    DEV.dht_humidity = 50.0;
    
    // Read the DHT data from the sensor
    double temperature, humidity;
    dhtSTAT status = sensor.readDHTdata(&temperature, &humidity);
    
    // Expect the status to be SUCCESS (since we simulated a valid response)
    EXPECT_EQ(status, SUCCESS);
    
    // Expect the temperature and humidity values to be as simulated
    EXPECT_DOUBLE_EQ(temperature, 25.0);
    EXPECT_DOUBLE_EQ(humidity, 50.0);
}