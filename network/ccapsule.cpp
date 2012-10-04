			
#include "ccapsule.hpp"
						
void San2::Network::CCapsule::setSourceAddress(const San2::Network::SanAddress &srcAddress)
{
	m_srcAddress = srcAddress;
}

void San2::Network::CCapsule::setDestinationAddress(const San2::Network::SanAddress &dstAddress)
{
	m_dstAddress = dstAddress;
}

void San2::Network::CCapsule::setHop(SAN_UINT16 hop)
{
	m_hop = hop;
}
						
void San2::Network::CCapsule::setDX(bool dx)
{
	m_flagDX = dx;
}					
					
void San2::Network::CCapsule::setEX(bool ex)
{
	m_flagEX = ex;
}					
	
San2::Network::SanAddress San2::Network::CCapsule::getSourceAddress()
{
	return m_srcAddress;
}

San2::Network::SanAddress San2::Network::CCapsule::getDestinationAddress()
{
	return m_dstAddress;
}

SAN_UINT16 San2::Network::CCapsule::getHop()
{
	return m_hop;
}

void San2::Network::CCapsule::setData(const San2::Utils::bytes &data)
{
	m_data = data; // so nice
}

const San2::Utils::bytes& San2::Network::CCapsule::getData()
{
	return m_data;
}

void San2::Network::CCapsule::pack(San2::Utils::bytes &out)
{
	out.clear();
	out.resize(minimumCapsuleHeaderSize + m_data.size());
	out.insert(out.end(), m_dstAddress.cbegin(), m_dstAddress.cend());
	out.insert(out.end(), m_srcAddress.cbegin(), m_srcAddress.cend());
	out += hop2bytes(m_hop); 
	// flags
	out += m_data;
}

bool San2::Network::CCapsule::unpack(const San2::Utils::bytes &capsule)
{
	return true; // rewri
}

San2::Utils::bytes hop2bytes(SAN_UINT16 hop)
{
	// Ugly but i dont know, how to do it better
	SAN_UINT16 netorder;
	unsigned char tmp[sizeof(SAN_UINT16)];
	netorder = San2::Utils::Endian::san_u_htobe16(hop);
	memcpy(tmp, &netorder, sizeof(SAN_UINT16));
	San2::Utils::bytes b(tmp, tmp + sizeof(SAN_UINT16));
	return b;
}

void San2::Network::CCapsule::setApplicationId(const San2::Network::SanApplicationId &appId)
{
	m_appId = appId;
}

San2::Network::SanApplicationId San2::Network::CCapsule::getAppId()
{
	return m_appId;
}

/*
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
*/
