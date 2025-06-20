#include "Utils.hpp"

u16 mHTONS(u16 port)
{
  u16 mask_for_endianness_check = 0x0102;
  u8 *byte_ptr = (u8 *)&mask_for_endianness_check;

  // 0x01 - MSB
  // 0x02 - LSB
  // little-endian : 0x02, 0x01
  // big-endian    : 0x01, 0x02
  
  if (byte_ptr[0] == 0x02) // little endian
  {
    return (port << 8) | (port >> 8);
  }

  // big endian
  return port;
}
