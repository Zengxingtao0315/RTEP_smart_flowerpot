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
#include "sensor.hpp"
#include "./Selector.hpp"
#include "./Plant.hpp"
#include "./GetTime.hpp"
#include "../OLED/OLED.hpp"
#include "./checkInternet.hpp"


using namespace std;
typedef enum{
    TIMEOUT,
    INITERROR,
    SUCCESS,
    UNKNOWN_ERROR
}STATE;








class mainLoop{
    private:
        DEV dev;
        Sensor sensor;
        std::thread Loopthread; //
        std::thread Checkthread;
        std::atomic<STATE> loopState;

    public:
        mainLoop();
        ~mainLoop();
        void loop();
        void StateChecker();
        void stopLoop();
        void setLoopState(STATE Lstate);
        STATE getLoopState();
    

};



#endif