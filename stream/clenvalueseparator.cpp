
#include "clenvalueseparator.hpp"

namespace San2
{
	namespace Stream
	{
		CLenValueSeparator::CLenValueSeparator(CIStreamRW &rw) :
			m_rw(rw)
		{
			
		}
		
		virtual ~CLenValueSeparator()
		{
			
		}
	
		bool CLenValueSeparator::readMessage(San2::Utils::bytes &out)
		{
			
		}
		
		bool CLenValueSeparator::writeMessage(const San2::Utils::bytes& in)
		{
			SAN_UINT32 iMessageSize = San2::Utils::Endian::be32toh(in.size());
			char tmpMessageSize[sizeof(SAN_UINT32)];
			memcpy(tmpMessageSize, &iMessageSize, sizeof(SAN_UINT32));
			
			San2::Utils::bytes b;
			std::copy(tmpMessageSize, tmpMessageSize + sizeof(SAN_UINT32), std::back_inserter(b));
			std::copy(first, last, std::back_inserter(b));
			return m_rw.writeAll(b);
		}
	}
	
}
