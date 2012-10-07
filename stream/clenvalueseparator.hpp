
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
		class CLenValueSeparator : public CIMessageSeparator
		{
		public:
			CLenValueSeparator(CIStreamRW &rw);
			virtual ~CLenValueSeparator();			
			bool readMessage(San2::Utils::bytes &out);
			bool writeMessage(const San2::Utils::bytes& in);
			
			// another msvc fix
		    #ifdef LINUX
			    CLenValueSeparator(const CLenValueSeparator& copyFromMe)=delete;
			    CLenValueSeparator& operator=(const CLenValueSeparator& copyFromMe)=delete;
		    #endif
		protected:
			CIStreamRW &m_rw;
		};
	}
}
