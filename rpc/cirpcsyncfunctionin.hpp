
#pragma once

#include "utils/cvector.hpp"

namespace San2 
{
	namespace Rpc
	{
		class CIRpcSyncFunctionIn
		{
		public:			
			// destructor must be virtual!
			virtual ~CIRpcSyncFunctionIn(){ /* intentionally empty */ }
		
			virtual unsigned int getUniqueId()const=0 ;
			virtual bool operator()(void)=0;
			
			// std::less<T> (because of std::map in CRpcExecutor)
			bool operator() (const CIRpcSyncFunctionIn& x, const CIRpcSyncFunctionIn& y) const;
			
			// TRUE = success
			virtual bool unpack(const San2::Utils::bytes &in)=0;
            virtual San2::Utils::bytes getResponse()=0;
		
		private:
			// another msvc fix
		    #ifdef LINUX
			    //CIRpcFunction(const CIRpcFunction& copyFromMe)=delete;
			    //CIRpcFunction& operator=(const CIRpcFunction& copyFromMe)=delete;
		    #endif
		};
	}
}
