/** @file main.c
 *  @brief Main utility for ICM
 *
 *  This application runs the main utility for ICM.
 *  It reads sensor values, adjusts the light,
 *  and pushes the values to the cloud.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */

/* libc includes */
#include <stdio.h>

/* Type definitions includes */
#include "app/cloud_data.h"

/* AVR includes */
#include <avr/io.h>
#include <util/delay.h>

/* Communication includes */
#include "com/uart.h"

/* Driver includes */
#include "driver/pwm.h"

/* Sensor includes */
#include "sensor/ccs811.h"
#include "sensor/spl.h"

/* App includes */
#include "app/climate_evaluator.h"
#include "app/indoor_climate_monitor.h"
#include "app/sensor_controller.h"

/* Bit manipulation defines */
#define RSET(r, v) r |= _BV(v)    /* Set bit */
#define RCLR(r, v) r &= ~(_BV(v)) /* Clear bit */
#define RTOG(r, v) r ^= _BV(v)    /* Toggle bit */

/* Buffer for string manipulation */
static unsigned char buffer[128];

static void scale_setup(scale_t *const sc);
static void sensor_setup(sensor_data_t *const sd);
static void cloud_setup(cloud_data_t *const cd, const sensor_data_t *const sd,
                        const scale_t *const sc, const visible_data_t *const vd,
                        const network_data_t *const nd);

/** @brief Heartbeat function.
 *
 *  Toggles hearbeat LED and waits 500 ms.
 *
 *  @return Void.
 */
void live(void);

/** @brief Main ICM application.
 *  @return Should not return.
 */

int main(void) {
  RCLR(DDRD, PORTD2);
  RSET(PORTD, PORTD2);
  loop_until_bit_is_clear(PIND, PIND2);
  init();
  /* Initialize LED */
  RSET(DDRB, 5);

  cloud_data_t cd;
  scale_t sc[4];
  sensor_data_t sd[4];
  visible_data_t vd;
  network_data_t nd;

  scale_setup(sc);
  sensor_setup(sd);

  vd.sensor_fault = 0;
  vd.network_fault = 0;
  nd.fault = 0;

  while (1) {
    live();

    read_sensor_values(sd);

    unsigned short co2 = sd[0].value;

    union {
      unsigned long w;
      float f;
    } temp, humd, spl;

    humd.w = sd[1].value;
    spl.w = sd[2].value;
    temp.w = sd[3].value;

    sprintf((char *)buffer,
            "CO2: %i\r\nTemp: %.2f\r\nHumd: %.2f\r\nSPL: %.2f\r\n", co2, temp.f,
            humd.f, spl.f);
    uart_transmit_string(buffer);

    for (unsigned char i = 0; i < 4; ++i) {
      sc[i].value = evaluate_scale(&sc[i], &sd[i]);
    }
    vd.icq = compute_icq(sc);

    /*sprintf((char *)buffer,
            "\r\nCO2 Scale: %i\r\nTemp Scale: %i\r\nHumd Scale: %i\r\nSPL "
            "Scale: %i\r\nICQ: %i\r\n",
            sc[CO2].value, sc[Temperature].value, sc[Humidity].value,
            sc[SoundPL].value, vd.icq);
    uart_transmit_string(buffer);*/

    emit_color(&vd);

    unsigned char sensor_fault = 0;
    for (unsigned char i = 0; i < 4; ++i) {
      sensor_fault |= sd[0].fault;
    }
    vd.sensor_fault = sensor_fault;

    cloud_setup(&cd, sd, sc, &vd, &nd);
    send_data(&cd);
  }
}

void live(void) { RTOG(PORTB, 5); }

static void scale_setup(scale_t *const sc) {
  const scale_t co2 = {255, {800, 1500, 0, 0}, CO2};
  const scale_t hum = {255, {25, 30, 50, 60}, Humidity};
  const scale_t temp = {255, {10, 20, 25, 35}, Temperature};
  const scale_t spl = {255, {90, 100, 0, 0}, SoundPL};
  sc[0] = co2;
  sc[1] = hum;
  sc[2] = spl;
  sc[3] = temp;
}

static void sensor_setup(sensor_data_t *const sd) {
  const sensor_data_t co2 = {0, 0, UInt16, CO2};
  const sensor_data_t hum = {0, 0, Float32, Humidity};
  const sensor_data_t spl = {0, 0, Float32, SoundPL};
  const sensor_data_t temp = {0, 0, Float32, Temperature};
  sd[0] = co2;
  sd[1] = hum;
  sd[2] = spl;
  sd[3] = temp;
}

static void cloud_setup(cloud_data_t *const cd, const sensor_data_t *const sd,
                        const scale_t *const sc, const visible_data_t *const vd,
                        const network_data_t *const nd) {
  for (unsigned char i = 0; i < 4; ++i) {
    cd->sd[i] = sd[i];
    cd->sc[i] = sc[i];
  }
  cd->vd = *vd;
  cd->nd = *nd;
}
