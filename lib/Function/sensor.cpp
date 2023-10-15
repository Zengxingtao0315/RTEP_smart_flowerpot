/*****************************************************************************
* | File      	:   sensor.cpp
* | Author      :   xingtao Zeng
* | Function    :   Create function classes to read sensors for temperature, humidity, and light intensity
* | Info        :   WENBO YANG modified the function of readDHTdata
******************************************************************************/

#include "Sensor.hpp"

using namespace std;

// Function to continuously read data from the DHT sensor in a loop.
void Sensor::readDHTdataLoop() {
    while (true) {
        // Read data from the DHT sensor.
        DHTdata data = readDHTdata();
        // Pause the loop for 1 second before the next reading.
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Acquire the lock on dataMutex to safely update the temperature and humidity variables.
        std::unique_lock<std::mutex> lock(dataMutex);
        temperature = data.temperature;
        humidity = data.humidity;

        // Notify the threads waiting on the condition variable (dataCondVar) that new data is available.
        dataCondVar.notify_all();

        // If the dhtThread is not joinable, start a new thread to continue reading data.
        if (!dhtThread.joinable()) {
            dhtThread = std::thread(&Sensor::readDHTdataLoop, this);
        }
    }
}

// Function to get the latest temperature reading.
double Sensor::getTemperature() {
    std::unique_lock<std::mutex> lock(dataMutex);
    // Wait for new data to arrive, or time out after 200 milliseconds.
    dataCondVar.wait_for(lock, std::chrono::milliseconds(200), [this] {
        return temperature != lastTemperature;
    });
    // Update the latest temperature value and return it.
    lastTemperature = temperature;
    return temperature;
}

// Function to get the latest humidity reading.
double Sensor::getHumidity() {
    std::unique_lock<std::mutex> lock(dataMutex);
    // Wait for new data to arrive, or time out after 200 milliseconds.
    dataCondVar.wait_for(lock, std::chrono::milliseconds(200), [this] {
        return humidity != lastHumidity;
    });
    // Update the latest humidity value and return it.
    lastHumidity = humidity;
    return humidity;
}

// Constructor for the Sensor class, which sets up the pins and starts the reading loop.
Sensor::Sensor(int digitalPin, int dhtPin)
    : digitalPin(digitalPin), dhtPin(dhtPin), lastTemperature(0.0), lastHumidity(0.0) {
    // Initialize wiringPi. If it fails, print an error message and exit.
    if (wiringPiSetup() == -1) {
        std::cerr << "Error initializing wiringPi" << std::endl;
        exit(1);
    }
    // Start a new thread for reading DHT sensor data in a loop.
    dhtThread = std::thread(&Sensor::readDHTdataLoop, this);
}

// Destructor for the Sensor class, which waits for the dhtThread to finish before cleaning up.
Sensor::~Sensor(){
    if (dhtThread.joinable()) {
        dhtThread.join();
    }
}

// Function to read the digital value from the light sensor.
UWORD Sensor::readDigitalValue() {
    UWORD value = digitalRead(digitalPin);
    return value;
}

// Function to read data from the DHT sensor.
DHTdata Sensor::readDHTdata() {   
	DHTdata data;
    int dht11_dat[5] = {0, 0, 0, 0, 0};
    uint8_t cnt = 7;
    uint8_t idx = 0;
    float f;

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    // Initialize the DHT sensor by pulling the data pin LOW and then HIGH.
    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(19);
    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(40);
    pinMode(DHTPIN, INPUT);

    // Wait for the DHT sensor to respond (ACKNOWLEDGE).
    unsigned int loopCnt = 10000;
    while (digitalRead(DHTPIN) == LOW)
        if (loopCnt-- == 0);

    loopCnt = 10000;
    while (digitalRead(DHTPIN) == HIGH)
        if (loopCnt-- == 0);

    // Read data from the DHT sensor.
    for (int i = 0; i < 40; i++) {
        loopCnt = 10000;
        while (digitalRead(DHTPIN) == LOW)
            if (loopCnt-- == 0);

        unsigned long t = micros();

        loopCnt = 10000;
        while (digitalRead(DHTPIN) == HIGH)
            if (loopCnt-- == 0);

        if ((micros() - t) > 40) {
            dht11_dat[idx] |= (1 << cnt);
        }
        if (cnt == 0) {
            cnt = 7;
            idx++;
        } else {
            cnt--;
        }
    }

    // Check if the data received from the DHT sensor is valid.
    if (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)) {
        data.humidity = dht11_dat[0];
        data.temperature = dht11_dat[2] + dht11_dat[3] * 0.1;
		sendDHTdataToHTML(data);
        f = dht11_dat[2] * 9.0 / 5.0 + 32;
        printf("Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
               dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f);
        return data;
    } else {
        printf("Data not good, skip\n");
    }

    // Return the data read from the DHT sensor.
    return data;
}
void Sensor::sendDHTdataToHTML(const DHTdata& data){
	std::ofstream jsFile("/var/www/html/data.js");

    if (jsFile.is_open()) {
        // Write temperature and humidity data in the data.js file
        jsFile << "var temperature = " << data.temperature << ";\n";
        jsFile << "var humidity = " << data.humidity << ";\n";
		jsFile << "var light = " << readDigitalValue() << ";\n";
        jsFile.close();
    } else {
        std::cerr << "Unable to open data.js file" << std::endl;
    }
	
}