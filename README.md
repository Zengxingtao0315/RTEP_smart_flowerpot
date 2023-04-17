# Smart Flowerpot
The project about ENG5220: Real Time Embedded Programming (2022-23)
Group 27

## Description
The intelligent flowerpot is designed to help you to ensure the plants optimal growth. With sensors, it can monitor your plants’ temperature, humidity and light level. It features a built-in screen that expresses the mood of your plants through emoji and real-time statistics to remind you what you should do to take good care of your lovely plant. Meanwhile, we have built a website interface to monitor and observe real-time condition of your plant remotely.
 
## Features
1) Visual expression of plants' mood
2) Real-time and remote monitor on web server
3) Connect to your smart device
4) Immediate feedback of environment figures
5) Intuitive screen display of plants' emojis

## Contributors
|  Name   | ID  |
|  ----  | ----  |
| Xingtao Zeng  | 2721279z |
| Wenbo Yang  | 2727018y |
| Qidi Wang  | 2724200w |

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
### 2.5 Plant pot that can accommodate the above-mentioned devices
Here are two options: 
1) DIY your handmade flower pot, make sure to wrap your wires and devices with waterproof materials to avoid water damage to the circuit.
2) 3D print your flower pot, the code and images for 3D model can be found in "Pot 3D Model" folder at the github repository.
