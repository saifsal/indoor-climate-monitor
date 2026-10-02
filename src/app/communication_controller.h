/** @file communication_controller.h
 *  @brief Communication controller.
 *
 *  The cloud controller sends the climate factors
 *  to the communication module via a I2C interface.
 *
 *  @author Fróði Vestergaard Dam
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _COMMUNICATION_CONTROLLER_H
#define _COMMUNICATION_CONTROLLER_H

/* -- Includes -- */
/* App includes */
#include "cloud_data.h"

/** @brief Send climate factors to communication module via I2C
 *  @return Success
 */
extern unsigned char push_data(const cloud_data_t *const data);

#endif /* _COMMUNICATION_CONTROLLER_H */
