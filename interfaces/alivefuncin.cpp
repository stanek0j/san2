
#include <algorithm>
#include <assert.h>
#include "alivefuncin.hpp"

namespace San2 { namespace Interfaces {

AliveFuncIn::AliveFuncIn()
{
	
}

unsigned int AliveFuncIn::getUniqueId()const
{
	return 2; // TODO: replace by global define file of func ids
}

bool AliveFuncIn::operator()(void)
{	
	// intentionaly empty
	return true;
}

bool AliveFuncIn::unpack(const San2::Utils::bytes &in)
{
	return true;
}

}} // ns
