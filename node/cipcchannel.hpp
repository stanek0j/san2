#pragma once

#include "cppl/pipechannel.hpp"
#include "cnode.hpp"

namespace San2 
{
	namespace Node
	{
		class CIpcChannel : public San2::Cppl::PipeChannel
		{
		  public:

			CIpcChannel(CPPL_PIPETYPE handle, unsigned int timRX, unsigned int timTX, CNode &node);
			virtual ~CIpcChannel();
			San2::Cppl::ErrorCode receive();

			private:

			 // another msvc fix
			#ifdef LINUX
				CIpcChannel(const CIpcChannel& copyFromMe)=delete;
				CIpcChannel& operator=(const CIpcChannel& copyFromMe)=delete;
			#endif
			
			CNode &m_node;
		};
	}
}
