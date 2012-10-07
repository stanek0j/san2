
#pragma once

#include "utils/cvector.hpp"

namespace San2 
{
	namespace Rpc
	{
		class CIRpcFunction
		{
		public:			
			// destructor must be virtual!
			virtual ~CIRpcFunction(){ /* intentionally empty */ }
		
			virtual unsigned int getUniqueId()const=0 ;
			virtual bool operator()(void)=0;
			
			// std::less<T> (because of std::map in CRpcExecutor)
			bool operator() (const CIRpcFunction& x, const CIRpcFunction& y) const;
			
			// TRUE = success
			virtual bool pack(San2::Utils::bytes &out)=0;
			virtual bool unpack(const San2::Utils::bytes &in)=0;
		
		private:
			// another msvc fix
		    #ifdef LINUX
			    //CIRpcFunction(const CIRpcFunction& copyFromMe)=delete;
			    //CIRpcFunction& operator=(const CIRpcFunction& copyFromMe)=delete;
		    #endif
		};
	}
}
