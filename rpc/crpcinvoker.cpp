
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

        bool CRpcInvoker::invokeSyncFunction(CIRpcSyncFunctionOut& syncFunc)
		{
			San2::Utils::bytes b;
			if (!syncFunc.pack(b))
			{
				printf("Func pack failed\n");
				return false;
			}
			
            unsigned int uniqueId = syncFunc.getUniqueId();

			if (m_channel.sendData(uniqueId, b) == false)
            {
                return false;
            }

            // wait for respose
            if (m_channel.recvData(uniqueId, b, m_timRX) == false)
            {
                return false;
            }

            return syncFunc.parseResponse(b);
		}
		
		bool CRpcInvoker::registerFunction(std::function<CIRpcFunctionOut* (void)> createFunction)
		{
			std::shared_ptr<CIRpcFunctionOut> sharedFuncPtr(createFunction()); // must be on separate line!!! otherwise mem leak
            unsigned int uniqueId = sharedFuncPtr->getUniqueId();

            if (m_syncFunctions.count(uniqueId)) return false; // safety check

			std::pair<std::map<unsigned int, std::shared_ptr<CIRpcFunctionOut> >::iterator, bool> ret;
			ret = m_functions.insert(std::pair<unsigned int, std::shared_ptr<CIRpcFunctionOut> > (uniqueId, sharedFuncPtr));
			return ret.second;	
		}
		
        bool CRpcInvoker::registerSyncFunction(std::function<CIRpcSyncFunctionOut* (void)> createSyncFunction)
		{
			std::shared_ptr<CIRpcSyncFunctionOut> sharedSyncFuncPtr(createSyncFunction()); // must be on separate line!!! otherwise mem leak
            unsigned int uniqueId = sharedSyncFuncPtr->getUniqueId();

            if (m_functions.count(uniqueId)) return false; // safety check

			std::pair<std::map<unsigned int, std::shared_ptr<CIRpcSyncFunctionOut> >::iterator, bool> ret;
			ret = m_syncFunctions.insert(std::pair<unsigned int, std::shared_ptr<CIRpcSyncFunctionOut> > (uniqueId, sharedSyncFuncPtr));
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

        bool CRpcInvoker::findSyncFunction(unsigned int uniqueId, std::shared_ptr<CIRpcSyncFunctionOut> &syncFunction)
        {
        	std::map<unsigned int, std::shared_ptr<CIRpcSyncFunctionOut> >::iterator it;
			it = m_syncFunctions.find(uniqueId);
			if (it == m_syncFunctions.end()) return false; // function not found
			syncFunction = it->second;
			return true; // ok, found
        }
	}
}
