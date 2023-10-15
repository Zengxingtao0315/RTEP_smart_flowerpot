#ifndef __MAIN_
#define __MAIN_

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

#include "../lib/Function/mainLoop.hpp"




using namespace std;

void Handler(int signo)
{
	DEV dev;
    // System Exit
	std::cout << "Handler: exit" << std::endl;
	dev.ModuleExit();

	std::exit(0);
}

int main()
{
	std::signal(SIGINT, Handler);
	mainLoop mainloop;
	mainloop.loop();
	mainloop.StateChecker();
    return 0;
}

#endif