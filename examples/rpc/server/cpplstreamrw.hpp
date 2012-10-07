
#pragma once

#include "utils/cvector.hpp"
#include "cppl/bufferprocessor.hpp"
#include "stream/cistreamrw.hpp"

class CpplStreamRW : public San2::Stream::CIStreamRW
{	
public:	
	CpplStreamRW(const unsigned int maxSingleReadSize, San2::Cppl::BufferProcessor *bp);
	// WARNING: data MUST be APPENDED at the END of vector!!!
	// this is a performance enhancement
	// PARAMETERS: data - read bytes will be appended at the end of vector
	//			   maxcount - maximum numbers that will be read - MUST be greater than 0
	// RETURN VALUE: number of bytes read, -1 if maxCount==0, or -1 on error
	int readSomeAppend(San2::Utils::bytes &data, unsigned int maxCount);		
	
	// RETURN VALUE: number of bytes read, -1 on error, 0 on timeout
	int writeSome(San2::Utils::bytes::const_iterator first, San2::Utils::bytes::const_iterator last);
	
private:
	San2::Cppl::BufferProcessor *m_bp;
	const unsigned int m_maxSingleReadSize;
};
