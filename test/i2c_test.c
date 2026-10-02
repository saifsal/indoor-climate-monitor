#include "com/i2c.h"
#include "com/spi.h"
#include "com/uart.h"
#include "driver/adc.h"
#include "driver/init.h"
#include "driver/pwm.h"
#include "sensor/am2320.h"
#include "sensor/ldr.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define RSET(r, v) r |= (1 << v)
#define RCLR(r, v) r &= ~(1 << v)
#define RTOG(r, v) r ^= (1 << v)

static unsigned char buffer[8];

void init(void);
void live(void);
void i2c_test(void);

int main(void) {
  init();
  /* Initialize LED */
  RSET(DDRB, 5);
  while (1) {
    i2c_test();
    live();
  }
}

void i2c_test(void) {
  static unsigned char i = 0;
  i2c_start(0x02);
  i2c_write(i);
  i2c_stop();
  i2c_start(0x02 + I2C_READ);
  const unsigned char store = i2c_read(NAK);
  i2c_stop();
  ++i;
  if ( i % 16 != 0) {
    sprintf((char*)buffer, "%i\t",store);
  } else {
    sprintf((char*)buffer, "%i\r\n",store);
  }
  uart_transmit_string(buffer);
}

void live(void) {
  RTOG(PORTB, 5);
  _delay_ms(500);
}
