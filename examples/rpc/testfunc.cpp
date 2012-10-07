
#include "testfunc.hpp"

unsigned int TestFunc::getUniqueId()const
{
	return 123; // TODO: replace by global define file of func ids
}

bool TestFunc::operator()(void)
{
	printf("Rpc Test function SUCCESS\n");
}

bool TestFunc::pack(San2::Utils::bytes &out)
{
	return true;
}

bool TestFunc::unpack(const San2::Utils::bytes &in)
{
	return true;
}
