

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
		
		const std::size_t IPv4Size = 4;
		const std::size_t IPv6Size = 16;
		
		const std::size_t minimumCapsuleHeaderSize = sanCapsuleIdSize + 2 * sanAddressSize + sanHashSize + sanFlagSize + sanHopSize;
		
		typedef std::array<unsigned char, sanAddressSize> SanAddress;
		typedef std::array<unsigned char, sanHashSize> SanApplicationId;
		
		typedef std::array<unsigned char, IPv4Size> IPv4Address;
		typedef std::array<unsigned char, IPv6Size> IPv6Address;
		
		
		// flags
		// DX - destination execute
		// EX - each node execute
	}
}
