#include <iostream>
#include <string>
#include <thread>
#include <csignal>
#include <ctime>
#include <cmath>

#include <boost/asio.hpp>
#include <memory>

#include <wiringPi.h>

#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

#include "../lib/Config/DEV_Config.hpp"
#include "../lib/Config/Debug.hpp"
#include "../lib/GUI/GUI_Paint.hpp"
#include "../lib/Function/Sensor.hpp"
#include "../lib/Function/Selector.hpp"
#include "../lib/Function/Plant.hpp"
#include "../lib/Function/GetTime.hpp"
#include "../lib/OLED/OLED.hpp"
#include "../lib/Function/checkInternet.hpp"



using namespace std;



int main()
{
	mainLoop mainloop;
	mainloop.Loop();
	mainloop.StatuChecker();
    return 0;
}
