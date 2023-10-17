# Smart Flowerpot
The project about ENG5220: Real Time Embedded Programming (2022-23)

Group 27

## Description
The intelligent flowerpot is designed to help you to ensure the plants optimal growth. With sensors, it can monitor your plants’ temperature, humidity and light level. It features a built-in screen that expresses the mood of your plants through emoji and real-time statistics to remind you what you should do to take good care of your lovely plant. Meanwhile, we have built a website interface to monitor and observe real-time condition of your plant remotely.
# Main implementation logic
The implementation of this project consists mainly of two separate threads. One thread reads the sensor data every second and the other thread is a big loop that constantly refreshes the data on the display. There is also a verify function that constantly checks whether the running state is normal or not, if the sensor reads the data timeout, it will get a TIMEOUT state, then the verify function will shut down the currently running main thread and restart the main thread. Of course, when the initialisation fails, the same operation will be performed.
 
## Features
1) Visual expression of plants' mood
2) Real-time and remote monitor on web server
3) Connect to your smart device
4) Immediate feedback of environment figures
5) Intuitive screen display of plants' emojis

## Contributors
|  Name   | ID  | Role |
|  ----  | ----  | ----  |
| Xingtao Zeng  | 2721279z | C++ Software Engineer | 
| Wenbo Yang  | 2727018y | Hardware and 3D Model Engineer | 
| Qidi Wang  | 2724200w | Web Application Engineer| 

# Guide

## 1. Software Installation and Enviroment Setup
### 1.1 Raspberry Configuration
Install Raspberry Pi Imager
https://www.raspberrypi.com/software/

Select according to operating system

After finishing the installment, choose os and storage.
Click burn, then wait a lot of time for the burn to succeed and install the SD card into the card slot of the Raspberry Pi board, plug in the power, the Raspberry Pi initialization configuration is successful

#### Enable SPI
```
sudo raspi-config
Choose Interfacing Options -> SPI -> Yes  to enable SPI interface
```

![Example Image](pic/RPI_open_spi.png)
#### Install Libraries
```
sudo apt-get install wiringpi

#For the Raspberry Pi system after May 2019, it may need to be upgraded

wget https://project-downloads.drogon.net/wiringpi-latest.deb
sudo dpkg -i wiringpi-latest.deb
gpio -v
#You will get 2.52 information if you install it correctly

#Bullseye branch system use the following command:
git clone https://github.com/WiringPi/WiringPi
cd WiringPi
./build
gpio -v

# Run gpio -v and version 2.60 will appear. If it does not appear, it means that there is an installation error
```


## 2. HardWare Build
### 2.1 Raspberry Pi B+/4B+
When connecting the Raspberry Pi, choose to use the 7PIN cable to connect, please refer to the pin correspondence table below.
|OLED	|Raspberry Pi|
|  ----  | ----  |
|VCC	|	3.3V|
|GND	|GND|
|DIN	|	19 / 3|
|CLK		|23 / 5|
|CS		|24|
|DC	|	22|
|RST	|	13|

![Example Image](pic/600px-1.5inch_RGB_OLED_Module-树莓派.jpg)

### 2.2 DHT11 Module (Humidity & Temperautre)
Wire Connection for DHT11 Module
|DHT	|Raspberry Pi|
|  ----  | ----  |
|VCC/+	|	3.3V-5V|
|OUT/DATA/D| 7|
|-/GND	|GND|

### 2.3 Photosensitive Diode Sensor
Wire Connection Photosensitive module
|PDC	|Raspberry Pi|
|  ----  | ----  |
|VCC	|	3.3V-5V|
|DO| 11|
|AO| NOT USED|
|GND	|GND|

### 2.4 128*128 RGB OLED with SPI Interface, or anyother compatiable LED devices to Raspberry Pi B+/4B+

![Example Image](pic/hardwares.jpg)

### 2.5 Plant pot that can accommodate the above-mentioned devices
Here are two options: 
1. DIY your handmade flower pot, make sure to wrap your wires and devices with waterproof materials to avoid water damage to the circuit.

![Example Image](pic/DIY_pot.jpg)

2. 3D print your flower pot, the modeling code (plant pot.obj) and images can be found in "Pot_3D_Model" folder at the github repository.

![Example Image](Pot_3D_Model/PlantFront.png)

## 3. Web Application Setup

# Unit Test
### Install Google Test (gtest)
```
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
make
sudo make install
```
### Run Unit Tests
```
$ g++ RTEP_smart_flowerpot/unit_test/dht_test.cpp -o dht_test
$ ./dht_test

$ g++ RTEP_smart_flowerpot/unit_test/getTime_test.cpp -o getTime_test
$ ./getTime_test

$ g++ RTEP_smart_flowerpot/unit_test/sensor_test.cpp -o sensor_test
$ ./sensor_test

$ g++ RTEP_smart_flowerpot/unit_test/timer_test.cpp -o timer_test
$ ./timer_test
```

# Project Management
## Work Distribution
During the initial meeting, each member of the team presented their unique skillset and prior experience. Drawing upon the available resources, we collectively identified suitable roles and responsibilities for each team member.
|  Name   |  Role | Tasks | 
|  ----  | ----  | ----  | 
| Xingtao Zeng  |  C++ Software Engineer | Purchase Neccessary Equipments,Implement Temperature, Humidity, Photosensitive Sensor drives, System Response and Emoji Algorithms|
| Wenbo Yang  | Hardware and 3D Model Engineer | Assembly Hardwares and Flower Pot, Adjust sensor thresholds, create pot's 3D Model|
| Qidi Wang  | Web Application Engineer| Develop Web Interface, Unit Tests for all Functions, Establish Web Server and Connection|

## Progress Tracker
Our team held regular weekly meetings to facilitate discussions on task management, progress tracking, issue and bug presentation, and recognition of weekly accomplishments. In situations where additional meetings were deemed necessary, they were scheduled accordingly. Furthermore, we diligently recorded meeting minutes to accurately monitor and document team activities.

We documented our weekly plan and progress in the Gantt Chart

![Example Image](pic/gantt.png)

# Future Plan & Marketing Value
Our future plan for the intelligent flowerpot involves developing and implementing more advanced sensors and technologies to enhance its ability to monitor and regulate plant growth. We also plan to expand our product line to include various sizes and designs to cater to different customer needs. In addition, we aim to collaborate with botanical experts to incorporate their insights and recommendations for optimal plant growth. In terms of the marking value of our product, we will conduct market research and analysis to determine a competitive and fair price at around £50. We will emphasize the unique features and benefits of the intelligent flowerpot, such as real-time monitoring, mood expression, and remote access through a website interface. To increase brand awareness and engagement, we will utilize social media and influencer marketing. Furthermore, we will offer customer support and resources, such as tutorials and FAQs, to ensure a positive user experience and increase customer satisfaction.

# Social media


