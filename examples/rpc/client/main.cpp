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

#include "cppl/pipeclient.hpp"
#include "cppl/pipeserver.hpp"
#include "cppl/pipechannel.hpp"
#include "cppl/abstractreceiver.hpp"

#include "clientreceiver.hpp"

#define CLI_TIM_CON 20000
#define CLI_TIM_RX   5000
#define CLI_TIM_TX   5000

#ifdef LINUX
	#define CLI_PIPENAME "/tmp/asd5345ds1asd315aa"
#endif
#ifdef WINDOWS
	#define CLI_PIPENAME "\\\\.\\pipe\\mynamedpipe"
#endif

#define CLI_SLEEP
#define CLI_SLEEP_MSEC 2000

int main(int argc, char *argv[])
{
	char data[50];
	
	char str1[] = "\x0A\x0A\x0A"; // 6
	char str2[] = "abc\x0Attt"; // 8
	char str3[] = "\x0Ag\x0A"; // 5
	char str4[] = "hij\x0A"; // 5
	char str5[] = "klmno"; // 5
	char str6[] = "\x0Apqrs"; // 6
	char str7[] = "tu"; // 2
	char str8[] = "v\x0Aw\x0Ax\x0Ayz"; // 11
	char str9[] = "12345\x0Arrrr\x0AK"; // 14
	char str10[] = "\x0A"; // 16

	#ifdef LINUX
		sprintf(data, "client pid %lu\n", (unsigned long)getpid());
	#endif
	
	#ifdef WINDOWS
		sprintf(data, "client pid %d\x0A", GetCurrentProcessId());
	#endif
	
	
	San2::Cppl::PipeClient pc(CLI_PIPENAME, [](){return new ClientReceiver();}, CLI_TIM_CON, CLI_TIM_RX, CLI_TIM_TX);
	San2::Cppl::ErrorCode rval = pc.open();
	if (rval != San2::Cppl::ErrorCode::SUCCESS)
	{
		 printf("open() failed errcode: %d\n", San2::Cppl::errorCodeToInt(rval));
		 return -1;
	}
	
	pc.start();
	
	printf("***\n");
	rval = pc.send(str1, strlen(str1));
	if (rval != San2::Cppl::ErrorCode::SUCCESS) printf("send(st1) error \n");
	rval = pc.send(str2, strlen(str2));
	if (rval != San2::Cppl::ErrorCode::SUCCESS) printf("send(st2) error \n");
	rval = pc.send(str3, strlen(str3));
	if (rval != San2::Cppl::ErrorCode::SUCCESS) printf("send(st3) error \n");
	rval = pc.send(str4, strlen(str4));
	if (rval != San2::Cppl::ErrorCode::SUCCESS) printf("send(st4) error \n");
	rval = pc.send(str5, strlen(str5));
	if (rval != San2::Cppl::ErrorCode::SUCCESS) printf("send(st5) error \n");
	rval = pc.send(str6, strlen(str6));
	if (rval != San2::Cppl::ErrorCode::SUCCESS) printf("send(st6) error \n");
	rval = pc.send(str7, strlen(str7));
	if (rval != San2::Cppl::ErrorCode::SUCCESS) printf("send(st7) error \n");
	rval = pc.send(str8, strlen(str8));
	if (rval != San2::Cppl::ErrorCode::SUCCESS) printf("send(st8) error \n");
	rval = pc.send(str9, strlen(str9));
	if (rval != San2::Cppl::ErrorCode::SUCCESS) printf("send(st9) error \n");
	rval = pc.send(str10, strlen(str10));
	if (rval != San2::Cppl::ErrorCode::SUCCESS) printf("send(st10) error \n");
	

	while(1)
	{
		/*
		rval = pc.send(data, strlen(data) + 1);
		if (rval)
		{
			printf("pc.send() error \n");
			break;
		} 
		*/
		
		#ifdef CLI_SLEEP
			#ifdef LINUX
				sleep(CLI_SLEEP_MSEC / 1000);
			#endif
			#ifdef WINDOWS
				SleepEx(CLI_SLEEP_MSEC, TRUE);
			#endif
		#endif
	}
	
	return 0;
}
