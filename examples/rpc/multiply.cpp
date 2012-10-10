
#include <algorithm>
#include "multiply.hpp"
#include "utils/cdatapack.hpp"

Multiply::Multiply()
{
	
}

Multiply::Multiply(int x, int y) :
	m_x(x),
	m_y(y)
{
	
}

unsigned int Multiply::getUniqueId()const
{
	return 124; // TODO: replace by global define file of func ids
}

bool Multiply::operator()(void)
{
	printf("Multiply: %d * %d = %d\n", m_x, m_y, m_x * m_y);
	return true;
}

bool Multiply::pack(San2::Utils::bytes &out)
{
	out = San2::Utils::CDataPack::pack(m_x) + San2::Utils::CDataPack::pack(m_y);
	return true;
}

bool Multiply::unpack(const San2::Utils::bytes &in)
{
	if (in.size() != 2 * sizeof(SAN_INT32)) return false;
	m_x = San2::Utils::CDataPack::unpackInt32(in, 0);
	m_y = San2::Utils::CDataPack::unpackInt32(in, sizeof(SAN_INT32));
	return true;
}
