
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
				r = readSomeAppend(out, num >= m_maxSingleReadSize ? m_maxSingleReadSize : num);
				if (r <= 0) return false;
				num -= r;
			}
			return true;
		}
			
		// TRUE = success
		bool CIStreamRW::writeAll(San2::Utils::bytes::const_iterator first, San2::Utils::bytes::const_iterator last)
		{
			int n;	
			while(first != last) 
			{
				n = writeSome(first, last);
				if (n <= 0) return false;
				first += n;
			}

			return true;
		}
	}
}
