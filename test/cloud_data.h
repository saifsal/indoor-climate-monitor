/** @file cloud_data.h
 *  @brief Type definitions for CloudData.
 *
 *  Custom data types making the ICM simpler and easier.
 *  ID type to determine the type of sensor.
 *  Data type to determine whether float or int is measured.
 *  Scale type containing value of scale, limits for configuring, and id.
 *  SensorData type containing value, fault, data type, and id.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef CLOUD_DATA_H
#define CLOUD_DATA_H

/** @brief ID type
 *  The ID type is an enum defining the type of sensor.
 *  For the ICM CO2, lux, humidity, temperature, and SPL are sufficient.
 */
typedef enum id_e {
  co2,
  lux,
  humidity,
  temperature,
  spl,
} id_t;

/** @brief Datatype type
 *  The datatype defines whether a sensor measurement is a floating point
 *  number or an integer.
 */
typedef enum data_e {
  float32,
  uint16,
} data_t;

/** @brief Scale type
 *  The scale type contains necessary information for an evaluation scale.
 *  It contains the value from 0-255.
 *  The two or four limits for the ranges.
 *  An ID designating the type of sensor the scale will be performed on.
 */
typedef struct scale_s {
  unsigned char value;
  int limit[4];
  id_t id;
} scale_t;

/** @brief SensorData type
 * It contains an array of four bytes to contain the value.
 * A boolean value that is true if the sensor has failed, false otherwise.
 * A datatype signifying what datatype the value must be interpreted as.
 * An ID designtaing the type of sensor.
 */
typedef struct sensor_data_s {
  unsigned char value[4];
  unsigned char fault;
  data_t type;
  id_t id;
} sensor_data_t;

#endif /* CLOUD_DATA_H */
