
#include <algorithm>
#include <assert.h>
#include "sendcapsulefuncin.hpp"
#include "utils/cdatapack.hpp"
#include "network/ccapsule.hpp"
#include "network/nettypedef.hpp"
#include "utils/log.h"

namespace San2 { namespace Interfaces {

const int SendCapsuleFuncIn::timeoutMilisec = 3000;

SendCapsuleFuncIn::SendCapsuleFuncIn(const San2::Network::SanAddress &interfaceAddress, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > *inputQueue, San2::Utils::CThread *thr) :
	m_interfaceAddress(interfaceAddress),
	m_inputQueue(inputQueue),
	m_thr(thr)
{
	
}

unsigned int SendCapsuleFuncIn::getUniqueId() const
{
	return 1; // TODO: replace by global define file of func ids
}

bool SendCapsuleFuncIn::operator()(void)
{	
	std::shared_ptr<San2::Network::CCapsule> capsule(new San2::Network::CCapsule);
	if (capsule->unpack(m_SerializedCapsule) != true)
	{
		 FILE_LOG(logDEBUG3) << "SendCapsuleFunc::operator(): cannot unpack capsule\n";
		 return false;
	}
	
	capsule->setFromInterfaceAddress(m_interfaceAddress);
	assert((*m_inputQueue).push(capsule, m_thr, timeoutMilisec) == 0);
	return true;
}

bool SendCapsuleFuncIn::unpack(const San2::Utils::bytes &in)
{
	m_SerializedCapsule = in;
	return true;
}

}} // ns
