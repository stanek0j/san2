#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <unistd.h>
#endif

#ifdef WINDOWS
	#include <windows.h>
#endif


#include "clientreceiver.hpp"

#define CLI_TIM_CON 20000
#define CLI_TIM_RX   5000
#define CLI_TIM_TX   5000

#define CLI_SLEEP
#define CLI_SLEEP_MSEC 2000

#define CLIENT_IP "127.0.0.1"
#define DESTPORT "2345"

int main(int argc, char *argv[])
{
	printf("RpcClient\n");
	/*
	char data[50];
	data[0] = 0xFF;
	data[1] = 0x00;
	data[2] = 0x00;
	data[3] = 0x00;
	*/
	ClientReceiver pc(CLIENT_IP, DESTPORT, CLI_TIM_CON, CLI_TIM_RX, CLI_TIM_TX);
	San2::Tcp::TcpErrorCode rval = pc.open();
	if (rval != San2::Tcp::TcpErrorCode::SUCCESS)
	{
		 printf("open() failed errcode: %d\n", San2::Tcp::errorCodeToInt(rval));
		 return -1;
	}
	
	pc.start();
	
	/*
	rval = pc.send(data, 4);
	if (rval != San2::Cppl::ErrorCode::SUCCESS)
	{
		printf("pc.send() error \n");
		return -1;
	} 
	* */

	pc.join();
	
	return 0;
}
