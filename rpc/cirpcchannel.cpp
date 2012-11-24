
#include "cirpcchannel.hpp"
#include "utils/platform/basictypes.hpp"
#include "utils/log.h"

#define SAN_RPC_MAX_FID 65000

namespace San2
{
	namespace Rpc
	{
		bool CIRpcChannel::sendData(unsigned int funcId, const San2::Utils::bytes &data)
		{
			if (funcId > SAN_RPC_MAX_FID) return false;
			SAN_UINT16 fid = funcId;
			fid = San2::Utils::Endian::san_u_htobe16(fid);
			char tmp[sizeof(SAN_UINT16)];
			memcpy(tmp, &fid, sizeof(SAN_UINT16));
			
			San2::Utils::bytes b(tmp, tmp + sizeof(SAN_UINT16));
			b += data;
			return sendDatagram(b);
		}
		
		bool CIRpcChannel::recvData(unsigned int &funcIdOut, San2::Utils::bytes &out, unsigned int timeoutMsec)
		{
			if (!recvDatagram(out, timeoutMsec)) return false;
			if (out.size() < sizeof(SAN_UINT16)) return false;
			
			SAN_UINT16 fid;
			memcpy(&fid, out.toArray(), sizeof(SAN_UINT16)); // ugly
			fid = San2::Utils::Endian::san_u_be16toh(fid);
			funcIdOut = fid;
			
			out.erase(out.begin(), out.begin() + sizeof(SAN_UINT16));
			return true;
		}
	}
}
