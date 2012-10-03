
#pragma once

#include "helper.hpp"

namespace San2
{	
	namespace Cppl
	{
		class AbstractReceiver
		{
		  public:
			AbstractReceiver();
			virtual ErrorCode run()=0;
			virtual void disconnected(ErrorCode errcode);
		  protected:
			virtual ~AbstractReceiver();
			//virtual int send(char *data, int len)=0;
		  private:
			 // another msvc fix
			#ifdef LINUX
				AbstractReceiver(const AbstractReceiver& copyFromMe)=delete;
				AbstractReceiver& operator=(const AbstractReceiver& copyFromMe)=delete;
			#endif
		};
	}
}
