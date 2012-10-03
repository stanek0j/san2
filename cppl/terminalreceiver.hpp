
#pragma once

#include "abstractclientreceiver.hpp"

namespace San2 { namespace Cppl {
    class TerminalReceiver : public AbstractClientReceiver
    {
      public:
	    TerminalReceiver();
	    ErrorCode run();
      protected:
	    ~TerminalReceiver();
      private:
	     // another msvc fix
	    #ifdef LINUX
		    TerminalReceiver(const TerminalReceiver& copyFromMe)=delete;
		    TerminalReceiver& operator=(const TerminalReceiver& copyFromMe)=delete;
	    #endif
    };
}} //  ns
