
#include <algorithm>
#include "cpplstreamrw.hpp"

CpplStreamRW::CpplStreamRW(const unsigned int maxSingleReadSize, San2::Cppl::BufferProcessor *bp) :
	San2::Stream::CIStreamRW(maxSingleReadSize)
	m_bp(bp)
{
	
}

// It is guaranteed by base interface class that (maxCount <= maxSingleReadSize)
int CpplStreamRW::readSomeAppend(San2::Utils::bytes &data, unsigned int maxCount)
{
	unsigned int bytesRead;
	char buffer[maxSingleRead];
	San2::Cppl::ErrorCode ret = m_bp->readSome(buffer, maxCount, &bytesRead); // Very ugly maxCount
	
	if (ret != San2::Cppl::ErrorCode::SUCCESS) return -1;
	if (bytesRead <= 0) return -1;
	std::copy(buffer, buffer + bytesRead, std::back_inserter(data)); // append
	return bytesRead;
}

// RETURN VALUE: number of bytes read, -1 on error, 0 on timeout
int CpplStreamRW::writeSome(const San2::Utils::bytes &data)
{
	if (m_bp->send(data.toArray(), data.size()) == San2::Cppl::ErrorCode::SUCCESS) return data.size();
	else return -1;
}
