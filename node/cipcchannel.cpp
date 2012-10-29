
#include <string>
#include <string.h>

#include "cipcchannel.hpp"
#include "utils/log.h"

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
		printf("READLINE\n");
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
		
		if (strlen(line) == 0)
        {
            continue;
        }
        
	    std::string sline(line);

	    if (!sline.compare("exit"))
	    {
           San2::Cppl::BufferProcessor::sendLine("exit requested");
		   return San2::Cppl::ErrorCode::REQUEST_DISCONNECT;
	    }


	    San2::Cppl::BufferProcessor::sendLine("unknown command");
	    continue;
		
	}
	
	return San2::Cppl::ErrorCode::SUCCESS;
}

}} // ns
