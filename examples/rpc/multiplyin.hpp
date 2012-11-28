
#include "rpc/cirpcsyncfunctionin.hpp"
#include "utils/platform/basictypes.hpp"

class MultiplyIn : public San2::Rpc::CIRpcSyncFunctionIn
{
public:
	MultiplyIn();
	unsigned int getUniqueId()const;
	bool operator()(void);
	bool unpack(const San2::Utils::bytes &in);
	San2::Utils::bytes getResponse();

	SAN_INT32 m_x, m_y;
private:
    San2::Utils::bytes m_response;
};
