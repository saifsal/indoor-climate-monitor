/** @file pwm.h
 *  @brief Function prototypes PWM.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _PWM_H
#define _PWM_H

/** @brief Sets intensity for red LED.
 *
 *  @param duty Intensity from 0 to 255.
 *  @return Void.
 */
extern void pwm_red(const unsigned char duty);

/** @brief Sets intensity for white LED.
 *
 *  @param duty Intensity from 0 to 255.
 *  @return Void.
 */
extern void pwm_white(const unsigned char duty);

/** @brief Sets intensity for blue LED.
 *
 *  @param duty Intensity from 0 to 255.
 *  @return Void.
 */
extern void pwm_blue(const unsigned char duty);

#endif /* _PWM_H */
