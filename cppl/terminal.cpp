#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>

#ifdef LINUX
	#include <sys/types.h>
	#include <unistd.h>
#endif

#ifdef WINDOWS
	#include <windows.h>
#endif

#include "pipeclient.hpp"
#include "terminalreceiver.hpp"

#define CLI_TIM_CON 20000
#define CLI_TIM_RX   5000
#define CLI_TIM_TX   5000

#define MAX_LINE_LEN 256

#ifdef LINUX
	#define CLI_PIPENAME "/tmp/asd5345ds1asd315aa"
#endif
#ifdef WINDOWS
	#define CLI_PIPENAME "\\\\.\\pipe\\mynamedpipe"
#endif

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("bad parameter; usage terminal <pipename>\n");
		return -1;
	}
	
	char line[256];
	San2::Cppl::PipeClient pc(argv[1], [](){return new San2::Cppl::TerminalReceiver();}, CLI_TIM_CON, CLI_TIM_RX, CLI_TIM_TX);
	San2::Cppl::ErrorCode rval = pc.open();
	
	if (rval != San2::Cppl::ErrorCode::SUCCESS) 
	{
		printf("pc.open() failed errcode: %d\n", errorCodeToInt(rval));
		return -1;
	}
	
	pc.start();

	while(1)
	{
		std::cin.getline(line, MAX_LINE_LEN);
		
		rval = pc.send(line, strlen(line));
		if (rval != San2::Cppl::ErrorCode::SUCCESS)
		{
			printf("error: sending data from client to server failed\n");
			break;
		}

		rval = pc.send("\x0A", 1); // line feed
		if (rval != San2::Cppl::ErrorCode::SUCCESS)
		{
			printf("error: sending data from client to server failed\n");
			break;
		} 

	}

	printf("\nTerminating client ...\n");
	pc.terminateAndJoin();
	return 0;
}
