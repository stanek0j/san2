
#pragma once

#include <map>
#include <memory>
#include <utility>

#include "utils/cvector.hpp"
#include "utils/cthread.hpp"
#include "cirpcfunction.hpp"
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
		class CRpcExecutor : San2::Utils::CThread
		{
		public:
			CRpcExecutor(CIRpcChannel& channel, unsigned int timRX);
			void run();
			// bool registerFunction(std::shared_ptr<RpcFunction> func);
			bool registerFunction(std::function<CIRpcFunction* (void)> createFunction);
			RpcError executeFunction(unsigned int uniqueId, const San2::Utils::bytes &in);
			
			// destructor must be virtual!
			virtual ~CRpcExecutor(){ /* intentionally empty */ };
		
		private:
			std::map<unsigned int, std::shared_ptr<CIRpcFunction> > m_functions;
			bool findFunction(unsigned int uniqueId, std::shared_ptr<CIRpcFunction> &function);
			CIRpcChannel &m_channel;
			unsigned int m_timRX;
			
			// another msvc fix
		    #ifdef LINUX
			    CRpcExecutor(const CRpcExecutor& copyFromMe)=delete;
			    CRpcExecutor& operator=(const CRpcExecutor& copyFromMe)=delete;
		    #endif
		};
	}
}
