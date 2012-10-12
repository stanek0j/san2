
#pragma once

#include "tcphelper.hpp"
#include "citcpbuffer.hpp"

namespace San2
{	
	namespace Tcp
	{		
		class CITcpReceiver
		{
		  public:
			CITcpReceiver();
			virtual TcpErrorCode run()=0;
			virtual void disconnected(TcpErrorCode errcode);
			CITcpBuffer* getBufferProcessor();
		  protected:
			virtual ~CITcpReceiver();

		  private:
			 // another msvc fix
			#ifdef LINUX
				CITcpReceiver(const CITcpReceiver& copyFromMe)=delete;
				CITcpReceiver& operator=(const CITcpReceiver& copyFromMe)=delete;
			#endif
			
			friend class CTcpChannel;
			friend class CTcpClient;
			CITcpBuffer* m_bp;
		  			
			
		};
	}
}
