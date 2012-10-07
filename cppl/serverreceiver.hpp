
#pragma once
#include "abstractreceiver.hpp"

namespace San2
{	
	namespace Cppl
	{
		class ServerReceiver : public AbstractReceiver
		{
		  public:
			ServerReceiver();
			//int received(char *data, unsigned int len);
		  protected:
			~ServerReceiver();
			ErrorCode run();
		  private:

			 // another msvc fix
			#ifdef LINUX
				ServerReceiver(const ServerReceiver& copyFromMe)=delete;
				ServerReceiver& operator=(const ServerReceiver& copyFromMe)=delete;
			#endif
			
		};
	}
}
