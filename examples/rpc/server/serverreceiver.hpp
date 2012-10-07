
#pragma once
#include "cppl/abstractreceiver.hpp"

class ServerReceiver : public San2::Cppl::AbstractReceiver
{
  public:
	ServerReceiver();
	
  protected:
	~ServerReceiver();
	ErrorCode run();
  private:
	CRpcExecutor exec;
	
	
	 // another msvc fix
	#ifdef LINUX
		ServerReceiver(const ServerReceiver& copyFromMe)=delete;
		ServerReceiver& operator=(const ServerReceiver& copyFromMe)=delete;
	#endif
	
};
