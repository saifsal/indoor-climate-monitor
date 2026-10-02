/** @file crc.c
 *  @brief CRC 16 calculation.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "crc.h"

/* AVR includes */
#include <util/crc16.h>

unsigned short crc16(const unsigned char *buf, unsigned char len) {
  unsigned short crc = 0xFFFF;
  while (len) {
    crc = _crc16_update(crc, *buf);
    ++buf;
    --len;
  }
  return crc;
}
