#pragma once

#ifdef UNIX
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <unistd.h> /* for close() for socket */
	#include <arpa/inet.h>
	#include <netdb.h>
#endif

#ifdef WIN32
	#pragma comment(lib, "wsock32.lib")
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

// get sockaddr, IPv4 or IPv6:
inline void *san_get_in_addr(struct sockaddr *sa)
{
	// horsi uz to nebude :)
    if (sa->sa_family == AF_INET) return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

inline void san_cleanup_socket(SNET_SOCKTYPE *sock)
{
	if (*sock != SNET_BADSOCKET) SNET_SOCKCLOSE(*sock);
	*sock = -1;
}
