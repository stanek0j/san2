
#pragma once

#include "utils/cvector.hpp"

namespace San2 
{
	namespace Rpc
	{
		class CIRpcSyncFunctionOut
		{
		public:			
			// destructor must be virtual!
			virtual ~CIRpcSyncFunctionOut(){ /* intentionally empty */ }
		
			virtual unsigned int getUniqueId()const=0;
			
			// std::less<T> (because of std::map in CRpcExecutor)
			bool operator() (const CIRpcSyncFunctionOut& x, const CIRpcSyncFunctionOut& y) const;
			
			// TRUE = success
			virtual bool pack(San2::Utils::bytes &out)=0;
            virtual bool parseResponse(const San2::Utils::bytes &in)=0;
		
		private:
			// another msvc fix
		    #ifdef LINUX
			    //CIRpcFunction(const CIRpcFunction& copyFromMe)=delete;
			    //CIRpcFunction& operator=(const CIRpcFunction& copyFromMe)=delete;
		    #endif
		};
	}
}
