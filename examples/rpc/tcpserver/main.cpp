#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#ifdef WIN32
	#pragma comment(lib, "wsock32.lib")
    #include <winsock2.h>
	#include <Ws2tcpip.h>
	#include <Wspiapi.h>
#endif

#include "tcp/ctcpserver.hpp"
#include "tcp/ctcpchannel.hpp"
#include "serverreceiver.hpp"

#define TIMEOUT_CON 5000
#define TIMEOUT_RX  5000
#define TIMEOUT_TX  5000

#define IP_ADDRESS "0.0.0.0"
#define PORT "2345"

San2::Tcp::CTcpChannel* createChannel(SNET_SOCKTYPE sock, unsigned long addr, unsigned int timRX, unsigned int timTX)
{
	return new ServerReceiver(sock, addr, timRX, timTX);
}

San2::Tcp::CTcpServer ps(IP_ADDRESS, PORT, createChannel, TIMEOUT_CON, TIMEOUT_RX, TIMEOUT_TX);


int main(int argc, char *argv[])
{
	#ifdef WIN32
		WSADATA wsaData;
		int rval = WSAStartup(0x202, &wsaData);
		if (rval)
		{
			printf("WSAStartup failed: %d", rval);
			return -1;
		}
	#endif

	printf("RpcServer\n");
	ps.start();
	ps.join();
	
	#ifdef WIN32
		// TODO: Error checking
		WSACleanup();
	#endif

	//printf("returned: %d\n", San2::Tcp::errorCodeToInt(ps.getErrorCode()));	
	return 0;
}
