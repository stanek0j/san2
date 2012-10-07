
#pragma once

#include "utils/cvector.hpp"
#include "cistreamrw.hpp"

// ze by RPC predelat na streamove zpracovani???
// dost sileny
// kapsule se musi stejne nejdriv nacist cela do pameti, aby se mohla
// spustit. Nebo by to tak alespon melo byt.?

namespace San2
{
	namespace Stream
	{
		class CIMessageSeparator
		{
		public:
			virtual ~CIMessageSeparator(){}
			
			virtual bool readMessage(San2::Utils::bytes &out)=0;
			virtual bool writeMessage(const San2::Utils::bytes& in)=0;
			
			// another msvc fix
		    #ifdef LINUX
			    CIMessageSeparator(const CIMessageSeparator& copyFromMe)=delete;
			    CIMessageSeparator& operator=(const CIMessageSeparator& copyFromMe)=delete;
		    #endif
		};
	}
}
