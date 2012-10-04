#pragma once

#include <stdint.h>

#define SAN_UINT8 uint8_t
#define SAN_UINT16 uint16_t
#define SAN_UINT32 uint32_t
#define SAN_UINT64 uint64_t

#define SAN_INT8 int8_t
#define SAN_INT16 int16_t
#define SAN_INT32 int32_t
#define SAN_INT64 int64_t

#define SAN_BIG_ENDIAN      0
#define SAN_LITTLE_ENDIAN   1

namespace San2
{
	namespace Utils
	{
		namespace Endian
		{
			inline int san_byte_order()
			{
				short int word = 0x0001;
				char *byte = (char *) &word;
				return(byte[0] ? SAN_LITTLE_ENDIAN : SAN_BIG_ENDIAN);
			}
			
			// ---- always swaps
			
			//! Byte swap unsigned short
			inline SAN_UINT16 san_swap_uint16(SAN_UINT16 val) 
			{
				return (val << 8) | (val >> 8 );
			}

			//! Byte swap short
			inline SAN_INT16 san_swap_int16(SAN_INT16 val) 
			{
				return (val << 8) | ((val >> 8) & 0xFF);
			}

			//! Byte swap unsigned int
			inline SAN_UINT32 san_swap_uint32(SAN_UINT32 val)
			{
				val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
				return (val << 16) | (val >> 16);
			}

			//! Byte swap int
			inline SAN_INT32 san_swap_int32(SAN_INT32 val)
			{
				val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
				return (val << 16) | ((val >> 16) & 0xFFFF);
			}

			inline SAN_INT64 san_swap_int64(SAN_INT64 val)
			{
				val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
				val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
				return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
			}

			inline SAN_UINT64 san_swap_uint64(SAN_UINT64 val)
			{
				val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
				val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
				return (val << 32) | (val >> 32);
			}
			
			// ----- unsigned
			inline SAN_UINT16 san_u_htobe16(SAN_UINT16 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_uint16(val);
				return val;
			}
			
			inline SAN_UINT32 san_u_htobe32(SAN_UINT32 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_uint32(val);
				return val;
			}
			
			inline SAN_UINT64 san_u_htobe64(SAN_UINT64 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_uint64(val);
				return val;
			}
			
			inline SAN_UINT16 san_u_be16toh(SAN_UINT16 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_uint16(val);
				return val;
			}
			
			inline SAN_UINT32 san_u_be32toh(SAN_UINT32 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_uint32(val);
				return val;	
			}
			
			inline SAN_UINT64 san_u_be64toh(SAN_UINT64 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_uint64(val);
				return val;
			}
			
			// ---- signed
			
			inline SAN_INT16 san_s_htobe16(SAN_INT16 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_int16(val);
				return val;
			}
			
			inline SAN_INT32 san_s_htobe32(SAN_INT32 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_int32(val);
				return val;
			}
			
			inline SAN_INT64 san_s_htobe64(SAN_INT64 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_int64(val);
				return val;
			}
			
			inline SAN_UINT16 san_s_be16toh(SAN_UINT16 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_int16(val);
				return val;
			}
			
			inline SAN_UINT32 san_s_be32toh(SAN_UINT32 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_int32(val);
				return val;	
			}
			
			inline SAN_UINT64 san_s_be64toh(SAN_UINT64 val)
			{
				if (san_byte_order() == SAN_LITTLE_ENDIAN) return san_swap_int64(val);
				return val;
			}
		}
	}
}


