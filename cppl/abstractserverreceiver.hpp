
#pragma once

#include "abstractreceiver.hpp"
#include "pipechannel.hpp"

namespace San2
{
	namespace Cppl
	{
		class PipeChannel;
		
		class AbstractServerReceiver : public AbstractReceiver
		{
		  public:
			AbstractServerReceiver();
			
		  protected:
			virtual ~AbstractServerReceiver();
			ErrorCode send(char *data, int len);
			PipeChannel* getPipeChannel();
			
		  private:
			friend class PipeChannel;
			PipeChannel* mChannel;

			// another msvc fix
			#ifdef LINUX
				AbstractServerReceiver(const AbstractServerReceiver& copyFromMe)=delete;
				AbstractServerReceiver& operator=(const AbstractServerReceiver& copyFromMe)=delete;
			#endif
		};
	}
}
