#include "rpc/cirpcfunctionout.hpp"

class TestFuncOut : public San2::Rpc::CIRpcFunctionOut
{
public:
	TestFuncOut();
	unsigned int getUniqueId()const;
	bool pack(San2::Utils::bytes &out);
};
