/** @file indoor_climate_monitor.h
 *  @brief Prototypes for the main utility
 *
 *  IndoorClimateMonitor has prototypes to handle each use case of the ICM.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef INDOOR_CLIMATE_MONITOR_H
#define INDOOR_CLIMATE_MONITOR_H

/* -- Includes -- */
/* App includes */
#include "cloud_data.h"

/** @brief Initializes all needed hardware.
 *
 *  @return Void.
 */
extern void init(void);

/** @brief Reads measurements from all sensors.
 *
 *  @param data Array of struct containing sensor data.
 *  @return Void.
 */
extern void read_sensor_values(sensor_data_t *const data);

/** @brief Emits wanted color.
 *
 *  @param data Struct containing visible data.
 *  @return Void.
 */
extern void emit_color(visible_data_t *const data);

/** @brief Pushes the measurement data and ICQ value to cloud.
 *
 *  @param data Struct containing data to be send.
 *  @return Void.
 */
extern void send_data(const cloud_data_t *const data);

/** @brief Saves the new limits wanted from the cloud service.
 *
 *  @return Void.
 */
extern void save_icq_parameters(void);

#endif /* INDOOR_CLIMATE_MONITOR_H */
