
#pragma once

#include "cppl/abstractreceiver.hpp"

class TerminalReceiver : public San2::Cppl::AbstractReceiver
{
  public:
    TerminalReceiver();
    San2::Cppl::ErrorCode run();
  protected:
    ~TerminalReceiver();
  private:
     // another msvc fix
    #ifdef LINUX
	    TerminalReceiver(const TerminalReceiver& copyFromMe)=delete;
	    TerminalReceiver& operator=(const TerminalReceiver& copyFromMe)=delete;
    #endif
};
