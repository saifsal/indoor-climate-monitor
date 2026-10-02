/** @file visible_controller.c
 *  @brief Handler of the visible component of the ICM.
 *
 *  @author Mikkel Isaksen Jensen
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "visible_controller.h"

#include <stdio.h>

/* Communication includes */
#include "../com/uart.h"

/* Driver includes */
#include "../driver/pwm.h"

/* Sensor includes */
#include "../sensor/ldr.h"

#define INCREMENT 5

static color_t target_color = {0, 255, 0};
static color_t current_color = {0, 255, 0};

static void change_color(color_t *const c, const color_t *const t);

void vc_set_visible(visible_data_t *const data) {
  unsigned char red = data->target_color.red;
  unsigned char white = data->target_color.white;
  unsigned char blue = data->target_color.blue;

  if (!(data->sensor_fault || data->network_fault)) {
    red = 255 - data->icq;
    white = data->icq;
    blue = 0;
  }

  /* Adjust intensity with light sensor value */
  const unsigned char mask = (255 - ldr_read());

  unsigned char buf[128];
  sprintf((char *)buf, "LDR: %i\r\n", mask);
  uart_transmit_string(buf);

  red = (mask * red) >> 8;
  white = (mask * white) >> 8;
  blue = (mask * blue) >> 8;

  red = red - (red % INCREMENT);
  white = white - (white % INCREMENT);

  SET_COLOR(target_color, red, white, blue);
  /*sprintf((char *)buf, "Target\tR: %i, W: %i, B: %i\r\n", target_color.red,
          target_color.white, target_color.blue);
  uart_transmit_string(buf);*/

  change_color(&current_color, &target_color);
  /* sprintf((char *)buf, "Current\tR: %i, W: %i, B: %i\r\n", current_color.red,
          current_color.white, current_color.blue);
  uart_transmit_string(buf); */

  data->current_color = current_color;
  data->target_color = target_color;
  data->mask = mask;
}

void vc_show_visible(void) {
  pwm_red(current_color.red);
  pwm_white(current_color.white);
  pwm_blue(current_color.blue);
}

#define ADJUST(c, t)                                                           \
  do {                                                                         \
    if ((t > c) && (c < 255)) {                                                \
      c += INCREMENT;                                                          \
    } else if ((t < c) && (c > 0)) {                                           \
      c -= INCREMENT;                                                          \
    }                                                                          \
  } while (0)

static void change_color(color_t *const c, const color_t *const t) {
  /* Increase or decrease red color according to current condition */
  ADJUST(c->red, t->red);
  /* Same for white color */
  ADJUST(c->white, t->white);
  /* Same for blue color */
  ADJUST(c->blue, t->blue);
}
