#pragma once

#include "cppl/pipechannel.hpp"

class CChannel : public San2::Cppl::PipeChannel
{
  public:

	CChannel(CPPL_PIPETYPE handle, unsigned int timRX, unsigned int timTX);
	virtual ~CChannel();
	San2::Cppl::ErrorCode receive();

	private:

	 // another msvc fix
	#ifdef LINUX
		CChannel(const CChannel& copyFromMe)=delete;
		CChannel& operator=(const CChannel& copyFromMe)=delete;
	#endif
	
};
