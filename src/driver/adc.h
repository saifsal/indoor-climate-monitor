/** @file adc.h
 *  @brief Function prototypes ADC.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _ADC_H
#define _ADC_H

/** @brief Performs ADC setup.
 *
 *  Measure from pin admux with reference refs. Result is right adjusted.
 *  Remember reference indicates the max voltage measurable by the ADC.
 *  refs:
 *  0   AREF, internal V_REF turned off. (I.e. voltage on external pin.)
 *  1   AV_CC with external capacitor at AREF pin. (I.e. internal 5V ref.)
 *  3   Internal 1.1V voltage reference with external capacitor at AREF pin.
 *      (I.e. internal 1.1V ref.)
 *
 *  admux:
 *  0 - 8 ADC0-ADC8
 *  4 and 5 used for I2C.
 *  8 is used for internal temperature measurement.
 *  0 is used for microphone.
 *  1 is used for LDR sensor.
 *
 *  First measurement after calling this function requires 25 ADC clock cycles.
 *  Measurements thereafter require 13 ADC clock cycle.
 *  ADC clock is set to 500 kHz. For more information lookup datasheet for
 * ATMega358P.
 *
 *  @param admux ADC pin to measure from.
 *  @param refs ADC reference to compare measurement to.
 *  @return Void.
 */
extern void adc_setup(const unsigned char admux, const unsigned char refs);

/** @brief Reads from ADC.
 *  @return 10-bit ADC measurement.
 */
extern unsigned short adc_read(void);

/* Call adc_setup(8,3) before calling this function!
   Returns temperature calculated from internal ATMega328p sensor.
   Temperature is very unprecise: ±10 C.
 */
/** @brief Measures temperature.
 *
 *  Call adc_setup(8,3) before calling this function.
 *  The temperature is calculated from the internal ATMega358P sensor.
 *  It is very imprecise ±10 C.
 *
 *  @return Temperature.
 */
extern float adc_temperature(void);

#endif /* _ADC_H */
