
#include "cistreamrw.hpp"

namespace San2
{
	namespace Stream
	{
		CIStreamRW::CIStreamRW(const unsigned int maxSingleReadSize) :
			m_maxSingleReadSize(maxSingleReadSize)
		{
			
		}
		
		/*
		virtual ~CIStreamRW::CIStreamRW()
		{
			
		}
		* */
		
		// TRUE = success
		bool CIStreamRW::readExactNumBytesAppend(San2::Utils::bytes &out, unsigned int num) // TRUE = success
		{
			int r;
			while(num > 0)
			{
				// NOTE: Member functions implemented in this class must ensure that (maxCount <= maxSingleReadSize)
				// when calling readSomeAppend()!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				r = readSomeAppend(out, num >= m_maxSingleReadSize ? m_maxSingleReadSize : num);
				if (r <= 0) return false;
				num -= r;
			}
			return true;
		}
			
		// TRUE = success
		bool CIStreamRW::writeAll(const San2::Utils::bytes &data)
		{
			int n;	
			
			San2::Utils::bytes::const_iterator first = data.begin();
			San2::Utils::bytes::const_iterator lasr = data.end();
			
			while(first != last) 
			{
				n = writeSome(first, last);
				if (n <= 0) return false;
				first += n;
			}

			return true;
		}
		
		unsigned int CIStreamRW::getMaxSingleReadSize() const
		{
			return m_maxSingleReadSize;
		}
	}
}
