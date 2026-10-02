/** @file i2c.c
 *  @brief I2C driver.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "i2c.h"

/* AVR includes */
#include <util/twi.h>

static void wait_timeout(void);

unsigned char i2c_start(const unsigned char address) {
  unsigned char status;

  /* Send start condition */
  TWCR = (_BV(TWINT) | _BV(TWSTA) | _BV(TWEN));

  /* Wait until transmission completed */
  wait_timeout();

  /* Check value of TWI Status Register. Mask prescaler bits. */
  status = TW_STATUS & 0xF8;
  if ((status != TW_START) && (status != TW_REP_START))
    return NAK;

  /* Send device address */
  TWDR = address;
  TWCR = (_BV(TWINT) | _BV(TWEN));

  /* Wait until transmission completed and ACK/NACK has been received */
  wait_timeout();

  /* Check value of TWI Status Register. Mask prescaler bits. */
  status = TW_STATUS & 0xF8;
  if ((status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK))
    return NAK;

  return ACK;
}

void i2c_stop(void) {
  /* Send stop condition */
  TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTO));
  /* Wait until stop condition is executed and line is released */
  loop_until_bit_is_clear(TWCR, TWSTO);
}

unsigned char i2c_write(const unsigned char data) {
  unsigned char status;

  /* Send data to previously addressed device */
  TWDR = data;
  TWCR = (_BV(TWINT) | _BV(TWEN));

  /* Wait until transmission completed */
  wait_timeout();

  /* Check value of TWI Status Register. Mask prescaler bits. */
  status = TW_STATUS & 0xF8;
  if (status != TW_MT_DATA_ACK)
    return NAK;

  return ACK;
}

unsigned char i2c_read(const unsigned char ack) {
  TWCR = (_BV(TWINT) | _BV(TWEN) | ((1 & ack) << TWEA));
  wait_timeout();
  return TWDR;
}

static void wait_timeout(void) {
  unsigned int i = 50000;
  do {
    if (bit_is_set(TWCR, TWINT))
      break;
  } while (--i);
}
