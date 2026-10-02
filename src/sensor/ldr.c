/** @file ldr.c
 *  @brief LDR sensor.
 *
 *  The LDR sensor is the light or lux sensor.
 *
 *  @author Mikkel Isaksen Jensen
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "ldr.h"

/* Driver includes */
#include "../driver/adc.h"

unsigned char ldr_read(void) {
  /* setup adc */
  adc_setup(3, 1); /* 3 = admux 1 = intern reff 5 volt */
  unsigned long samples = 0;
  for (unsigned char i = 0; i < (1 << 4); ++i) {
    samples += adc_read();
  }
  /* return samples bitshifted to the right because of _BV(4) in forloop */
  return (unsigned char)(samples >> 6);
}
