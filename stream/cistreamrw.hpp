
#pragma once

// ze by RPC predelat na streamove zpracovani???
// dost sileny
// kapsule se musi stejne nejdriv nacist cela do pameti, aby se mohla
// spustit. Nebo by to tak alespon melo byt.?

#include "utils/cvector.hpp"

// NOTE: Member functions implemented in this class must ensure that (maxCount <= maxSingleReadSize)
// when calling readSomeAppend()!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

namespace San2
{
	namespace Stream
	{
		class CIStreamRW
		{
		public:
			CIStreamRW(const unsigned int maxSingleReadSize);
			virtual ~CIStreamRW(){};
			// interface
			
			// WARNING: data MUST be APPENDED at the END of vector!!!
			// this is a performance enhancement
			// PARAMETERS: data - read bytes will be appended at the end of vector
			//			   maxcount - maximum numbers that will be read - MUST be greater than 0
			// RETURN VALUE: number of bytes read, -1 if maxCount==0, or -1 on error
			virtual int readSomeAppend(San2::Utils::bytes &data, unsigned int maxCount)=0;
			
			// RETURN VALUE: number of bytes read, -1 on error, 0 on timeout
			virtual int writeSome(San2::Utils::bytes::const_iterator first, San2::Utils::bytes::const_iterator last)=0;
			
			// implemented in this class
			// warning: data WILL be APPENDED at the END of vector!!!
			// this is a performance enhancement
			bool readExactNumBytesAppend(San2::Utils::bytes &out, unsigned int num); // TRUE = success
			bool writeAll(const San2::Utils::bytes &data); // TRUE = success
			
			unsigned int getMaxSingleReadSize() const;
		protected:
			
		private:
			const unsigned int m_maxSingleReadSize;
		};
	}
}
