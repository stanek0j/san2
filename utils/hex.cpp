#include <cctype> // For isxdigit

#include "hex.hpp"

namespace San2 { namespace Utils {

bool hexToBytes(const std::string& str, std::vector<unsigned char>& data)
{
  // Sanity check
  //static_assert<8 == CHAR_BIT>::valid_expression();

  // Clear output
  data.clear();

  // No data? Then we're done
  if (str.empty()) return true;

  // Must be prepared that string can have odd number of
  // nybbles, in which case the first is treated like the low
  // nybble of the first byte
  size_t lengthOverflow = str.length() % 2;

  // This also affects the length of the data buffer we
  // allocate (need full  byte for nybble)
  const size_t length = lengthOverflow + str.length() / 2;
  data.resize(length);

  // Buffer for byte conversion
  static char buf[3];
  buf[2] = 0;
  // End of input
  char* pend = &buf[2];

  // Iterators for input and output
  size_t i = 0;
  size_t c = 0;

  // If the first nybble is a low, we'll do it separately
  if (1 == lengthOverflow)
  {
    buf[0] = '0';
    buf[1] = str[c++];
    unsigned char x = static_cast<unsigned char> (strtoul(buf, &pend, 16));

    // Parsing should stop at terminating zero
    if (pend != &buf[2])
    {
      std::string e = "Invalid character in hex string: \'";
      e += *(pend);
      e += "'";
      //throw std::invalid_argument(e);
      return false;
    }
    data[i++] = x;
  }

  // For each output byte, we use two input characters for
  // high and low nybble, respectively
  for (; i < length; ++i)
  {
    buf[0] = str[c++];
    // strtoul accepts initial whitespace or sign, we can't
    if (!isxdigit(buf[0]))
    {
      std::string e = "Invalid character in hex string: \'";
      e += buf[0];
      e += "'";
      // throw std::invalid_argument(e);
      return false;
    }

    buf[1] = str[c++];
    unsigned char x = static_cast<unsigned char> (strtoul(buf, &pend, 16));

    // Parsing should stop at terminating zero
    if (pend != &buf[2])
    {
      std::string e = "Invalid character in hex string: \'";
      e += *(pend);
      e += "'";
      // throw std::invalid_argument(e);
      return false;
    }

    data[i] = x;
  }
  return true;
}

}} // ns
