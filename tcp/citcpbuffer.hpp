
#pragma once

#include "tcphelper.hpp"
#include "utils/cthread.hpp"

namespace San2
{
	namespace Tcp
	{
		class CITcpBuffer : public San2::Utils::CThread
		{
		public:
			CITcpBuffer();
			virtual ~CITcpBuffer();

			/* interface */
			virtual TcpErrorCode send(char *data, int len)=0;
			virtual TcpErrorCode read(char *data, unsigned int dataSize, unsigned int *bytesRead)=0;    
		private:
			// another msvc fix
			#ifdef LINUX
				CITcpBuffer(const CITcpBuffer& copyFromMe)=delete;
				CITcpBuffer& operator=(const CITcpBuffer& copyFromMe)=delete;
			#endif

		};
	}
}
