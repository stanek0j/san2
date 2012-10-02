#pragma once

#ifdef UNIX
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <unistd.h> /* for close() for socket */
	#include <arpa/inet.h>
	#include <netdb.h>
#endif

#ifdef WINDOWS
    #include <winsock2.h>
	#include <Ws2tcpip.h>
	#include <Wspiapi.h>
#endif

#ifdef UNIX
    #define SNET_SOCKTYPE int
    #define SNET_BADSOCKET -1
    #define SNET_SOCKCLOSE(x) close(x)
    #define SNET_PORTTYPE uint16_t
    #define SNET_TIMOTYPE unsigned long
#endif

#ifdef WINDOWS
    #define SNET_SOCKTYPE SOCKET
    #define SNET_BADSOCKET INVALID_SOCKET
    #define SNET_SOCKCLOSE(x) closesocket(x)
    #define SNET_PORTTYPE u_short    
    #define SNET_TIMOTYPE unsigned long
#endif
