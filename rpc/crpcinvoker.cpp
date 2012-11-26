
#include <utility>
#include "crpcinvoker.hpp"
#include "utils/log.h"

namespace San2
{
	namespace Rpc
	{
		CRpcInvoker::CRpcInvoker(CIRpcChannel& channel, unsigned int timRX) :
			m_channel(channel),
			m_timRX(timRX)
		{
		
		}
		
		bool CRpcInvoker::invokeFunction(CIRpcFunctionOut& func)
		{
			San2::Utils::bytes b;
			if (!func.pack(b))
			{
				printf("Func pack failed\n");
				return false;
			}
			
			return m_channel.sendData(func.getUniqueId(), b);
		}	
		
		bool CRpcInvoker::registerFunction(std::function<CIRpcFunctionOut* (void)> createFunction)
		{
			std::shared_ptr<CIRpcFunctionOut> sharedFuncPtr(createFunction()); // must be on separate line!!! otherwise mem leak
			std::pair<std::map<unsigned int, std::shared_ptr<CIRpcFunctionOut> >::iterator, bool> ret;
			ret = m_functions.insert(std::pair<unsigned int, std::shared_ptr<CIRpcFunctionOut> > (sharedFuncPtr->getUniqueId(), sharedFuncPtr));
			return ret.second;	
		}
		
		
		
		bool CRpcInvoker::findFunction(unsigned int uniqueId, std::shared_ptr<CIRpcFunctionOut> &function)
		{
			std::map<unsigned int, std::shared_ptr<CIRpcFunctionOut> >::iterator it;
			it = m_functions.find(uniqueId);
			if (it == m_functions.end()) return false; // function not found
			function = it->second;
			return true; // ok, found
		}
	}
}
