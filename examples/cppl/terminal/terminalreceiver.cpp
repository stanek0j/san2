
#include <stdio.h>

#include "cppl/bufferprocessor.hpp"
#include "terminalreceiver.hpp"

TerminalReceiver::TerminalReceiver(const char *pipeName, unsigned int timCON, unsigned int timRX, unsigned int timTX) :
	San2::Cppl::PipeClient(pipeName, timCON, timRX, timTX)
{
	
}
	
San2::Cppl::ErrorCode TerminalReceiver::receive()
{
	unsigned int bytesRead;
	const unsigned int dataSize = 512;
	char data[dataSize];
	while(readSome(data, dataSize, &bytesRead) == San2::Cppl::ErrorCode::SUCCESS) fwrite(data, 1, bytesRead, stdout);	
	return San2::Cppl::ErrorCode::SUCCESS;
}
 
TerminalReceiver::~TerminalReceiver()
{
	// empty	
}
