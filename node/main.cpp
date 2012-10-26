
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <algorithm>

#include "cnode.hpp"
#include "interfaces/tcp/ctcpinterface.hpp"
#include "utils/platform/sleep.hpp"
#include "utils/config.hpp"
#include "utils/log.h"
#include "utils/hex.hpp"
#include "network/ccapsule.hpp"
#include "network/nettypedef.hpp"

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

// ugly conversion
bool string2address(const std::string & strAddress, San2::Network::SanAddress &sanAddress)
{
	San2::Utils::bytes b;
	if (San2::Utils::hexToBytes(strAddress, b) != true) // string -> bytes
	{
		FILE_LOG(logDEBUG3) << "Cannot convert hex digits (string2address)\n";
		return false;
	}
	
	printf("b.size(): %d\n", b.size());
	if (b.size() != San2::Network::sanAddressSize) return false;
	std::copy(b.begin(), b.end(), sanAddress.begin()); // bytes -> SanAddress
	return true;
}


int main(int argc, char *argv[])
{
	FILELog::ReportingLevel() = logDEBUG4;
	FILE_LOG(logDEBUG3) << "logger working";
	
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
	
	std::string localIp, localPort, remoteIp, remotePort, ifAddress;
    
    San2::Utils::bytes ba;
    San2::Network::SanAddress sanaddr;
		
	int i;
	for (i = 1; ; i++)
	{
		ifAddress = getPeerConfig(cfg, i, "ifAddress"); 
		localIp = getPeerConfig(cfg, i, "localIp");
		localPort = getPeerConfig(cfg, i, "localPort");
		remoteIp = getPeerConfig(cfg, i, "remoteIp");
		remotePort = getPeerConfig(cfg, i, "remotePort");
		
		if (!ifAddress.compare(std::string("")) || !localIp.compare(std::string("")) || !localPort.compare(std::string("")) || !remoteIp.compare(std::string("")) || !remotePort.compare(std::string("")))
		{
			printf("failed to parse interface %d", i);
			break;
		}
		
		if (string2address(ifAddress, sanaddr) != true)
		{
			printf("failed to parse peer.%d.ifAddress from config\n", i);
			continue; //skip
		}
		
		std::shared_ptr<San2::Interfaces::CTcpInterface> tcpif(new San2::Interfaces::CTcpInterface(sanaddr, localIp, localPort, remoteIp, remotePort, TIME_CON, TIME_RX, TIME_TX, node.getInputQueue(), TCP_QUEUESIZE));
		printf("adding peer #%d\n", i);
		node.addInterface(tcpif);
	}
	
	node.start();
	
	
	
	printf("\ninfinite loop\n");
	
	std::string strx = cfg.getValue("testsend");
	
	while(1)
	{
		if (strx.compare(std::string("")))
		{
			std::shared_ptr<San2::Network::CCapsule> cap(new San2::Network::CCapsule);
			
			San2::Network::SanAddress dstAddress;
			if (string2address("000000000000000000000000000000000000000000000000000000000000FF31", dstAddress) != true)
			{
				FILE_LOG(logDEBUG2) << "failed to parse destination address";
			}
			cap->setDestinationAddress(dstAddress);
			
			FILE_LOG(logDEBUG4) << "@@@ sending";
			node.injectCapsule(cap);
		}
		San2::Utils::SanSleep(3);
	}
	
	return 0;
}
