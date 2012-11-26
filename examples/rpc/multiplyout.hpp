
#include "rpc/cirpcfunctionout.hpp"
#include "utils/platform/basictypes.hpp"

class MultiplyOut : public San2::Rpc::CIRpcFunctionOut
{
public:
	MultiplyOut();
    MultiplyOut(int x, int y);
	unsigned int getUniqueId()const;	
	bool pack(San2::Utils::bytes &out);

	SAN_INT32 m_x, m_y;
private:

};
