/** @file sensor_controller.h
 *  @brief Prototypes for handling the sensors of the ICM.
 *
 *  SensorController handles and reads from the sensors.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

/* -- Includes -- */
/* App includes */
#include "cloud_data.h"

/** @brief Take measurements with all the available sensors.
 *
 *  @return True with success and false with failure.
 */
extern unsigned char take_measurements(void);

/** @brief Gets the measurement.
 *
 *  The function looks at the data parameter to determine which sensor to
 *  read from. It then stores all necessary data for measurement into the data
 *  parameter.
 *
 *  @param data SensorData reference. All data pertaining to the measurement
 *              is stored here after the function call.
 *  @return True with success and false with failure.
 */
extern unsigned char get_measurement(sensor_data_t *data);

#endif /* SENSOR_CONTROLLER_H */
