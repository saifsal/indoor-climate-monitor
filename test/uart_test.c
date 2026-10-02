#include "i2c.h"
#include "spi.h"
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

#define RSET(r, v) r |= (1 << v)
#define RCLR(r, v) r &= ~(1 << v)
#define RTOG(r, v) r ^= (1 << v)

void init(void);
void live(void);
void uart_test(void);

int main(void) {
  init();

  while (1) {
    uart_test();
    live();
  }
}

void init(void) {
  /* Set clock 16 prescaler*/
  CLKPR = (1 << CLKPCE);
  CLKPR = (1 << CLKPS2);
  /* Initialize LED */
  RSET(DDRB, 5);
  /* Initialize UART */
  uart_init();
}

void live(void) {
  RTOG(PORTB, 5);
  _delay_ms(500);
}

void uart_test(void) {
  static unsigned char i = 0;
  uart_transmit(i);
  i = (i + 1) % 128;
}
