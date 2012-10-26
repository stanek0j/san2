
#pragma once

#include <string>
#include <vector>

namespace San2 
{
	namespace Utils
	{
		// return value: true = success
		bool hexToBytes(const std::string& str, std::vector<unsigned char>& data);
	}
}
