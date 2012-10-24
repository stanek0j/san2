
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>

#include "cnode.hpp"
#include "interfaces/tcp/ctcpinterface.hpp"
#include "utils/platform/sleep.hpp"
#include "utils/config.hpp"
#include "utils/log.h"

#define TIME_CON 1000
#define TIME_RX  1000
#define TIME_TX  1000
#define TCP_QUEUESIZE 50
#define INPUT_QUEUE_SIZE 50

std::string int2string(int x)
{
    std::stringstream stream;
	stream << x;
	return stream.str();
}

std::string getKeyTextString(int x, const std::string &key)
{
	std::string s;
	s.append("peer."); 
	s.append(int2string(x));
	s.append(".");
	s.append(key);
	return s;
}

std::string getPeerConfig(San2::Utils::Config::ConfigFile &cfg, int x, const std::string &key)
{
	return cfg.getValue(getKeyTextString(x, key));
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Missing parameter: configuration file\nUsage: sanode <configFile>\n");
		return -1;
	}
	
	San2::Node::CNode node(INPUT_QUEUE_SIZE);
	
	San2::Utils::Config::ConfigFile cfg;
	
	if (!cfg.load(argv[1]))
	{
		printf("failed to load config file: %s\nexiting\n", argv[1]);
		return -1;
	}
	
	std::string localIp, localPort, remoteIp, remotePort;
		
	int i;
	for (i = 1; ; i++)
	{
		localIp = getPeerConfig(cfg, i, "localIp");
		localPort = getPeerConfig(cfg, i, "localPort");
		remoteIp = getPeerConfig(cfg, i, "remoteIp");
		remotePort = getPeerConfig(cfg, i, "remotePort");
		
		if (!localIp.compare(std::string("")) || !localPort.compare(std::string("")) || !remoteIp.compare(std::string("")) || !remotePort.compare(std::string("")))
		{
			printf("failed to load interface %d", i);
			break;
		}
		
		
		
		std::shared_ptr<San2::Interfaces::CTcpInterface> tcpif(new San2::Interfaces::CTcpInterface(localIp, localPort, remoteIp, remotePort, TIME_CON, TIME_RX, TIME_TX, node.getInputQueue(), TCP_QUEUESIZE));
		printf("adding peer #%d\n", i);
		node.addInterface(tcpif);
	}
	
	printf("infinite loop\n");
	while(1)
	{
		San2::Utils::SanSleep(10);
	}
	
	return 0;
}
