#include <iostream> // C++ standard header file that includes the definition of cout and endl.
#include "checkInternet.hpp"

/*******************************************************************************
Function:
    Check internet connection
*******************************************************************************/

// Declaration and definition of a class named InternetConnectionChecker.
class InternetConnectionChecker {
public:
    // Member function: Checks if the internet connection is available.
    bool CheckInternetConnection() {
        // Declare and define an addrinfo struct named "hints" to provide hints for the getaddrinfo function for address resolution.
        struct addrinfo hints;
        // Declare and define a pointer to an addrinfo struct named "res" to store the result returned by the getaddrinfo function.
        struct addrinfo *res;
        // Use the memset function to set all bytes of the "hints" struct to 0 to ensure there are no random values.
        std::memset(&hints, 0, sizeof hints);
        // Set the "hints" struct's ai_family field to AF_UNSPEC, which means the address family (IPv4 or IPv6) will be determined by the system.
        hints.ai_family = AF_UNSPEC;
        // Set the "hints" struct's ai_socktype field to SOCK_STREAM, indicating that we are interested in a TCP (stream) socket.
        hints.ai_socktype = SOCK_STREAM;
        // Set the "hints" struct's ai_flags field to AI_PASSIVE, indicating that we want the result to be suitable for binding a socket.
        hints.ai_flags = AI_PASSIVE;

        // Call the getaddrinfo function, attempting to resolve the hostname "www.google.com" to an IP address.
        // The arguments are: the hostname, service (NULL indicates not specifying a specific service), the "hints" struct, and the result storage variable "res".
        int status = getaddrinfo("www.google.com", NULL, &hints, &res);
        // If the getaddrinfo function fails (returns a non-zero value), it means the internet connection could not be established or the hostname could not be resolved to an address. The function returns false.
        if (status != 0) {
            return false;
        }
        
        // Free the memory allocated by the getaddrinfo function for the addrinfo struct linked list pointed to by "res".
        freeaddrinfo(res);
        // If getaddrinfo executed successfully and resolved the hostname, the function returns true, indicating that the internet connection is available.
        return true;
    }
};
