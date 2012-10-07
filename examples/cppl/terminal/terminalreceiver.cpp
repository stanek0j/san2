
#include <stdio.h>

#include "cppl/bufferprocessor.hpp"
#include "terminalreceiver.hpp"

TerminalReceiver::TerminalReceiver()
{
	
}
	
San2::Cppl::ErrorCode TerminalReceiver::run()
{
	San2::Cppl::BufferProcessor *bp = getBufferProcessor();

	unsigned int bytesRead;
	const unsigned int dataSize = 512;
	char data[dataSize];

	while(bp->readSome(data, dataSize, &bytesRead) == San2::Cppl::ErrorCode::SUCCESS)
	{
		fwrite(data, 1, bytesRead, stdout);
	}
	return San2::Cppl::ErrorCode::SUCCESS;
}
 
TerminalReceiver::~TerminalReceiver()
{
	// empty	
}
