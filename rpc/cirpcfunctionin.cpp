
#include "cirpcfunctionin.hpp"

namespace San2
{
	namespace Rpc
	{
			bool CIRpcFunctionIn::operator() (const CIRpcFunctionIn& x, const CIRpcFunctionIn& y) const
			{
				return x.getUniqueId() < y.getUniqueId();
			}
	}
}
