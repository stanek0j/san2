
#pragma once

#include <iterator>
#include "cimessageseparator.hpp"
#include "utils/platform/basictypes.hpp"

// ze by RPC predelat na streamove zpracovani???
// dost sileny
// kapsule se musi stejne nejdriv nacist cela do pameti, aby se mohla
// spustit. Nebo by to tak alespon melo byt.?

namespace San2
{
	namespace Stream
	{
		template<class CStreamRWDerived>
		class CLenValueSeparator : public CIMessageSeparator<CStreamRWDerived> // protected or private? I think protected.
		{
		public:
			CLenValueSeparator(const unsigned int maxSingleReadSize) :
				CLenValueSeparator<CStreamRWDerived>(maxSingleReadSize)
			{
				
			}
			
			virtual ~CLenValueSeparator(){}
			
			bool readOneMessage(San2::utils::bytes &out)
			{
				
			}
			
			bool writeOneMessage(San2::Utils::bytes::const_iterator first, San2::Utils::bytes::const_iterator last)
			{
				SAN_UINT32 iMessageSize = last - first; // so nice
				iMessageSize = San2::Utils::Endian::be32toh(iMessageSize);
				char tmpMessageSize[sizeof(SAN_UINT32)];
				memcpy(tmpMessageSize, &iMessageSize, sizeof(SAN_UINT32));
				
				San2::Utils::bytes b;
				std::copy(tmpMessageSize, tmpMessageSize + sizeof(SAN_UINT32), std::back_inserter(b));
				std::copy(first, last, std::back_inserter(b));
				return writeAll(b.first(), b.last());
			}
			
			// another msvc fix
		    #ifdef LINUX
			    CLenValueSeparator(const CLenValueSeparator& copyFromMe)=delete;
			    CLenValueSeparator& operator=(const CLenValueSeparator& copyFromMe)=delete;
		    #endif
		};
	}
}
