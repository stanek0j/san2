
#include <stdio.h>
#include "terminalreceiver.hpp"

namespace San2 { namespace Cppl {

	TerminalReceiver::TerminalReceiver()
	{
		
	}
		
	ErrorCode TerminalReceiver::run()
	{
		PipeClient *client = getPipeClient();

		unsigned int bytesRead;
		const unsigned int dataSize = 512;
		char data[dataSize];

		while(client->readSome(data, dataSize, &bytesRead) == ErrorCode::SUCCESS)
		{
			fwrite(data, 1, bytesRead, stdout);
		}
		return ErrorCode::SUCCESS;
	}
	 
	TerminalReceiver::~TerminalReceiver()
	{
		// empty	
	}
	
}} // ns
