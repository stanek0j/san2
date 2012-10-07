

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "utils/cvector.hpp"

int main(int argc, char *argv[])
{	
	std::string str1("Ahoj, jak");
	std::wstring str2 = L" se mas,";
	std::wstring str3 = L" co delas?";

	San2::Utils::bytes vec1;
	San2::Utils::bytes vec2;
	San2::Utils::bytes vec3;
	
	//vec1 = San2::Utils::string2bytes(str1);
	//vec2 = San2::Utils::string2bytes(str2);
	
	vec1 = str1;
	vec2 = str2;
	
	vec3 += str3;
	
	//vec1.append(vec2);
	//vec1 += vec2;
	San2::Utils::bytes result = vec1 + vec2;
	result += vec3;
	
	San2::Utils::bytes::printBytesAsString(result);
	
    printf("\n");
    return 0;
}
