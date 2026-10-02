/** @file spl.h
 *  @brief SPL sensor.
 *
 *  The SPL sensor is the sensor for sound pressure level.
 *
 *  @author Fróði Vestergaard Dam
 *  @author Saif Salih
 *  @bug No known bugs.
 */

/* -- Includes -- */
#include "spl.h"

/* libc includes */
#include <math.h>

/* Drive includes */
#include "../driver/adc.h"

#define SAMPLES 1874
#define SPL_SLOW 8
//#define SPL_REF 94.0
//#define ADC_COUNT_REF 626.0

double spl_read(void) {
  unsigned int splSum = 0;
  unsigned int splMean = 0;
  double spl = 0;

  /* Get ADC peak-peak value 8 times = 1s */
  for (int i = 0; i < SPL_SLOW; ++i) {
    splSum += spl_raw_read();
  }

  /* Calculate splMean */
  splMean = splSum / SPL_SLOW;

  /* Calculate dB value based on calibration factors */
  // spl = ((20.0 * log10((double)splMean / ADC_COUNT_REF)) + SPL_REF);

  /* Calculate dB value based on logarithmic regression */
  spl = (26.057 * log(splMean)) -79.819;

  return spl;
}

unsigned int spl_raw_read(void) {
  /* Initialise ADC to channel 2 and 5V ref */
  adc_setup(2, 1);

  unsigned int sample = 0;       /* Value of sample */
  unsigned int minSample = 1023; /* Minimum value sampled */
  unsigned int maxSample = 0;    /* Maximum value sampled */
  unsigned int peak_peak = 0;    /* Difference between minimum and maximum */

  /* Get samples for 125ms */
  for (unsigned int i = 0; i < SAMPLES; ++i) {
    /* Measure new sample. */
    sample = adc_read();

    /* Adjust minimum value if necessary*/
    if (sample < minSample) {
      minSample = sample;
    }

    /* Adjust maximum value if necessary */
    if (sample > maxSample) {
      maxSample = sample;
    }
  }

  /* calculate peak-peak value */
  peak_peak = maxSample - minSample;
  return peak_peak;
}
