
#include "rpc/cirpcsyncfunctionout.hpp"
#include "utils/platform/basictypes.hpp"

class MultiplyOut : public San2::Rpc::CIRpcSyncFunctionOut
{
public:
	MultiplyOut();
    MultiplyOut(int x, int y);
	unsigned int getUniqueId()const;	
	bool pack(San2::Utils::bytes &out);
    bool parseResponse(const San2::Utils::bytes &in);

    SAN_INT32 getResult();

	SAN_INT32 m_x, m_y;
    SAN_INT32 m_result; // calculated remotely
private:

};
