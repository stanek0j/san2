
#pragma once

#include "abstractclientreceiver.hpp"

class TerminalReceiver : public CPPL::AbstractClientReceiver
{
  public:
	TerminalReceiver();
	CPPL::ErrorCode run();
  protected:
	~TerminalReceiver();
  private:
	 // another msvc fix
	#ifdef LINUX
		TerminalReceiver(const TerminalReceiver& copyFromMe)=delete;
		TerminalReceiver& operator=(const TerminalReceiver& copyFromMe)=delete;
	#endif
};

