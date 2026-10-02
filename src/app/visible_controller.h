/** @file visible_controller.h
 *  @brief Prototypes for handling the visible component of the ICM.
 *
 *  VisibleController takes the Indoor Climate Quality factor,
 *  and sets the PWMs for the LEDs accordingly.
 *
 *  @author Mikkel Isaksen Jensen
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef VISIBLE_CONTROLLER_H
#define VISIBLE_CONTROLLER_H

/* -- Includes -- */
/* App includes */
#include "cloud_data.h"

/** @brief Determines the hue to set the light to from the Indoor Climate
 *  Quality factor.
 *
 *  @param data Struct with data regarding the configuration of the colour
 * strip.
 *  @return Void.
 */
extern void vc_set_visible(visible_data_t *const data);

/** @brief Sets the PWMs according to the previously determined hue.
 *
 *  @return Void.
 */
extern void vc_show_visible(void);

#endif /* VISIBLE_CONTROLLER_H */
