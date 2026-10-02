/** @file pwm.h
 *  @brief Function prototypes PWM.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "pwm.h"

/* AVR includes */
#include <avr/io.h>

#define R 5 /* Pin 5 */
#define W 3 /* Pin 3 */
#define B 6 /* Pin 6 */
#define G 9 /* Pin 9 */

void pwm_red(const unsigned char duty) {
  /* Set OC0B as output */
  DDRD |= _BV(R);
  PORTD &= ~(_BV(R));

  /* Set OC0B on match down counting / Clear OC0B on match up counting */
  TCCR0A |= 0x21;
  /* Start clock, set prescaler to 1 */
  TCCR0B |= 0x01;
  /* Set duty cycle */
  OCR0B = duty;
}

void pwm_white(const unsigned char duty) {
  /* Set OC2B as output */
  DDRD |= _BV(W);
  PORTD &= ~(_BV(W));

  /* Set OC2B on match down counting / Clear OC2B on match up counting */
  TCCR2A |= 0x21;
  /* Start clock, set prescaler to 1 */
  TCCR2B |= 0x01;
  /* Set duty cycle */
  OCR2B = duty;
}

void pwm_blue(const unsigned char duty) {
  /* Set OC0A as output */
  DDRD |= _BV(B);
  PORTD &= ~(_BV(B));

  /* Set OC0A on match down counting / Clear OC0A on match up counting */
  TCCR0A |= 0x81;
  /* Start clock set prescaler to 1 */
  TCCR0B |= 0x01;
  /* Set duty cycle */
  OCR0A = duty;
}
