/** @file init.h
 *  @brief Function prototypes for initializing the ICM.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _INIT_H
#define _INIT_H

/** @brief Initializes peripherals needed for the ICM.
 *
 *  Sets clock prescaler to 16. FCPU=1MHz.
 *  Initiliazes the peripherals needed for the ICM.
 *  Including I2C, SPI, UART.
 *
 *  @return Void.
 */
extern void init_program(void);

#endif
