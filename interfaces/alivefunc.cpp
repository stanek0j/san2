
#include <algorithm>
#include <assert.h>
#include "alivefunc.hpp"

namespace San2 { namespace Interfaces {

AliveFunc::AliveFunc()
{
	
}

unsigned int AliveFunc::getUniqueId()const
{
	return 2; // TODO: replace by global define file of func ids
}

bool AliveFunc::operator()(void)
{	
	// intentionaly empty
	return true;
}

bool AliveFunc::pack(San2::Utils::bytes &out)
{
	return true;
}

bool AliveFunc::unpack(const San2::Utils::bytes &in)
{
	return true;
}

}} // ns
