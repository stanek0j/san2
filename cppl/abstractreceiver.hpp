
#pragma once

#include "helper.hpp"
#include "bufferprocessor.hpp"

namespace San2
{	
	namespace Cppl
	{		
		class AbstractReceiver
		{
		  public:
			AbstractReceiver();
			virtual ErrorCode run()=0;

			BufferProcessor* getBufferProcessor()
			{
				return m_bp;
			}
			
			virtual void disconnected(ErrorCode errcode);
			
			ErrorCode send(char *data, int len)
			{
				return m_bp->send(data, len);
			}
			
			
		  protected:
			virtual ~AbstractReceiver();
			//virtual int send(char *data, int len)=0;
		  private:
			 // another msvc fix
			#ifdef LINUX
				AbstractReceiver(const AbstractReceiver& copyFromMe)=delete;
				AbstractReceiver& operator=(const AbstractReceiver& copyFromMe)=delete;
			#endif
			
			BufferProcessor *m_bp;
			
			friend class PipeChannel;
			friend class PipeClient;
		};
	}
}
