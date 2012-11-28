
#include <algorithm>
#include "examples/rpc/multiplyin.hpp"
#include "utils/cdatapack.hpp"

MultiplyIn::MultiplyIn()
{
	
}

unsigned int MultiplyIn::getUniqueId() const
{
	return 124; // TODO: replace by global define file of func ids
}

bool MultiplyIn::unpack(const San2::Utils::bytes &in)
{
	if (in.size() != 2 * sizeof(SAN_INT32)) return false;
	m_x = San2::Utils::CDataPack::unpackInt32(in, 0);
	m_y = San2::Utils::CDataPack::unpackInt32(in, sizeof(SAN_INT32));
	return true;
}

bool MultiplyIn::operator()(void)
{
    SAN_INT32 result = m_x * m_y;
	printf("Multiply: %d * %d = %d\n", m_x, m_y, result);
    m_response = San2::Utils::CDataPack::pack(result);
	return true;
}

San2::Utils::bytes MultiplyIn::getResponse()
{
    return m_response;
}