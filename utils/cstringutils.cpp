#include "cstringutils.hpp"

namespace San2 { namespace Utils {

std::vector<std::string>& CStringUtils::split(std::vector<std::string>& result, const std::string& inputString, char delimiter)
{
  result.clear();
  std::istringstream ss(inputString);
  while (!ss.eof())
  {
    std::string field;
    std::getline(ss, field, delimiter );
    if (field.empty()) continue;
    result.push_back(field);
  }
  return result;
}


}} // ns