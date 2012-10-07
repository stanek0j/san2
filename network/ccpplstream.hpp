
#pragma once

// ze by RPC predelat na streamove zpracovani???
// dost sileny
// kapsule se musi stejne nejdriv nacist cela do pameti, aby se mohla
// spustit. Nebo by to tak alespon melo byt.?

#include "utils/cvector.hpp"

namespace San2
{
	namespace Network
	{
		class CCpplStream : public CIStreamRW
		{
		public:
			CCpplStream(const unsigned int maxSingleReadSize);
			virtual ~CCpplStream();
			virtual int readSomeAppend(San2::Utils::bytes &data, unsigned int maxCount);
			virtual int writeSome(San2::Utils::bytes::const_iterator first, San2::Utils::bytes::const_iterator last);
		};
	}
}
