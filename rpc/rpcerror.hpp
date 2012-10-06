
#pragma once

namespace San2
{
	namespace Rpc
	{
		enum class RpcError : int
		{ 
			SUCCESS = 0, 
			FAILURE = -1,
			TERMINATED = -2,
			TIMEOUT = -3, 
			FUNCTION_NOT_FOUND = -4,
			UNPACK_ERROR = -5,
			PACK_ERROR = -6
		};
	}
}
