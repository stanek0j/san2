
#pragma once

#include "cistreamrw.hpp"


// ze by RPC predelat na streamove zpracovani???
// dost sileny
// kapsule se musi stejne nejdriv nacist cela do pameti, aby se mohla
// spustit. Nebo by to tak alespon melo byt.?

// Kdo se v tom nevyzna at si precte http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern 
// jednoduse ten template parametr je trida implementujici CIStreamRW interface, ve skutecnosti
// ji vubec nemusi dedit, to slouzi jen jako kontorla.
// Takze az vam to nekomu hodi chybu pro kompilaci, 
// nejdriv zkontrolujte, zda-li trida CIStreamRWDerived dedi CIStreamRW 

namespace San2
{
	namespace Stream
	{
		template<class CIStreamRWDerived>
		class CIMessageSeparator : protected CIStreamRWDerived // protected or private? I think protected.
		{
		public:
			CIMessageSeparator(const unsigned int maxSingleReadSize) :
				CStreamRWDerived(maxSingleReadSize)
			{
				
			}
			
			virtual ~CIMessageSeparator(){}
			
			bool readOneMessage(San2::Utils::bytes &out)=0;
			bool writeOneMessage(const San2::Utils::bytes& in)=0;
			
			// another msvc fix
		    #ifdef LINUX
			    CIMessageSeparator(const CIMessageSeparator& copyFromMe)=delete;
			    CIMessageSeparator& operator=(const CIMessageSeparator& copyFromMe)=delete;
		    #endif
		};
	}
}
