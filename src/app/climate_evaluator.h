/** @file climate_evaluator.h
 *  @brief Protoypes for evaluating the parameter scales and ICQ of the ICM.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef CLIMATE_EVALUATOR_H
#define CLIMATE_EVALUATOR_H

/* -- Includes -- */
/* App includes */
#include "cloud_data.h"

/** @brief Determine scale for certain environmental parameter.
 *
 *  This function uses information regarding the scale to be applied to a
 *  certain measurement and the measurements value. It determines how good the
 *  parameter is by a scale from 0-255. The higher the better.
 *
 *  @param scale Information regarding the scale used for the parameter.
 *  @param data  Sensor data and measurement of the environmental parameter.
 *  @return Value of the scale.
 */
extern unsigned char evaluate_scale(scale_t *scale, const sensor_data_t *data);

/** @brief Computes the ICQ from the geometric average of all the scale values.
 *
 *  This function takes all the values of the scales and multiplies them
 *  together. Then it takes the fourth root and returns the result. The number
 *  scales given to the function should always be four for the function to
 *  work.
 *
 *  @param scale Array of scales.
 *  @return Value of ICQ.
 */
extern unsigned char compute_icq(const scale_t *scale);

#endif /* CLIMATE_EVALUATOR_H */
