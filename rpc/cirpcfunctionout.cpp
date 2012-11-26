
#include "cirpcfunctionout.hpp"

namespace San2
{
	namespace Rpc
	{
			bool CIRpcFunctionOut::operator() (const CIRpcFunctionOut& x, const CIRpcFunctionOut& y) const
			{
				return x.getUniqueId() < y.getUniqueId();
			}
	}
}
