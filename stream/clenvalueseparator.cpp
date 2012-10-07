
#include "clenvalueseparator.hpp"
#include "utils/platform/basictypes.hpp"

namespace San2
{
	namespace Stream
	{
		CLenValueSeparator::CLenValueSeparator(CIStreamRW &rw) :
			m_rw(rw)
		{
			
		}
		
		CLenValueSeparator::~CLenValueSeparator()
		{
			
		}
	
		bool CLenValueSeparator::readMessage(San2::Utils::bytes &out)
		{
			// TODO: implelent
			return false;
		}
		
		bool CLenValueSeparator::writeMessage(const San2::Utils::bytes& in)
		{
			SAN_UINT32 iMessageSize = San2::Utils::Endian::san_u_be32toh(in.size());
			char tmpMessageSize[sizeof(SAN_UINT32)];
			memcpy(tmpMessageSize, &iMessageSize, sizeof(SAN_UINT32));
			
			San2::Utils::bytes b;
			std::copy(tmpMessageSize, tmpMessageSize + sizeof(SAN_UINT32), std::back_inserter(b));
			std::copy(in.begin(), in.end(), std::back_inserter(b));
			return m_rw.writeAll(b);
		}
	}
	
}
