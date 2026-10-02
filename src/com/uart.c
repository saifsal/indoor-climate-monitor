/** @file uart.c
 *  @brief UART driver.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "uart.h"

/* AVR includes */
#include <avr/io.h>

void uart_transmit_string(const unsigned char *const dataPtr) {
  for (unsigned long i = 0; dataPtr[i] != '\0'; ++i)
    uart_transmit(dataPtr[i]);
  /* Wait for last transmission to finalize */
  loop_until_bit_is_set(UCSR0A, TXC0);
}

void uart_transmit(const unsigned char data) {
  /* Wait for empty transmit buffer */
  loop_until_bit_is_set(UCSR0A, UDRE0);

  /* Put data into transmit buffer to transmit it */
  UDR0 = data;
}

unsigned char uart_receive(void) {
  /* Wait for full receive buffer */
  loop_until_bit_is_set(UCSR0A, RXC0);

  /* Return gotten data */
  return UDR0;
}
