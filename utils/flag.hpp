#pragma once

namespace San2 
{
	namespace Utils
	{
		namespace Flag
		{
			inline void setFlag(unsigned char &flags, unsigned int position, bool value)
			{
				if (value) flags |= 1 << position;
				else flags &= ~(1 << position);
			}

			inline bool getFlag(unsigned char flags, unsigned int position)
			{
				return (flags & (1 << position));
			}

			inline void toggleFlag(unsigned char &flags, unsigned int position)
			{
				flags ^= 1 << position;
			}
		}
	}
}
