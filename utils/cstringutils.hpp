
#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace San2
{
    namespace Utils
    {
        class CStringUtils
        {
        public:
            static std::vector<std::string>& split(std::vector<std::string>& result, const std::string& inputString, char delimiter);
            
            // true = SUCCESS
            static std::string uintToString(unsigned int num);
            static bool stringToUint(const std::string &sNum, unsigned int &result);

        protected:


        private:
        
        };
    
    
    }


}