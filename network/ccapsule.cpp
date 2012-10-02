			
#include "ccapsule.hpp"
			
void San2::Network::CCapsule::setSourceAddress(const San2::Network::SanAddress &srcAddress)
{
	m_srcAddress = srcAddress;
}

void San2::Network::CCapsule::setDestinationAddress(const San2::Network::SanAddress &dstAddress)
{
	m_dstAddress = dstAddress;
}

void San2::Network::CCapsule::setHopLimit(const San2::Network::SanHops &maxHops)
{
	m_hopLimit = maxHops;
}
						
San2::Network::SanAddress San2::Network::CCapsule::getSourceAddress()
{
	return m_srcAddress;
}

San2::Network::SanAddress San2::Network::CCapsule::getDestinationAddress()
{
	return m_dstAddress;
}

San2::Network::SanHops San2::Network::CCapsule::getHopLimit()
{
	return m_hopLimit;
}


//|DX|EX|0|0|0|0|0|0|			
void San2::Network::CCapsule::setDX(bool dx)
{
	if (dx) m_flags |= 0x80;
	else m_flags &= 0x7F;
}

void San2::Network::CCapsule::setEX(bool ex)
{
	if (ex) m_flags |= 0x40;
	else m_flags &= 0xBF;
}
