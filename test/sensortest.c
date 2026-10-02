/** @file sensor_controller.c
 *  @brief Handler for the sensors of the ICM.
 *
 *  SensorController handles and reads from the sensors.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
//#include "sensor_controller.h"
#include "cloud_data.h"

/* libc includes */
#include <string.h>
#include <stdio.h>

/* Sensor includes */
/* #include "../sensor/am2320.h"
#include "../sensor/ccs811.h" */

unsigned char get_measurement(sensor_data_t * const sd);

int main(void) {
  sensor_data_t data[5];
  data[0].id = co2;
  data[1].id = lux;
  data[2].id = humidity;
  data[3].id = temperature;
  data[4].id = spl;

  unsigned short tmpS;
  float tmpF;
  for (unsigned char i = 0; i < 5; ++i) {
    get_measurement(&data[i]);
    if (data[i].type == float32) {
      memcpy(&tmpF, &data[i].value, 4);
      printf("Value read is: %f (float) from %i\n", tmpF, data[i].id);
    } else if (data[i].type == uint16) {
      memcpy(&tmpS, &data[i].value, 2);
      printf("Value read is: %i (unsigned short) from %i\n",tmpS, data[i].id);
    }
  }
  return 0;
}


static unsigned char am2320_err = 0; /* Error for temperature humidity sensor */
static unsigned char ccs811_err = 0; /* Error for CO2 sensor */

/** @brief Registers fault in sensor
 *  @return Void.
 */
void register_sensor_fault(sensor_data_t * const sd);

/* unsigned char take_measurements(void) {
  if (!am2320_read()) {
    am2320_err = 1;
    return 0;
  }
  if (ccs811_read_data()) {
    ccs811_err = 1;
    return 0;
  }
  return 1;
} */

unsigned char get_measurement(sensor_data_t * const sd) {
  static unsigned short i = 0;
  ++i;
  switch (sd->id) {
  case humidity: {
    if (am2320_err) {
      register_sensor_fault(sd);
      return 0;
    }
    const float val = (float)i; //am2320_humidity();
    memcpy(&sd->value, &val, sizeof(float));
    sd->type = float32;
    sd->fault = 0;
    return 1;
    break;
  }
  case temperature: {
    if (am2320_err) {
      register_sensor_fault(sd);
      return 0;
    }
    const float val = (float)i; //am2320_temperature();
    memcpy(&sd->value, &val, sizeof(float));
    sd->type = float32;
    sd->fault = 0;
    return 1;
    break;
  }
  case co2: {
    if (ccs811_err) {
      register_sensor_fault(sd);
      return 0;
    }
    const unsigned short val = i; //ccs811_co2();
    memcpy(&sd->value, &val, sizeof(unsigned short));
    sd->type = uint16;
    sd->fault = 0;
    return 1;
    break;
  }
  case spl: {
    const float val = (float)i;
    memcpy(&sd->value, &val, sizeof(float));
    sd->type = float32;
    sd->fault = 0;
    return 1;
    break;
  }
  case lux:
  default:
    return 0;
    break;
  }
}

void register_sensor_fault(sensor_data_t * const sd) { sd->fault = 1; }
