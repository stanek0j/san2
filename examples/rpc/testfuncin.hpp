
#include "rpc/cirpcfunctionin.hpp"

class TestFuncIn : public San2::Rpc::CIRpcFunctionIn
{
public:
	TestFuncIn();
	unsigned int getUniqueId()const;
	bool operator()(void);
	bool unpack(const San2::Utils::bytes &in);
};
