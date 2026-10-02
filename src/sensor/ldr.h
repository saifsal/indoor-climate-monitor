/** @file ldr.h
 *  @brief Prototypes for LDR sensor.
 *
 *  The LDR sensor is the light or lux sensor.
 *
 *  @author Mikkel Isaksen Jensen
 *  @author Saif Salih
 *  @bug No known bugs.
 */
#ifndef _LDR_H
#define _LDR_H

/** @brief Read measurement from LDR.
 *  @return LDR measurement 0-255.
 */
extern unsigned char ldr_read(void);

#endif /* _LDR_H */
