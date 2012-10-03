

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
		
		typedef std::array<unsigned char, sanAddressSize> SanAddress;
		typedef std::array<unsigned char, sanHashSize> SanApplicationId;
		typedef unsigned char SanFlags;
		typedef std::array<unsigned char, sanHopSize> SanHops;
		
		typedef std::array<unsigned char, IPv4Size> IPv4Address;
		typedef std::array<unsigned char, IPv6Size> IPv6Address;
		
		typedef San2::Utils::CVector<unsigned char> bytes;
		
		// flags
		// DX - destination execute
		// EX - each node execute
	}
}
