			
#include "ccapsule.hpp"
#include "utils/flag.hpp"												
			
namespace San2 { namespace Network {			
						
void CCapsule::setSourceAddress(SanAddress srcAddress)
{
	m_srcAddress = srcAddress;
}

void CCapsule::setDestinationAddress(SanAddress dstAddress)
{
	m_dstAddress = dstAddress;
}

void CCapsule::setHop(SAN_UINT16 hop)
{
	m_hop = hop;
}
						
void CCapsule::setDX(bool dx)
{
	m_flagDX = dx;
}					
					
void CCapsule::setEX(bool ex)
{
	m_flagEX = ex;
}					
	
SanAddress CCapsule::getSourceAddress()
{
	return m_srcAddress;
}

SanAddress CCapsule::getDestinationAddress()
{
	return m_dstAddress;
}

SAN_UINT16 CCapsule::getHop()
{
	return m_hop;
}

void CCapsule::setData(San2::Utils::bytes data)
{
	m_data = data; // so nice
}

const San2::Utils::bytes& CCapsule::getData()
{
	return m_data;
}

void CCapsule::pack(San2::Utils::bytes &out)
{
	out.clear();
	out.resize(minimumCapsuleHeaderSize + m_data.size());
	out.insert(out.end(), m_dstAddress.cbegin(), m_dstAddress.cend());
	out.insert(out.end(), m_srcAddress.cbegin(), m_srcAddress.cend());
	out += hop2bytes(m_hop); 
	out += constructFlags();
	out += m_data;
}

bool CCapsule::unpack(const San2::Utils::bytes &capsule)
{
	return true; // rewri
}

San2::Utils::bytes CCapsule::hop2bytes(SAN_UINT16 hop)
{
	// Ugly but i dont know, how to do it better
	SAN_UINT16 netorder;
	unsigned char tmp[sizeof(SAN_UINT16)];
	netorder = San2::Utils::Endian::san_u_htobe16(hop);
	memcpy(tmp, &netorder, sizeof(SAN_UINT16));
	San2::Utils::bytes b(tmp, tmp + sizeof(SAN_UINT16));
	return b;
}

void San2::Network::CCapsule::setApplicationId(San2::Network::SanApplicationId appId)
{
	m_appId = appId;
}

SanApplicationId CCapsule::getAppId()
{
	return m_appId;
}

unsigned char CCapsule::constructFlags()
{
	unsigned char flags = 0; // 0 - null all flags
	San2::Utils::Flag::setFlag(flags, SAN_FLAGPOS_DX_POSITION, m_flagDX);
	San2::Utils::Flag::setFlag(flags, SAN_FLAGPOS_EX_POSITION, m_flagEX);
	return flags;
}

void CCapsule::setFromInterfaceAddress(SanAddress interfaceAddress)
{
	m_interfaceAddress = interfaceAddress;
}

SanAddress CCapsule::getFromInterfaceAddress()
{
	return m_interfaceAddress;
}

}} // ns
