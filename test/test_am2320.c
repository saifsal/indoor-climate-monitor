#include "com/i2c.h"
#include "com/uart.h"
#include "driver/init.h"
#include "sensor/am2320.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#define RSET(r, v) r |= (1 << v)
#define RCLR(r, v) r &= ~(1 << v)
#define RTOG(r, v) r ^= (1 << v)

static unsigned char buffer[_BV(5)];

void init(void);
void live(void);

int main(void) {
  static float temp;
  static float humd;

  static unsigned long t;
  static unsigned long h;

  init();

  while (1) {
    if (am2320_read()) {
      humd = am2320_humidity();
      temp = am2320_temperature();
      memcpy(&t, &temp, sizeof(temp));
      memcpy(&h, &humd, sizeof(humd));
      sprintf((char *)buffer, "H: 0x%lX\r\nT: 0x%lX\r\n", h, t);
      uart_transmit_string(buffer);
    }
    live();
  }
}

void init(void) {
  init_program(0);
  /* Initialize LED */
  RSET(DDRB, 5);
}

void live(void) {
  RTOG(PORTB, 5);
  _delay_ms(500);
}
