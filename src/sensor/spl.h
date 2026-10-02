/** @file spl.h
 *  @brief Prototypes for SPL sensor.
 *
 *  The SPL sensor is the sensor for sound pressure level.
 *
 *  @author Fróði Vestergaard Dam
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _SPL_H
#define _SPL_H

/** @brief Measures the sound pressure level.
 *  @return SPL value in dBSPL.
 *          The device has been manually calibrated previously.
 */
extern double spl_read(void);

/** @brief Measures the sound pressure level.
 *  @return Raw measurement counts.
 *          I.e. the max peak-peak difference measured during the sampling.
 */
extern unsigned int spl_raw_read(void);

#endif /* _SPL_H */
