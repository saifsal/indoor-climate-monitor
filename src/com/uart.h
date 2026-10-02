/** @file uart.h
 *  @brief Function prototypes for the UART driver.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _UART_H
#define _UART_H

/** @brief Transmits null-terminated string through UART.
 *
 *  @param data The string to transmit. Interpreted as unsigned char buffer.
 *  @return Void.
 */
extern void uart_transmit_string(const unsigned char *const data);

/** @brief Transmit byte through UART.
 *
 *  @param data Byte to transmit. Unsigned char.
 *  @return Void.
 */
extern void uart_transmit(const unsigned char data);

/** @brief Receive byte from UART.
 *
 *  @param Void.
 *  @return Byte received from UART.
 */
extern unsigned char uart_receive(void);

#endif /* _UART_H */
