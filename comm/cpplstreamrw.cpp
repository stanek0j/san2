
#include <algorithm>
#include "cpplstreamrw.hpp"

namespace San2 { namespace Comm {

CpplStreamRW::CpplStreamRW(const unsigned int maxSingleReadSize, San2::Cppl::BufferProcessor *bp) :
	San2::Stream::CIStreamRW(maxSingleReadSize),
	m_bp(bp),
	m_maxSingleReadSize(maxSingleReadSize)
{
	
}

// It is guaranteed by base interface class that (maxCount <= maxSingleReadSize)
int CpplStreamRW::readSomeAppend(San2::Utils::bytes &data, unsigned int maxCount)
{
	unsigned int bytesRead;
	char buffer[m_maxSingleReadSize];
	San2::Cppl::ErrorCode ret = m_bp->readSome(buffer, maxCount, &bytesRead); // Very ugly maxCount
	
	if (ret != San2::Cppl::ErrorCode::SUCCESS) return -1;
	if (bytesRead <= 0) return -1;
	std::copy(buffer, buffer + bytesRead, std::back_inserter(data)); // append
	return bytesRead;
}

// RETURN VALUE: number of bytes read, -1 on error, 0 on timeout
int CpplStreamRW::writeSome(San2::Utils::bytes::const_iterator first, San2::Utils::bytes::const_iterator last)
{
	unsigned int dataSize = last - first;
	if (m_bp->send((char *)&(*first), dataSize) == San2::Cppl::ErrorCode::SUCCESS) // ugly!
		return dataSize;
	else return -1;
}

}} // ns
