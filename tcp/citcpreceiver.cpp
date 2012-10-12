
#include <stdlib.h>

#include "citcpreceiver.hpp"
#include "tcphelper.hpp"

namespace San2 { namespace Tcp {
	
	CITcpReceiver::CITcpReceiver()
	{
		// empty	
	}
		
	CITcpReceiver::~CITcpReceiver()
	{
		// empty
	}
	
	void CITcpReceiver::disconnected(TcpErrorCode errcode)
	{
		// empty
	}
	
	
	CITcpBuffer* CITcpReceiver::getBufferProcessor()
	{
		return m_bp;
	}
	
	/*
	TpcErrorCode CITcpReceiver::send(char *data, int len)
	{
		return m_bp->send(data, len);
	}
	*/
	
}} //ns
