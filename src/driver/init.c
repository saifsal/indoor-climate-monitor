/** @file init.c
 *  @brief Function prototypes for initializing the ICM.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "init.h"

/* AVR includes */
#include <avr/io.h>
#include <compat/twi.h>

/* Sensor includes */
#include "../sensor/ccs811.h"

/** @brief Initializes I2C
 *
 *  With FCPU=1MHz, SCL clock = 62500.
 *
 *  @return Void.
 */
static void i2c_init(void);

/** @brief Initializes SPI
 *
 *  With FCPU=1MHz, SPI clock = 62500.
 *
 *  @return Void.
 */
static void spi_init(void);

/** @brief Initializes UART
 *
 *  With FCPU=1MHz, baud rate = 4808.
 *  8 data bit, no parity, and 1 stop bit.
 *
 *  @return Void.
 */
static void uart_init(void);

void init_program(void) {
  /* Set clock 16 prescaler*/
  CLKPR = _BV(CLKPCE);
  CLKPR = _BV(CLKPS2);
  /* Initialize communication */
  i2c_init();
  spi_init();
  uart_init();
  /* Initialize CCS811 sensor */
  ccs811_start();
}

/* #define SCL_CLOCK 10000 */
static void i2c_init(void) {
  /* Pull up internal resistors */
  PORTC |= (_BV(4) | _BV(5));
  /* Set prescaler to no prescaler */
  TWSR = 0;
  /* Set bit rate REMEMBER CLOCK 1MHZ*/
  TWBR = 0; /*((F_CPU/SCL_CLOCK)-16)/2 */
}

static void spi_init(void) {
  /* Set MOSI and SCK, as output, all others as input */
  DDRB |= (_BV(3) | _BV(5));
  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (_BV(SPE) | _BV(MSTR) | _BV(SPR0));
}

static void uart_init(void) {
  /* BAUDRATE: 4808, F_CPU: 1MHZ */
  const unsigned int baudPrescale = 12; /*(F_CPU / (16 * baudrate)) - 1;*/

  /* Set baudrate in register */
  UBRR0L = baudPrescale & 0x00FF;
  UBRR0H = baudPrescale >> 8;
  /* Default frame format is 8 bits data, no parity, and 1 stop bit */

  /* Enable RX, TX */
  UCSR0B = (_BV(RXEN0) | _BV(TXEN0));
}
