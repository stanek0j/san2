#include <cctype> // For isxdigit

#include "hex.hpp"

namespace San2 { namespace Utils {

// byte -> string
const char *hexTable[] = {
"00","01","02","03","04","05","06","07","08","09","0A","0B","0C","0D","0E","0F",
"10","11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F",
"20","21","22","23","24","25","26","27","28","29","2A","2B","2C","2D","2E","2F",
"30","31","32","33","34","35","36","37","38","39","3A","3B","3C","3D","3E","3F",
"40","41","42","43","44","45","46","47","48","49","4A","4B","4C","4D","4E","4F",
"50","51","52","53","54","55","56","57","58","59","5A","5B","5C","5D","5E","5F",
"60","61","62","63","64","65","66","67","68","69","6A","6B","6C","6D","6E","6F",
"70","71","72","73","74","75","76","77","78","79","7A","7B","7C","7D","7E","7F",
"80","81","82","83","84","85","86","87","88","89","8A","8B","8C","8D","8E","8F",
"90","91","92","93","94","95","96","97","98","99","9A","9B","9C","9D","9E","9F",
"A0","A1","A2","A3","A4","A5","A6","A7","A8","A9","AA","AB","AC","AD","AE","AF",
"B0","B1","B2","B3","B4","B5","B6","B7","B8","B9","BA","BB","BC","BD","BE","BF",
"C0","C1","C2","C3","C4","C5","C6","C7","C8","C9","CA","CB","CC","CD","CE","CF",
"D0","D1","D2","D3","D4","D5","D6","D7","D8","D9","DA","DB","DC","DD","DE","DF",
"E0","E1","E2","E3","E4","E5","E6","E7","E8","E9","EA","EB","EC","ED","EE","EF",
"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","FA","FB","FC","FD","FE","FF"
};

const char** getHexTable()
{
    return hexTable; // avoiding extern keyword
}

std::string byteToString(unsigned char b)
{
    return std::string(hexTable[b]);
}

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
