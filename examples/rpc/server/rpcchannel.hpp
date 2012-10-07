
#pragma once

#include "rpc/cirpcchannel.hpp"
#include "cpplstreamrw.hpp"

class RpcChannel : public San2::Rpc::CIRpcChannel
{
public:
	RpcChannel(San2::Cppl::BufferProcessor *bp);
	// destructor must be virtual!
	virtual ~CIRpcChannel(){ /* intentionally empty */ };

	// TRUE = success
	virtual bool sendDatagram(const San2::Utils::bytes &data);
	virtual bool recvDatagram(San2::Utils::bytes &out, unsigned int timeoutMsec);

private:
	San2::Cppl::BufferProcessor *m_bp;
	CLenValueSeparator<CpplStreamRW> m_lvsep;
};
