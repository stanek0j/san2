
#include <stdlib.h>
#include "abstractserverreceiver.hpp"

namespace San2 { namespace Cppl {
	
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

}} // ns
