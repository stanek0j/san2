
#include <stdlib.h>
#include "abstractserverreceiver.hpp"

namespace CPPL
{
	AbstractServerReceiver::AbstractServerReceiver() :
		mChannel(NULL)
	{
		
	}
		
	AbstractServerReceiver::~AbstractServerReceiver()
	{
		// empty
	}
	
	ErrorCode AbstractServerReceiver::send(char *data, int len)
	{
		return mChannel->send(data, len);
	}

	PipeChannel* AbstractServerReceiver::getPipeChannel()
	{
		return mChannel;
	}

}
