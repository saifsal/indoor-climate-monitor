/** @file communication_controller.c
 *  @brief CommunicationContorller.
 *
 *  @author Saif Salih
 *  @author Fróði Vestergaard Dam
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "communication_controller.h"

/* libc includes */
#include <string.h>

/* AVR includes */
#include <avr/io.h>

/* Communication includes */
#include "../com/i2c.h"

/* Address of communication module */
#define COMM_MODULE_ADDR (0x01 << 1)

/* Numbers of bytes to be send */
#define BYTE_NUM 15

unsigned char push_data(const cloud_data_t *const data) {
  /* Buffer array for holding climate factors */
  unsigned char buf[BYTE_NUM];

  /* Place climate in correct place in buffer array */
  buf[0] = (unsigned char)(data->sd[0].value);      /* CO2 low byte */
  buf[1] = (unsigned char)(data->sd[0].value >> 8); /* CO2 high byte */
  buf[6] = data->vd.icq;                            /* ICQ*/

  /* Format needed climate factors into buffer array */
  memcpy(buf + 2, &data->sd[1].value, 4);  /* Humidity */
  memcpy(buf + 7, &data->sd[2].value, 4);  /* SPL */
  memcpy(buf + 11, &data->sd[3].value, 4); /* Temperature */

  /* Start I2C communication */
  i2c_start(COMM_MODULE_ADDR);

  /* Send climate factors to communication module */
  for (unsigned char i = 0; i < 15; ++i)
    i2c_write(buf[i]);

  /* Stop I2C communication*/
  i2c_stop();

  return 1;
}
