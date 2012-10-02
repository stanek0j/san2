
#include <stdlib.h>
#include "abstractreceiver.hpp"
#include "helper.hpp"

namespace CPPL
{
	AbstractReceiver::AbstractReceiver()
	{
		// empty	
	}
		
	AbstractReceiver::~AbstractReceiver()
	{
		// empty
	}
	
	void AbstractReceiver::disconnected(ErrorCode errcode)
	{
		// empty
	}
}
