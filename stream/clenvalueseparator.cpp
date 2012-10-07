
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
			San2::Utils::bytes btmp;
			if (!(m_rw.readExactNumBytesAppend(btmp, sizeof(SAN_UINT32))))
			{
				printf("FAIL:CLenValueSeparator::readMessage()::1\n");
				return false;
			}
			
			
			SAN_UINT32 iSize;
			memcpy(&iSize, btmp.toArray(), sizeof(SAN_UINT32));			
			iSize = San2::Utils::Endian::san_u_be32toh(iSize);
			
			printf("size is: %d\n", iSize);
			
			if (!(m_rw.readExactNumBytesAppend(out, iSize)))
			{
				printf("FAIL:CLenValueSeparator::readMessage()::2\n");
				return false;
			}
			
			return true;
		}
		
		bool CLenValueSeparator::writeMessage(const San2::Utils::bytes& in)
		{
			SAN_UINT32 iMessageSize = San2::Utils::Endian::san_u_htobe32(in.size());
			char tmpMessageSize[sizeof(SAN_UINT32)];
			memcpy(tmpMessageSize, &iMessageSize, sizeof(SAN_UINT32));
			
			San2::Utils::bytes b;
			std::copy(tmpMessageSize, tmpMessageSize + sizeof(SAN_UINT32), std::back_inserter(b));
			std::copy(in.begin(), in.end(), std::back_inserter(b));
			printf("total size: %d\n", b.size());
			return m_rw.writeAll(b);
		}
	}
	
}
