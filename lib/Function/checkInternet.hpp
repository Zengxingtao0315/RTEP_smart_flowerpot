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
    bool CheckInternetConnection();

private:
	// Declare and define an addrinfo struct named "hints" to provide hints for the getaddrinfo function for address resolution.
	struct addrinfo hints;
	// Declare and define a pointer to an addrinfo struct named "res" to store the result returned by the getaddrinfo function.
	struct addrinfo *res;

	int status = 0;
	
};


#endif