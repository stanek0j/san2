#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "tcp/ctcpserver.hpp"
#include "tcp/ctcpchannel.hpp"
#include "serverreceiver.hpp"

#define TIMEOUT_CON 5000
#define TIMEOUT_RX  5000
#define TIMEOUT_TX  5000

#define IP_ADDRESS "0.0.0.0"
#define PORT "2345"

San2::Tcp::CTcpChannel* createChannel(SNET_SOCKTYPE sock, unsigned long s_addr, unsigned int timRX, unsigned int timTX)
{
	return new ServerReceiver(sock, s_addr, timRX, timTX);
}

San2::Tcp::CTcpServer ps(IP_ADDRESS, PORT, createChannel, TIMEOUT_CON, TIMEOUT_RX, TIMEOUT_TX);


int main(int argc, char *argv[])
{
	printf("RpcServer\n");
	ps.start();
	ps.join();
	
	//printf("returned: %d\n", San2::Tcp::errorCodeToInt(ps.getErrorCode()));	
	return 0;
}
