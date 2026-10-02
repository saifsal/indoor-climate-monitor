/** @file am2320.h
 *  @brief Prototypes for AM2320 sensor.
 *
 *  The AM2320 sensor is the temperature and humidity sensor.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _AM2320_H
#define _AM2320_H

/** @brief Read temperature and humidity measurements from AM2320.
 *  @return True if successful, false otherwise.
 */
extern unsigned char am2320_read(void);

/** @brief Get humidity measurement from AM2320.
 *  @return Humidity in relative humidity percentage RH%.
 */
extern float am2320_humidity(void);

/** @brief Get temperature measurement from AM2320.
 *  @return Temperature in degrees Celsius.
 */
extern float am2320_temperature(void);

#endif /* _AM2320_H */
