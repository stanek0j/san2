
#include <algorithm>
#include "tcpstreamrw.hpp"
#include "utils/log.h"

namespace San2 { namespace Comm {

TcpStreamRW::TcpStreamRW(const unsigned int maxSingleReadSize, San2::Tcp::CITcpBuffer *bp) :
	San2::Stream::CIStreamRW(maxSingleReadSize),
	m_bp(bp),
	m_maxSingleReadSize(maxSingleReadSize)
{
    // No, this is a msvc fix. (MSVC does not support variable length arrays)
	rsapBuffer = (char *) malloc(sizeof(char) * m_maxSingleReadSize);
}

TcpStreamRW::~TcpStreamRW()
{
    // No, this is a msvc fix. (MSVC does not support variable length arrays)
    if (rsapBuffer != NULL) free(rsapBuffer);
}

// It is guaranteed by base interface class that (maxCount <= maxSingleReadSize)
int TcpStreamRW::readSomeAppend(San2::Utils::bytes &data, unsigned int maxCount)
{
	unsigned int bytesRead;
	San2::Tcp::TcpErrorCode ret = m_bp->read(rsapBuffer, maxCount, &bytesRead); // Very ugly maxCount
	
	if (ret != San2::Tcp::TcpErrorCode::SUCCESS)
	{ 
        FILE_LOG(logDEBUG4) << "FAIL: TcpStreamRW::readSomeAppend():1:errcode:" << San2::Tcp::errorCodeToInt(ret);
		return -1;
	}
	if (bytesRead <= 0)
	{ 
		FILE_LOG(logDEBUG4) <<  "FAIL: TcpStreamRW::readSomeAppend::2";
		return -1;
	}
	
	std::copy(rsapBuffer, rsapBuffer + bytesRead, std::back_inserter(data)); // append
	return bytesRead;
}

// RETURN VALUE: number of bytes read, -1 on error, 0 on timeout
int TcpStreamRW::writeSome(San2::Utils::bytes::const_iterator first, San2::Utils::bytes::const_iterator last)
{
	unsigned int dataSize = last - first;
	if (m_bp->send((char *)&(*first), dataSize) == San2::Tcp::TcpErrorCode::SUCCESS) // ugly!
		return dataSize;
	else return -1;
}

}} // ns
