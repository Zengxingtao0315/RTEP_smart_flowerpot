#ifndef _CHECKINTERNET_HPP_
#define _CHECKINTERNET_HPP_

#include <cstring>
extern "C" {
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
}

class InternetConnectionChecker {
public:
    CheckInternetConnection();
        
};