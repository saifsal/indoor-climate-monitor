/** @file sensor_controller.c
 *  @brief Handler for the sensors of the ICM.
 *
 *  SensorController handles and reads from the sensors.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "sensor_controller.h"

/* Sensor includes */
#include "../sensor/am2320.h"
#include "../sensor/ccs811.h"
#include "../sensor/spl.h"

static unsigned char am2320_err = 0; /* Error for temperature humidity sensor */
static unsigned char ccs811_err = 0; /* Error for CO2 sensor */

/** @brief Registers fault in sensor
 *  @return Void.
 */
void register_sensor_fault(sensor_data_t *sd);

unsigned char take_measurements(void) {
  am2320_err = !am2320_read();
  ccs811_err = !!ccs811_read_data();

  return (am2320_err || ccs811_err);
}

unsigned char get_measurement(sensor_data_t *sd) {
  union {
    unsigned long w;
    float f;
  } val;

  switch (sd->id) {
  case CO2: {
    if (ccs811_err) {
      register_sensor_fault(sd);
      return 0;
    }
    sd->value = ccs811_co2();
    sd->type = UInt16;
    sd->fault = 0;
    return 1;
    break;
  }
  case Humidity: {
    if (am2320_err) {
      register_sensor_fault(sd);
      return 0;
    }
    val.f = am2320_humidity();
    sd->value = val.w;
    sd->type = Float32;
    sd->fault = 0;
    return 1;
    break;
  }
  case SoundPL: {
    val.f = (float)(spl_read());
    sd->value = val.w;
    sd->type = Float32;
    sd->fault = 0;
    return 1;
    break;
  }
  case Temperature: {
    if (am2320_err) {
      register_sensor_fault(sd);
      return 0;
    }
    val.f = am2320_temperature();
    sd->value = val.w;
    sd->type = Float32;
    sd->fault = 0;
    return 1;
    break;
  }
  }
  return 0;
}

void register_sensor_fault(sensor_data_t *sd) { sd->fault = 1; }
