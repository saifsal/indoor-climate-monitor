/** @file am2320.c
 *  @brief AM2320 sensor.
 *
 *  The AM2320 sensor is the temperature and humidity sensor.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "am2320.h"

/* AVR includes */
#include <util/delay.h>

/* Communication includes */
#include "../com/i2c.h"

/* Driver includes */
#include "../driver/crc.h"

/* The address must be bitshifted once left to convert to 7-bit address */
#define AM2320_I2C_ADDR (0x5C << 1)
#define AM2320_CMD_READ 0x03
#define AM2320_REG_HUMIDITY_H 0x00
#define AM2320_REG_HUMIDITY_L 0x01
#define AM2320_REG_TEMPERATURE_H 0x02
#define AM2320_REG_TEMPERATURE_L 0x03

/* Buffer to store the measurements from AM2320 */
static unsigned char buffer[8];

unsigned char am2320_read(void) {
  /* Wake up chip */
  i2c_start(AM2320_I2C_ADDR);
  i2c_stop();

  /* Read humidity and temperature register */
  i2c_start(AM2320_I2C_ADDR);
  i2c_write(AM2320_CMD_READ);
  i2c_write(0x00);
  i2c_write(0x04);
  i2c_stop();
  /* Wait >1.5 ms */
  _delay_ms(2);
  /* Read data: function_code(1) + counts(1) + data(4) + crc(2) = 8 */
  i2c_start(AM2320_I2C_ADDR + I2C_READ);
  for (unsigned char i = 0; i < 6; ++i)
    buffer[i] = i2c_read(ACK);

  /* Check for correct reply */
  if ((buffer[0] != AM2320_CMD_READ) || (buffer[1] != 0x04))
    return 0;

  unsigned short crc = 0;
  crc = i2c_read(ACK);       /* crc low byte */
  crc |= i2c_read(NAK) << 8; /* crc high byte */

  if (crc == crc16(buffer, 6))
    return 1;

  return 0;
}

float am2320_humidity(void) {
  return (float)((buffer[2] << 8) + buffer[3]) / 10.0f;
}

float am2320_temperature(void) {
  return (float)((buffer[4] << 8) + buffer[5]) / 10.0f;
}
