
#include <algorithm>
#include <assert.h>
#include "alivefuncout.hpp"

namespace San2 { namespace Interfaces {

AliveFuncOut::AliveFuncOut()
{
	
}

unsigned int AliveFuncOut::getUniqueId()const
{
	return 2; // TODO: replace by global define file of func ids
}

bool AliveFuncOut::pack(San2::Utils::bytes &out)
{
	return true;
}


}} // ns
