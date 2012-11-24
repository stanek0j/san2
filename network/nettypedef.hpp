

#pragma once

#include <array>
#include "utils/cvector.hpp"  

namespace San2
{
	namespace Network
	{
		const std::size_t sanCapsuleIdSize = 4;
		const std::size_t sanAddressSize = 32;
		const std::size_t sanHashSize = 20; // length of SHA-1
		const std::size_t sanFlagSize = 1;
		const std::size_t sanHopSize = 2;
		
		typedef std::array<unsigned char, sanAddressSize> SanAddress;
		typedef std::array<unsigned char, sanHashSize> SanApplicationId;
		
		// it is called "aggregate initialization"
		// and yep, the double brackets are needed
        // MSVC again have problems with this
        // const SanAddress sanDefaultAddress {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
		const SanAddress sanDefaultAddress = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        const SanApplicationId sanDefaultApplicationId = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


        // for debugging only (it is randomly chosen)
        const SanApplicationId sanTestMessageApplicationId = {95,103,20,66,235,217,66,62,164,152,26,129,132,0,211,90,0,128,182,176};

		// flags
		// DX - destination execute
		// EX - each node execute
	}
}
