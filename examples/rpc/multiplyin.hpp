
#include "rpc/cirpcfunctionin.hpp"
#include "utils/platform/basictypes.hpp"

class MultiplyIn : public San2::Rpc::CIRpcFunctionIn
{
public:
	MultiplyIn();
	unsigned int getUniqueId()const;
	bool operator()(void);
	bool unpack(const San2::Utils::bytes &in);
	
	SAN_INT32 m_x, m_y;
private:

};
