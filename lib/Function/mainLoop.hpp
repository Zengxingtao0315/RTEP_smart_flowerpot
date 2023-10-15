#ifndef _MAINLOOP_HPP_
#define _MAINLOOP_HPP_

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

#include "../Config/DEV_Config.hpp"
#include "../Config/Debug.hpp"
#include "../GUI/GUI_Paint.hpp"
#include "./Sensor.hpp"
#include "./Selector.hpp"
#include "./Plant.hpp"
#include "./GetTime.hpp"
#include "../OLED/OLED.hpp"
#include "./checkInternet.hpp"


using namespace std;
typedef enum{
    TIMEOUT;
    INITERROR;
    SUCCESS;
    UNKNOWN_ERROR;
}STATE;




void Handler(int signo, DEV * DEV)
{
	// System Exit
	std::cout << "Handler: exit" << std::endl;
	DEV.ModuleExit();

	std::exit(0);
}



class mainLoop{
    private:
        DEV DEV;
        Sensor Sensor(DIGITALPIN,  DHTPIN);
        std::thread Loopthread; //
        std::atomic<STATE> loopState;

    public:
        mainLoop();
        ~mainLoop();
        void loop();
        void StateChecker();
        void stopLoop();
        void setLoopState(STATE Lstate);
        STATE getLoopState();
        void controlWriteToE(Paint& paint);
        e.writeToBuffer(value);
    }

}



#endif