
#include "rpc/cirpcfunction.hpp"
#include "utils/platform/basictypes.hpp"

class Multiply : public San2::Rpc::CIRpcFunction
{
public:
	Multiply();
	Multiply(int x, int y);
	unsigned int getUniqueId()const;
	bool operator()(void);
	
	bool pack(San2::Utils::bytes &out);
	bool unpack(const San2::Utils::bytes &in);
	
	SAN_INT32 m_x, m_y;
private:

};
