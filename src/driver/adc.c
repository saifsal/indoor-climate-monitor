/** @file adc.c
 *  @brief ADC driver.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "adc.h"

/* AVR includes */
#include <avr/boot.h>
#include <avr/io.h>

void adc_setup(const unsigned char admux, const unsigned char refs) {
  /* Set ADC to admux with refs, result right adjusted
   * refs:
   * 0 AREF, internal V_REF turned off
   * 1 AV_CC with external capacitor at AREF pin
   * 3 Internal 1.1V voltage reference with external capacitor at AREF pin
   * admux:
   * 0 - 8 ADC0-ADC8
   * */
  ADMUX = (refs << 6) | admux;
  /* Enable ADC */
  ADCSRA = 0x81;
  /* Set prescaler to 1, no interrupts, no auto trigger */
}

unsigned short adc_read(void) {
  unsigned short res;

  /* Start conversion */
  ADCSRA |= 0x40;

  /* Wait for converstion to finish */
  loop_until_bit_is_set(ADCSRA, ADIF);

  /* Read result, low must be read first! */
  res = ADCL;
  res |= (ADCH << 8);

  /* Clear conversion finished flag */
  ADCSRA &= ~(_BV(ADIF));
  return res;
}

float adc_temperature(void) {
  short ts_offset = boot_signature_byte_get(0x0002);
  /* Converting from unsigned 8-bit fixed point to integer
   * After some tests it appears that its value always is 255
   * In fixed point unsigned this equals 2 - 2^(-7) = 1.9922
   * To convert to floating point divide the fixed point by 128
   * */
  float ts_gain = boot_signature_byte_get(0x0003) / 128.0f;

  unsigned short val = adc_read();
  float temp =
      ((float)((val - (273 + 100 - ts_offset)) * 128)) / ts_gain + 25.0f;

  return temp;
}
