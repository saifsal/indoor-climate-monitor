/** @file crc.h
 *  @brief Function prototypes for CRC calculation.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _CRC_H
#define _CRC_H

/** @brief Performs CRC 16 calculation
 *
 *  @param buf Pointer to byte buffer.
 *  @param len Length of byte buffer.
 *  @return CRC 16 result.
 */
extern unsigned short crc16(const unsigned char *buf, unsigned char len);

#endif /* _CRC_H */
