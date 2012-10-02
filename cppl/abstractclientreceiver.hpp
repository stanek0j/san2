
#pragma once

#include "abstractreceiver.hpp"
#include "pipeclient.hpp"

namespace CPPL
{
	class PipeClient;
	
	class AbstractClientReceiver : public virtual AbstractReceiver
	{
	  public:
	    AbstractClientReceiver();
	  protected:
	    virtual ~AbstractClientReceiver();
	    ErrorCode send(char *data, int len);
	    PipeClient* getPipeClient();
	  private:
		friend class PipeClient;
		PipeClient* mClient;
		
		// another msvc fix
		#ifdef LINUX		
			AbstractClientReceiver(const AbstractClientReceiver& copyFromMe)=delete;
			AbstractClientReceiver& operator=(const AbstractClientReceiver& copyFromMe)=delete;
		#endif
	};

}
