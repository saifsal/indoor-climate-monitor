/** @file spi.h
 *  @brief SPI driver.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */
/* -- Includes -- **/
#include "spi.h"

/* AVR includes */
#include <avr/io.h>

unsigned char spi_transfer(const unsigned char data) {
  /* Start transmission */
  SPDR = data;

  /* Wait for transmission to complete */
  loop_until_bit_is_set(SPSR, SPIF);

  /* Return byte received */
  return SPDR;
}
