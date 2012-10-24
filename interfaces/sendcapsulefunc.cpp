
#include <algorithm>
#include "sendcapsulefunc.hpp"
#include "utils/cdatapack.hpp"
#include "network/ccapsule.hpp"

namespace San2 { namespace Interfaces {

const int SendCapsuleFunc::timeoutSec = 3;

SendCapsuleFunc::SendCapsuleFunc(San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > *inputQueue, San2::Utils::CThread *thr) :
	m_inputQueue(inputQueue),
	m_duration(timeoutSec),
	m_thr(thr)
{
	
}

void SendCapsuleFunc::setCapsuleToSend(std::shared_ptr<San2::Network::CCapsule> capsule)
{
	m_capsule = capsule;
}

unsigned int SendCapsuleFunc::getUniqueId()const
{
	return 1; // TODO: replace by global define file of func ids
}

bool SendCapsuleFunc::operator()(void)
{	
	(*m_inputQueue).push<int>(m_capsule, m_thr, m_duration);
	return true;
}

bool SendCapsuleFunc::pack(San2::Utils::bytes &out)
{
	m_capsule->pack(out); // TODO: fix rval from void to bool
	return true;
}

bool SendCapsuleFunc::unpack(const San2::Utils::bytes &in)
{
	return m_capsule->unpack(in);
}

}} // ns
