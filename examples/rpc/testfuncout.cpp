
#include "testfuncout.hpp"

TestFuncOut::TestFuncOut()
{
	
}

unsigned int TestFuncOut::getUniqueId()const
{
	return 123; // TODO: replace by global define file of func ids
}

bool TestFuncOut::pack(San2::Utils::bytes &out)
{
	return true;
}
