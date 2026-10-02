/** @file ccs811.c
 *  @brief CCS811 sensor.
 *
 *  The CCS811 sensor is the CO2 sensor.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "ccs811.h"

/* libc includes */
#include <stddef.h>

/* AVR includes */
#include <util/delay.h>

/* Communication includes */
#include "../com/i2c.h"

#define MIN(x, y) (x < y) ? x : y

/* Measurements */
static unsigned short co2 = 400;
static unsigned short tvoc;

/* Hardware ID */
#define CCS811_HW_ID_CODE 0x81

/* Defined registers */
#define CCS811_ADDR (0x5A << 1)
#define CCS811_STATUS 0x00
#define CCS811_MEAS_MODE 0x01
#define CCS811_ALG_RESULT_DATA 0x02
#define CCS811_RAW_DATA 0x03
#define CCS811_ENV_DATA 0x05
#define CCS811_NTC 0x06
#define CCS811_THRESHOLDS 0x10
#define CCS811_BASELINE 0x11
#define CCS811_HW_ID 0x20
#define CCS811_HW_VERSION 0x21
#define CCS811_FW_BOOT_VERSION 0x23
#define CCS811_FW_APP_VERSION 0x24
#define CCS811_ERROR_ID 0xE0
#define CCS811_SW_RESET 0xFF

/* Defined drive modes */
#define CCS811_DRIVE_MODE_IDLE 0x00
#define CCS811_DRIVE_MODE_1SEC 0x01
#define CCS811_DRIVE_MODE_10SEC 0x02
#define CCS811_DRIVE_MODE_60SEC 0x03
#define CCS811_DRIVE_MODE_250MS 0x04

/* Useful boot mode */
#define CCS811_BOOTLOADER_APP_START 0xF4

static void sw_reset(void);
static void set_drive_mode(const unsigned char mode);
static void disable_irq(void);
static unsigned char available(void);
static void read(const unsigned char reg, unsigned char *const buf,
                 const unsigned char num);
static void write(const unsigned char reg, const unsigned char *const buf,
                  const unsigned char num);
static unsigned char read_byte(const unsigned char reg);
static void write_byte(const unsigned char reg, const unsigned char val);
static void touch(const unsigned char reg);

unsigned char ccs811_start(void) {
  sw_reset();
  _delay_ms(100);

  /* Check hardware id */
  if (read_byte(CCS811_HW_ID) != CCS811_HW_ID_CODE)
    return 0;

  /* Try to start the app */
  touch(CCS811_BOOTLOADER_APP_START);
  _delay_ms(100);

  /* Read status register */
  const unsigned char status = read_byte(CCS811_STATUS);
  /* Check if error has occured */
  const unsigned char is_err = status & (1 << 0);
  /* Check if chip started in boot mode */
  const unsigned char is_boot = !(status & (1 << 7));
  /* Return failure in case of error or boot mode operation */
  if (is_err || is_boot)
    return 0;

  disable_irq();

  /* Default to read every second */
  set_drive_mode(CCS811_DRIVE_MODE_1SEC);

  return 1;
}

unsigned char ccs811_read_data(void) {
  if (!available())
    /* Return 2^7 as unavailable, as 0 denotes success from chip */
    return (1 << 7);
  else {
    unsigned char buf[8];
    /* Read result data */
    read(CCS811_ALG_RESULT_DATA, buf, 8);
    /* Calculate CO2 */
    co2 = ((unsigned short)buf[0] << 8) | ((unsigned short)buf[1]);
    /* Calculate TVOC */
    tvoc = ((unsigned short)buf[2] << 8) | ((unsigned short)buf[3]);
    /* Return error id if error has occured else return success */
    if (buf[5] != 0)
      return buf[5];
    else
      return 0;
  }
}

unsigned short ccs811_co2(void) { return co2; }

unsigned short ccs811_tvoc(void) { return tvoc; }

static void sw_reset(void) {
  /* Reset sequence gotten from datasheet */
  unsigned char rst[] = {0x11, 0xE5, 0x72, 0x8A};
  write(CCS811_SW_RESET, rst, 4);
}

static void set_drive_mode(const unsigned char mode) {
  /* Get current meas mode from chip register */
  unsigned char set_mode = read_byte(CCS811_MEAS_MODE);
  /* Clear the three bits with drive mode settings */
  set_mode &= 0x8F;
  /* Input new drive mode settings */
  set_mode |= (mode << 4);
  /* Write new meas mode settngs to chip */
  write_byte(CCS811_MEAS_MODE, set_mode);
}

static void disable_irq(void) {
  /* Get current meas mode from chip register */
  unsigned char set_mode = read_byte(CCS811_MEAS_MODE);
  /* Disable interrupt setting */
  set_mode &= ~(1 << 3);
  /* Write new meas mode settings */
  write_byte(CCS811_MEAS_MODE, set_mode);
}

static unsigned char available(void) {
  /* Check if data is ready */
  if (read_byte(CCS811_STATUS) & (1 << 3))
    return 1;
  return 0;
}

static void read(const unsigned char reg, unsigned char *const buf,
                 const unsigned char num) {

  unsigned char pos = 0;

  while (pos < num) {
    const unsigned char to_read = MIN(32, num - pos);
    i2c_start(CCS811_ADDR);
    i2c_write(reg + pos);
    i2c_stop();

    i2c_start(CCS811_ADDR + I2C_READ);
    for (unsigned char i = 0; i < to_read - 1; ++i) {
      buf[pos] = i2c_read(ACK);
      ++pos;
    }
    buf[pos] = i2c_read(NAK);
    ++pos;
    i2c_stop();
  }
}

static void write(const unsigned char reg, const unsigned char *const buf,
                  const unsigned char num) {
  i2c_start(CCS811_ADDR);
  i2c_write(reg);
  for (unsigned char i = 0; i < num; ++i)
    i2c_write(buf[i]);
  i2c_stop();
}

static unsigned char read_byte(const unsigned char reg) {
  i2c_start(CCS811_ADDR);
  i2c_write(reg);
  i2c_stop();

  i2c_start(CCS811_ADDR + I2C_READ);
  const unsigned char val = i2c_read(NAK);
  i2c_stop();
  return val;
}

static void write_byte(const unsigned char reg, const unsigned char val) {
  i2c_start(CCS811_ADDR);
  i2c_write(reg);
  i2c_write(val);
  i2c_stop();
}

static void touch(const unsigned char reg) {
  i2c_start(CCS811_ADDR);
  i2c_write(reg);
  i2c_stop();
}
