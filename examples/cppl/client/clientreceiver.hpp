

#include "cppl/abstractreceiver.hpp"

namespace San2
{
	namespace Cppl
	{
		class ClientReceiver : public AbstractReceiver
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
}
