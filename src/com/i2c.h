/** @file i2c.h
 *  @brief Function prototypes for the I2C driver.
 *
 *  @author Saif Salih
 *  @bug No known bugs.
 */

#ifndef _I2C_H
#define _I2C_H

#define ACK 1 /* Return when ACK */
#define NAK 0 /* Return when NAK */

#define I2C_READ 1  /* Added to address when reading */
#define I2C_WRITE 0 /* Added to address when writing */

/** @brief Start I2C transmission.
 *
 *  @param address 7-bit address of other I2C device.
 *  @return ACK or NAK.
 */
extern unsigned char i2c_start(const unsigned char address);

/** @brief Stop I2C transmission.
 *
 *  @return Void.
 */
extern void i2c_stop(void);

/** @brief Write byte through I2C.
 *
 *  Write byte of data to I2C device set when transmission began.
 *
 *  @param data Byte of data.
 *  @return ACK or NAK.
 */
extern unsigned char i2c_write(const unsigned char data);

/** @brief Read byte from I2C.
 *
 *  Read byte of data from I2C device set when transmission began.
 *
 *  @param Whether ACK or NAK is expected.
 *  @return Byte of data.
 */
extern unsigned char i2c_read(const unsigned char ack);

#endif /* _I2C_H */
