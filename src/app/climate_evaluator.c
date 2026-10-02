/** @file climate_evaluator.c
 *  @brief Evaluator for the parameter scales and ICQ of the ICM.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "climate_evaluator.h"

/** @brief Calculates squareroot by binary comparision.
 *  @param num Input number.
 *  @return Squareroot of input number.
 */
static unsigned long bsqrt(unsigned long num);

/** @brief Calculates the scale for a plateau type parameter.
 *
 *  An example of a plateau type parameter is temperature.
 *  Humans are comfortable with a temperature between 20-25 C.
 *  This is the high point. The quality of the environment decreases
 *  as the temperature falls below 20 C, or rises above 25 C.
 *
 *  @param scale Information regarding scale.
 *  @param meas Measurement value.
 *  @return Value of scale.
 */
static unsigned char plateau(scale_t *scale, const float meas);

/** @brief Calculates the scale for a slope type parameter.
 *
 *  An example of a slope type parameter is CO2 level.
 *  Humans are comfortable with 800 ppm or lower.
 *  This is the high point. The quality of the environment decreases
 *  as the CO2 level rises above 800 ppm.
 *
 *  @param scale Information regarding scale.
 *  @param meas Measurement value.
 *  @return Value of scale.
 */
static unsigned char slope_f(scale_t *scale, const float meas);
static unsigned char slope_i(scale_t *scale, const unsigned short meas);

unsigned char evaluate_scale(scale_t *s, const sensor_data_t *d) {
  union {
    unsigned long w;
    float f;
  } val;

  switch (s->id) {
  case Humidity:
  case Temperature: {
    val.w = d->value;
    return plateau(s, val.f);
    break;
  }
  case CO2: {
    val.w = d->value;
    return slope_i(s, val.w);
    break;
  }
  case SoundPL: {
    val.w = d->value;
    return slope_f(s, val.f);
    break;
  }
  default:
    return 255;
    break;
  }
}

unsigned char compute_icq(const scale_t *s) {
  unsigned long prod[4];
  for (unsigned char i = 0; i < 4; ++i) {
    prod[i] = (unsigned long)(s[i].value);
  }
  const unsigned long prodres = prod[0] * prod[1] * prod[2] * prod[3];
  return (unsigned char)(bsqrt(bsqrt(prodres)));
}

static unsigned long bsqrt(unsigned long num) {
  unsigned long res = 0;
  unsigned long bit = (unsigned long)(1) << 30;

  while (bit > num) {
    bit >>= 2;
  }

  while (bit != 0) {
    if (num >= res + bit) {
      num -= res + bit;
      res = (res >> 1) + bit;
    } else {
      res >>= 1;
    }
    bit >>= 2;
  }
  return res;
}

static unsigned char plateau(scale_t *s, const float meas) {
  s->value = 0;
  if ((meas > (float)(s->limit[0])) && (meas < (float)(s->limit[3]))) {
    if (meas < (float)(s->limit[1])) {
      const double scalar = 1.0 / ((float)(s->limit[1]) - (float)(s->limit[0]));
      const double scale = scalar * (meas - (float)(s->limit[0]));
      s->value = (unsigned char)(255.0 * scale);
    } else if (meas < (float)s->limit[2]) {
      s->value = 255;
    } else {
      const double scalar = 1.0 / ((float)(s->limit[3]) - (float)(s->limit[2]));
      const double scale = scalar * (meas - (float)(s->limit[2]));
      s->value = 255 - (unsigned char)(255.0 * scale);
    }
  }
  return s->value;
}

static unsigned char slope_f(scale_t *s, const float meas) {
  s->value = 0;
  if (meas < (float)(s->limit[0])) {
    s->value = 255;
  } else if (meas < (float)(s->limit[1])) {
    const int sub =
        ((255 * ((int)meas - s->limit[0])) / (s->limit[1] - s->limit[0]));
    s->value = 255 - sub;
  }
  return s->value;
}

static unsigned char slope_i(scale_t *s, const unsigned short meas) {
  s->value = 0;
  if (meas < s->limit[0]) {
    s->value = 255;
  } else if (meas < s->limit[1]) {
    const int sub =
        ((255 * (meas - s->limit[0])) / (s->limit[1] - s->limit[0]));
    s->value = 255 - sub;
  }
  return s->value;
}
