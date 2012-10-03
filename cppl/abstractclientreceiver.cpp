
#include <stdlib.h>
#include "abstractclientreceiver.hpp"

namespace San2 { namespace Cppl {
	
	AbstractClientReceiver::AbstractClientReceiver() :
		mClient(NULL)
	{
		
	}
		
	AbstractClientReceiver::~AbstractClientReceiver()
	{
		// empty
	}
	
	
	ErrorCode AbstractClientReceiver::send(char *data, int len)
	{
		return mClient->send(data, len);
	}

	PipeClient* AbstractClientReceiver::getPipeClient()
	{
		return mClient;
	}

}} //ns
