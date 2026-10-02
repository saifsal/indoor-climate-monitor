/** @file ccs811.h
 *  @brief Prototypes for CCS811 sensor.
 *
 *  The CCS811 sensor is the CO2 sensor.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _CCS811_H
#define _CCS811_H

/** @brief Startup the CCS811 sensor.
 *  @return True if successful, false otherwise.
 */
extern unsigned char ccs811_start(void);

/** @brief Get CO2 measurement from CCS811.
 *  @return CO2 measurement in ppm.
 */
extern unsigned short ccs811_co2(void);

/** @brief Get toxic volatile organic gas count (TVOC) measurement from CCS811.
 *  @return TVOC measurement in ppm.
 */
extern unsigned short ccs811_tvoc(void);

/** @brief Read measurement data from CCS811.
 *  @return Zero if successful, error ID otherwise.
 */
extern unsigned char ccs811_read_data(void);

#endif /* _CCS811_H */
