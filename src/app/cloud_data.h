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
  CO2 = 0,
  Humidity = 1,
  SoundPL = 2,
  Temperature = 3,
} id_t;

/** @brief Datatype type
 *  The datatype defines whether a sensor measurement is a floating point
 *  number or an integer.
 */
typedef enum data_e {
  Float32 = 1,
  UInt16 = 2,
} data_t;

/** @brief Color type
 *  The color type defines color by intensity from 0-255 in three channels:
 *  Red, white, and blue.
 */
typedef struct color_s {
  unsigned char red;
  unsigned char white;
  unsigned char blue;
} color_t;

#define SET_COLOR(c, r, w, b)                                                  \
  c.red = r;                                                                   \
  c.white = w;                                                                 \
  c.blue = b

/** @brief Scale type
 *  The scale type contains necessary information for an evaluation scale.
 *  It contains the value from 0-255.
 *  The two or four limits for the ranges.
 *  An ID designating the type of sensor the scale will be performed on.
 */
typedef struct scale_s {
  unsigned char value;
  unsigned int limit[4];
  id_t id;
} scale_t;

/** @brief SensorData type
 * It contains an array of four bytes to contain the value.
 * A boolean value that is true if the sensor has failed, false otherwise.
 * A datatype signifying what datatype the value must be interpreted as.
 * An ID designtaing the type of sensor.
 */
typedef struct sensor_data_s {
  unsigned long value;
  unsigned char fault;
  data_t type;
  id_t id;
} sensor_data_t;

/** @brief VisibleData type
 * It contains a color for current configuration and one for target
 * configuration. A byte containing the ICQ, and one for the masking value with
 * the regards to the ambient lighting. Booleans for checking for sensor and
 * network faults.
 */
typedef struct visible_data_s {
  color_t current_color;
  color_t target_color;
  unsigned char icq;
  unsigned char mask;
  unsigned char sensor_fault;
  unsigned char network_fault;
} visible_data_t;

/** @brief NetworkData type
 * It contains boolean for checking for network fault.
 */
typedef struct network_data_s {
  unsigned char fault;
} network_data_t;

/** @brief CloudData type
 * Struct containing all needed data for the ICM.
 * It contains four SensorData and Scale structs.
 * One VisibleData struct and one NetworkData struct.
 */
typedef struct cloud_data_s {
  sensor_data_t sd[4];
  scale_t sc[4];
  visible_data_t vd;
  network_data_t nd;
} cloud_data_t;

#endif /* CLOUD_DATA_H */
