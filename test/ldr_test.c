#include "com/uart.h"
#include "driver/adc.h"
#include "driver/init.h"
#include "sensor/ldr.h"
#include <avr/io.h>
#include <stdio.h>

#define RSET(r, v) r |= (1 << v)
#define RCLR(r, v) r &= ~(1 << v)
#define RTOG(r, v) r ^= (1 << v)

void init(void);

int main(void) {
  unsigned char buf[20];
  init();
  adc_setup(8, 3);

  while (1) {
    unsigned char ldr = ldr_read();
    snprintf((char *)buf, 20, "LDR: %i\n", ldr);
    uart_transmit_string(buf);
  }
}

void init(void) {
  init_program();

  /* Initialize LED */
  RSET(DDRB, 5);
}
