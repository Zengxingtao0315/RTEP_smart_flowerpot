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



int main()
{
	mainLoop mainloop;
	mainloop.Loop();
	mainloop.StatuChecker();
    return 0;
}
