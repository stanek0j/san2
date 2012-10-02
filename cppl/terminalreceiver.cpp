
#include <stdio.h>
#include "terminalreceiver.hpp"

TerminalReceiver::TerminalReceiver()
{
	
}
	
CPPL::ErrorCode TerminalReceiver::run()
{
    CPPL::PipeClient *client = getPipeClient();

    unsigned int bytesRead;
    const unsigned int dataSize = 512;
    char data[dataSize];

    while(client->readSome(data, dataSize, &bytesRead) == CPPL::ErrorCode::SUCCESS)
    {
	    fwrite(data, 1, bytesRead, stdout);
    }
	return CPPL::ErrorCode::SUCCESS;
}
 
TerminalReceiver::~TerminalReceiver()
{
	// empty	
}
	