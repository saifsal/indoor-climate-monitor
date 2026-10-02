/** @file indoor_climate_monitor.c
 *  @brief Main utility
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "indoor_climate_monitor.h"

/* Driver includes */
#include "../driver/init.h"

/* App includes */
#include "cloud_data.h"
#include "communication_controller.h"
#include "sensor_controller.h"
#include "visible_controller.h"

void init(void) { init_program(); }

void read_sensor_values(sensor_data_t *const data) {
  take_measurements();

  for (unsigned char i = 0; i < 4; ++i) {
    get_measurement(&data[i]);
  }
}

void emit_color(visible_data_t *const data) {
  if (data->sensor_fault) {
    SET_COLOR(data->target_color, 0, 0, 0);
  } else if (data->network_fault) {
    SET_COLOR(data->target_color, 0, 0, 255);
  }
  vc_set_visible(data);

  vc_show_visible();
}

void send_data(const cloud_data_t *const data) {
  unsigned char success = push_data(data);

  if (!success) {
    for (unsigned char i = 0; i < 5; ++i) {
      success = push_data(data);
      if (success)
        break;
    }
  }
}

void save_icq_parameters(void) {}
