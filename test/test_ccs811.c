/** @file main.c
 *  @brief Main utility for ICM
 *
 *  This application runs the main utility for ICM.
 *  It reads sensor values, adjusts the light,
 *  and pushes the values to the cloud.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */

/* libc includes */
#include <stdio.h>

/* AVR includes */
#include <avr/io.h>
#include <util/delay.h>

/* Communication includes */
#include "com/uart.h"
#include "com/i2c.h"
/* Driver includes */
#include "driver/init.h"

/* Sensor includes */
#include "sensor/ccs811.h"

/* Bit manipulation defines */
#define RSET(r, v) r |= _BV(v)    /* Set bit */
#define RCLR(r, v) r &= ~(_BV(v)) /* Clear bit */
#define RTOG(r, v) r ^= _BV(v)    /* Toggle bit */

/* Buffer for string manipulation */
static unsigned char buffer[_BV(5)];


/** @brief Heartbeat function.
 *
 *  Toggles hearbeat LED and waits 500 ms.
 *
 *  @return Void.
 */
void live(void);

/** @brief Main ICM application.
 *  @return Should not return.
 */

int main(void) {
  static unsigned short co2;
  static unsigned short tvoc;

  init_program();
  /* Initialize LED */
  RSET(DDRB, 5);
  unsigned char success = ccs811_start();
  sprintf((char*)buffer,"Turned on: %i\r\n",success);
  uart_transmit_string(buffer);

  while(1) {
    live();

    success = ccs811_read_data();
    sprintf((char*)buffer,"Data ready: %i\r\n",success);
    uart_transmit_string(buffer);

    co2 = ccs811_co2();
    tvoc = ccs811_tvoc();

    sprintf((char*)buffer,"CO2: %i, TVOC: %i\r\n",co2,tvoc);
    uart_transmit_string(buffer);
  }
}

void live(void) {
  RTOG(PORTB, 5);
  _delay_ms(2000);
}
