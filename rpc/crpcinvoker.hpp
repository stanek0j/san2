
#pragma once

#include <map>
#include <memory>
#include <utility>

#include "utils/cvector.hpp"
#include "utils/cthread.hpp"
#include "cirpcfunctionout.hpp"
#include "cirpcsyncfunctionout.hpp"
#include "cirpcchannel.hpp"
#include "rpcerror.hpp"

/* HUGE NOTE:
 * As the functions are in fact pointers to INSTANCES of class Derived form RpcFunction (implementig operator())
 * there must be guarantee that inside instance of RpcExecutor the "function" is NOT accessed concurrently
 * as the members are shared between functions calls.
 * 
 * Semi-Persisten function object
*/

namespace San2 
{
	namespace Rpc
	{
		class CRpcInvoker
		{
		public:
            CRpcInvoker(CIRpcChannel& channel, unsigned int timRX);
			
			// bool registerFunction(std::shared_ptr<RpcFunction> func);
			bool registerFunction(std::function<CIRpcFunctionOut* (void)> createFunction);
            bool registerSyncFunction(std::function<CIRpcSyncFunctionOut* (void)> createFunction);
			
			bool invokeFunction(CIRpcFunctionOut& func);
            bool invokeSyncFunction(CIRpcSyncFunctionOut& func);
			
			// destructor must be virtual!
			virtual ~CRpcInvoker(){ /* intentionally empty */ };
		
		private:
			std::map<unsigned int, std::shared_ptr<CIRpcFunctionOut> > m_functions;
            std::map<unsigned int, std::shared_ptr<CIRpcSyncFunctionOut> > m_syncFunctions;

			bool findFunction(unsigned int uniqueId, std::shared_ptr<CIRpcFunctionOut> &function);
            bool findSyncFunction(unsigned int uniqueId, std::shared_ptr<CIRpcSyncFunctionOut> &syncFunction);
			
            CIRpcChannel &m_channel;
			unsigned int m_timRX;
			
			// another msvc fix
		    #ifdef LINUX
			    CRpcInvoker(const CRpcInvoker& copyFromMe)=delete;
			    CRpcInvoker& operator=(const CRpcInvoker& copyFromMe)=delete;
		    #endif
		};
	}
}
