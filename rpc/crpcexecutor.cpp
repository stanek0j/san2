
#include <utility>
#include "crpcexecutor.hpp"

namespace San2
{
	namespace Rpc
	{
		CRpcExecutor::CRpcExecutor(CIRpcChannel& channel, unsigned int timRX) :
			m_channel(channel),
			m_timRX(timRX)
		{
		
		}
		
		void CRpcExecutor::run()
		{
			printf("CRpcExecutor::run() begin\n");
			
			San2::Utils::bytes b;
			unsigned int functionId;
			
			// needs some kind of return value
			while(!isTerminated())
			{
				if (!(m_channel.recvData(functionId, b, m_timRX)))
				{
					 printf("FAIL: CRpcExecutor::run()::recvData()\n");
					 break;
				}
				if (executeFunction(functionId, b) != RpcError::SUCCESS);
			}
			printf("CRpcExecutor::run() end\n");
		}
		
		bool CRpcExecutor::registerFunction(std::function<CIRpcFunction* (void)> createFunction)
		{
			std::shared_ptr<CIRpcFunction> sharedFuncPtr(createFunction()); // must be on separate line!!! otherwise mem leak
			std::pair<std::map<unsigned int, std::shared_ptr<CIRpcFunction> >::iterator, bool> ret;
			ret = m_functions.insert(std::pair<unsigned int, std::shared_ptr<CIRpcFunction> > (sharedFuncPtr->getUniqueId(), sharedFuncPtr));
			return ret.second;	
		}
		
		RpcError CRpcExecutor::executeFunction(unsigned int uniqueId, const San2::Utils::bytes &in)
		{
			std::shared_ptr<CIRpcFunction> func;
			bool exists = findFunction(uniqueId, func);
			if (!exists) return RpcError::FUNCTION_NOT_FOUND;
			if (!func->unpack(in)) return RpcError::UNPACK_ERROR;
			(*func)(); // execute
			return RpcError::SUCCESS;
		}
		
		bool CRpcExecutor::findFunction(unsigned int uniqueId, std::shared_ptr<CIRpcFunction> &function)
		{
			std::map<unsigned int, std::shared_ptr<CIRpcFunction> >::iterator it;
			it = m_functions.find(uniqueId);
			if (it == m_functions.end()) return false; // function not found
			function = it->second;
			return true; // ok, found
		}
	}
}
