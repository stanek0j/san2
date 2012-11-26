
#include "testfuncin.hpp"

TestFuncIn::TestFuncIn()
{
	
}

unsigned int TestFuncIn::getUniqueId()const
{
	return 123; // TODO: replace by global define file of func ids
}

bool TestFuncIn::operator()(void)
{
	printf("Rpc Test function SUCCESS\n");
	return true;
}

bool TestFuncIn::unpack(const San2::Utils::bytes &in)
{
	return true;
}
