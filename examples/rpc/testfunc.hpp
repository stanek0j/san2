
#include "rpc/cirpcfunction.hpp"

class TestFunc : public San2::Rpc::CIRpcFunction
{
public:
	TestFunc();
	unsigned int getUniqueId()const;
	bool operator()(void);
	
	bool pack(San2::Utils::bytes &out);
	bool unpack(const San2::Utils::bytes &in);
};
