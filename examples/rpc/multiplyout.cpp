
#include <algorithm>
#include "examples/rpc/multiplyout.hpp"
#include "utils/cdatapack.hpp"

MultiplyOut::MultiplyOut()
{
	
}

MultiplyOut::MultiplyOut(int x, int y) :
    m_x(x),
    m_y(y)
{

}

unsigned int MultiplyOut::getUniqueId()const
{
	return 124; // TODO: replace by global define file of func ids
}

bool MultiplyOut::pack(San2::Utils::bytes &out)
{
	out = San2::Utils::CDataPack::pack(m_x) + San2::Utils::CDataPack::pack(m_y);
	return true;
}

SAN_INT32 MultiplyOut::getResult()
{
    return m_result;
}


bool MultiplyOut::parseResponse(const San2::Utils::bytes &in)
{
    m_result = San2::Utils::CDataPack::unpackInt32(in, 0);
    return true;
}
