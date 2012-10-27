
#include "clenvalueseparator.hpp"
#include "utils/platform/basictypes.hpp"
#include "utils/log.h"

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
				FILE_LOG(logDEBUG3) << "FAIL:CLenValueSeparator::readMessage()::1";
				return false;
			}
			
			
			SAN_UINT32 iSize;
			memcpy(&iSize, btmp.toArray(), sizeof(SAN_UINT32));			
			iSize = San2::Utils::Endian::san_u_be32toh(iSize);
			
			if (!(m_rw.readExactNumBytesAppend(out, iSize)))
			{
				FILE_LOG(logDEBUG3) << "FAIL:CLenValueSeparator::readMessage()::2";
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
			return m_rw.writeAll(b);
		}
	}
	
}
