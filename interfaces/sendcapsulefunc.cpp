
#include <algorithm>
#include <assert.h>
#include "sendcapsulefunc.hpp"
#include "utils/cdatapack.hpp"
#include "network/ccapsule.hpp"
#include "network/nettypedef.hpp"
#include "utils/log.h"

namespace San2 { namespace Interfaces {

const int SendCapsuleFunc::timeoutMilisec = 3000;

SendCapsuleFunc::SendCapsuleFunc(const San2::Network::SanAddress &interfaceAddress, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > *inputQueue, San2::Utils::CThread *thr) :
	m_interfaceAddress(interfaceAddress),
	m_inputQueue(inputQueue),
	m_thr(thr)
{
	
}

bool SendCapsuleFunc::setCapsuleToSend(std::shared_ptr<San2::Network::CCapsule> capsule)
{
	capsule->pack(m_SerializedCapsule);
	return true;
}

unsigned int SendCapsuleFunc::getUniqueId()const
{
	return 1; // TODO: replace by global define file of func ids
}

bool SendCapsuleFunc::operator()(void)
{	
	std::shared_ptr<San2::Network::CCapsule> capsule(new San2::Network::CCapsule);
	if (capsule->unpack(m_SerializedCapsule) != true)
	{
		 FILE_LOG(logWARNING) << "SendCapsuleFunc::operator(): cannot unpack capsule\n";
		 return false;
	}
	
	capsule->setFromInterfaceAddress(m_interfaceAddress);
	assert((*m_inputQueue).push(capsule, m_thr, timeoutMilisec) == 0);
	return true;
}

bool SendCapsuleFunc::pack(San2::Utils::bytes &out)
{
	out = m_SerializedCapsule;
	return true;
}

bool SendCapsuleFunc::unpack(const San2::Utils::bytes &in)
{
	m_SerializedCapsule = in;
	return true;
}

}} // ns
