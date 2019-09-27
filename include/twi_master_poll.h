#ifndef TWI_MASTER_POLL_H
#define TWI_MASTER_POLL_H

/**
 * @file twi_master_poll.h
 * @author Ranul Pallemulle
 * @date 14/10/2019
 * @brief TWI master receiver/transmitter working in polled mode.
 *
 * A TWI (I2C) master implementation that can be utilised in both the
 * transmitter mode and the receiver mode (as specified in the Atmel
 * documentation for their microcontrollers). The TWI interrupt flag
 * is polled after some action is taken on the TWI bus. The TWI status
 * register is then checked if the action was successful. Functions
 * that take some action on the bus return a value indicating whether
 * the action was successful.
 */

#define MASTER_READ 1
#define MASTER_WRITE 0

/**
 * @brief Call twi_master_init() with a bitrate such that SCL is set to
 * 400 kHz.
 *
 * The prescaler argument to twi_master_init() is 1. Remember to use the
 * correct pull up resistor value on the TWI bus.
 * SCL_freq = F_CPU/(16 + 2(TWBR)(PrescalerValue))
 */
void twi_master_init_400khz(void);

/**
 * @brief Call twi_master_init() with a bitrate such that SCL is set to
 * 100 kHz.
 *
 * The prescaler argument to twi_master_init() is 1. Remember to use the
 * correct pull up resistor value on the TWI bus.
 * SCL_freq = F_CPU/(16 + 2(TWBR)(PrescalerValue))
 */
void twi_master_init_100khz(void);

/**
 * @brief Start polled TWI master operation.
 *
 * Set the bit-rate register and the clock prescaler value. This sets
 * TWI to master mode.
 * @param bitrate The bitrate value to be assigned to the bitrate
 * register.
 * @param The prescaler value to use. Should be a value in {1,4,16,64}.
 */
void twi_master_init(unsigned char bitrate, unsigned char prescaler);

/**
 * @brief Put the start condition on the bus.
 * @return -1 if an error occurred, else 0.
 */
int twi_master_send_start(void);

/**
 * @brief Put the stop condition on the bus.
 *
 * This simply calls the basic TWI function twi_send_stop(). No status
 * codes are checked (because they don't exist).
 */
void twi_master_send_stop(void);

/**
 * @brief Select the slave to read from or write to.
 * @param addr The 7 bit slave address.
 * @param readwrite 1 if the slave is to be read from, 0 if the slave
 * is to be written to.
 * @return -1 if an error occurred, else 0.
 */
int twi_master_select_slave(unsigned char addr, unsigned char readwrite);

/**
 * @brief Write a single byte to the bus.
 * @param byte The byte to write.
 * @return 1 if the slave ACKed the byte. 0 if the slave NACKed the
 * byte. -1 if an error occurred.
 */
int twi_master_write_byte(char byte);

/**
 * @brief Write multiple bytes of data to the bus.
 * @param data Pointer to the first byte of the data to be sent.
 * @param nbytes Number of bytes to write
 * @return 1 if the slave ACKed the last byte sent. 0 if the slave
 * NACKed any bytes. -1 if an error occurred.
 */
int twi_master_write_bytes(const char *data, int nbytes);

/**
 * @brief Read multiple bytes from the bus.
 * @param data Pointer to the first byte of the data buffer to write
 * the received data into.
 * @param nbytes Number of bytes to receive.
 * @return -1 if an error occurred, else 0.
 */
int twi_master_read_bytes(char *data, int nbytes);

/**
 * @brief Read multiple bytes from a specific slave device.
 *
 * This is a higher level function in that it sends the start signal,
 * sends the device address, reads the data and finally sends the stop
 * signal. It is assumed that bus initialisation has been done already.
 * @param addr The slave device address.
 * @param data Pointer to the first byte of the data buffer to write
 * the received data into.
 * @param nbytes Number of bytes to receive.
 * @return -1 if an error occurred, else 0.
 */
int twi_master_read_from_device(unsigned char addr, char *data,
                                int nbytes);

/**
 * @brief Read multiple bytes from a register of a specific slave
 * device.
 *
 * This is a higher level function in that it sends the start signal,
 * sends the device address, sends the register address, reads the
 * data, and finally sends the stop signal. It is assumed that bus
 * initialisation has been done already.
 * @param addr The slave device address.
 * @param reg The register address in the slave device to read from.
 * @param data Pointer to the first byte of the data buffer to write
 * the received data into.
 * @param nbytes Number of bytes to receive.
 * @return -1 if an error occurred, else 0.
 */
int twi_master_read_from_device_register(unsigned char addr, unsigned char reg,
                                         char *data, int nbytes);

/**
 * @brief Write multiple bytes to a register of a specific slave
 * device.
 *
 * This is a higher level function in that it sends the start signal,
 * sends the device address, sends the register address, writes the
 * data, and finally sends the stop signal. It is assumed that bus
 * initialisation has been done already.
 * @param addr The slave device address.
 * @param reg The register address in the slave device to write to.
 * @param data Pointer to the first byte of the data buffer.
 * @param nbytes Number of bytes to write.
 * @return -1 if an error occurred, else 0.
 */
int twi_master_write_to_device_register(unsigned char addr, unsigned char reg,
                                        const char *data, int nbytes);

#endif /* TWI_MASTER_POLL_H */
