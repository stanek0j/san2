

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>

#include "cnode.hpp"
#include "interfaces/tcp/ctcpinterface.hpp"
#include "utils/platform/sleep.hpp"

#define TIME_CON 1000
#define TIME_RX  1000
#define TIME_TX  1000
#define TCP_QUEUESIZE 50
#define INPUT_QUEUE_SIZE 50

int main(int argc, char *argv[])
{
	San2::Node::CNode node(INPUT_QUEUE_SIZE);
	
	// TODO: Needs fix
	std::shared_ptr<San2::Interfaces::CTcpInterface> tcpif(new San2::Interfaces::CTcpInterface("127.0.0.1", "2222", "127.0.0.1", "2223", TIME_CON, TIME_RX, TIME_TX, node.getInputQueue(), TCP_QUEUESIZE));
	
	
	node.addInterface(tcpif);
	
	
	printf("infinite loop\n");
	while(1)
	{
		San2::Utils::SanSleep(10);
	}
	
	return 0;
}
