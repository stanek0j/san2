

#include "abstractclientreceiver.hpp"

namespace CPPL
{
	class ClientReceiver : public AbstractClientReceiver
	{
	  public:
		ClientReceiver();
		ErrorCode run();
	  protected:
		~ClientReceiver();
	  private:
		 // another msvc fix
		#ifdef LINUX
			ClientReceiver(const ClientReceiver& copyFromMe)=delete;
			ClientReceiver& operator=(const ClientReceiver& copyFromMe)=delete;
		#endif
	};
}
