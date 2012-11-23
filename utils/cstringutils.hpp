
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

        protected:


        private:
        
        };
    
    
    }


}