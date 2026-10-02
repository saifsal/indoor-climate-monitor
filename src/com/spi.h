/** @file spi.h
 *  @brief Function prototypes for the SPI driver.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _SPI_H
#define _SPI_H

/** @brief Transfer byte via SPI.
 *
 *  @param data The byte of data to transmit. Interpreted as unsigned char
 * buffer.
 *  @return The byte of data received.
 */
extern unsigned char spi_transfer(const unsigned char data);

#endif /* _SPI_H */
