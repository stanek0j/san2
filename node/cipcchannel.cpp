
#include <string>
#include <string.h>

#include "cipcchannel.hpp"
#include "utils/log.h"
#include "utils/cstringutils.hpp"

namespace San2 { namespace Node {

CIpcChannel::CIpcChannel(CPPL_PIPETYPE handle, unsigned int timRX, unsigned int timTX, CNode &node) :
	San2::Cppl::PipeChannel(handle, timRX, timTX),
	m_node(node)
{
	
}

CIpcChannel::~CIpcChannel()
{
	// empty
}

void CIpcChannel::printShell()
{
	San2::Cppl::BufferProcessor::send(m_node.getNodeName());
    San2::Cppl::BufferProcessor::send(">");
}

San2::Cppl::ErrorCode CIpcChannel::receive()
{	
	const unsigned int lineSize = 512;
	char line[lineSize];
	
	sendLine("Welcome to SAN node terminal");
	
	while (1)
	{		
		printShell();
			
		San2::Cppl::ErrorCode rval = readLine(line, lineSize);
		
		if (rval != San2::Cppl::ErrorCode::SUCCESS)
		{
			if (rval == San2::Cppl::ErrorCode::PEER_DISCONNECT)
			{
				// FILE_LOG(logDEBUG4) <<  "terminal peer disconnected";
				return rval;
			}
			FILE_LOG(logDEBUG4) <<  "CIpcChannel::receive():readLine(): failed errorcode: " << errorCodeToInt(rval);
			break;
		}
		
		if (strlen(line) == 0) continue;
	    
        std::string sline(line);
        
        std::vector<std::string> args;
        San2::Utils::CStringUtils::split(args, sline, 0x20); // space split

        if (args.size() < 1) continue;

        if (!args[0].compare("exit"))
        {
            San2::Cppl::BufferProcessor::sendLine("exit requested");
            return San2::Cppl::ErrorCode::REQUEST_DISCONNECT;
        }

        rval = lineParser(args);

        if (rval == San2::Cppl::ErrorCode::SUCCESS) continue;
        return rval;
	}
	
	return San2::Cppl::ErrorCode::SUCCESS;
}


San2::Cppl::ErrorCode CIpcChannel::lineParser(const std::vector<std::string> &args)
{
    if (!args[0].compare("send"))
    {
        San2::Cppl::BufferProcessor::sendLine("send command");
        return San2::Cppl::ErrorCode::SUCCESS;
    }


    San2::Cppl::BufferProcessor::sendLine("unknown command");

    return San2::Cppl::ErrorCode::SUCCESS;
}

}} // ns
