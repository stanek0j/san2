
#pragma once

#include "cppl/pipeclient.hpp"

class TerminalReceiver : public San2::Cppl::PipeClient
{
  public:
    TerminalReceiver(const char *pipeName, unsigned int timCON, unsigned int timRX, unsigned int timTX);
    virtual ~TerminalReceiver();
    San2::Cppl::ErrorCode receive();
    
  private:
     // another msvc fix
    #ifdef LINUX
	    TerminalReceiver(const TerminalReceiver& copyFromMe)=delete;
	    TerminalReceiver& operator=(const TerminalReceiver& copyFromMe)=delete;
    #endif
};
