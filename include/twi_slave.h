#ifndef TWI_SLAVE_H
#define TWI_SLAVE_H

/**
 * @file twi_slave.h
 * @author Ranul Pallemulle
 * @date 14/10/2019
 * @brief Interrupt driven TWI slave receiver/transmitter.
 *
 * A TWI (I2C) slave implementation that can be utilised in both the
 * transmitter mode and the receiver mode (as specified in the Atmel
 * documentation for their microcontrollers). If the device is to be
 * used as a receiver, the user should specify a callback function to
 * be called when data is received. Likewise, the set_data function
 * should be used to specify data to be sent if the transmitter mode
 * is used. The typical usage is to set the callback function, call
 * slave_init(), set the mode, and then enable global interrupts using
 * sei(). The specific status codes for the hardware must be specified
 * in the file "status_codes.h".
 */
#ifndef MAXBUFFLEN
#define MAXBUFFLEN 30 /* max number of bytes to receive */
#endif                /* MAXBUFFLEN */

/**
 * @brief Set the user function to be called when data is received.
 * @param func Pointer to the user function. The first argument of the
 * function is a pointer to the data. The second argument is the
 * number of bytes received.
 */
void twi_slave_set_callback(void (*func)(char *, unsigned char));

/**
 * @brief Copy data to an internal buffer so that it can be sent on
 * the next transmission.

 * TWI interrupts are disabled before data is copied over. TWI
 * interrups are not re-enabled after data is copied over.
 * @param data Pointer to the first byte of the data to be sent.
 * @param len Number of bytes to be copied starting from the first
 * byte.
 */
void twi_slave_set_data(const char *data, int len);

/**
 * @brief Set the current mode to transmitter mode.
 *
 * Note that the mode may change internally (e.g if master wants to
 * receive data, the mode will change internally to transmitter).
 */
void twi_slave_transmitter_mode(void);

/**
 * @brief Set the current mode to receiver mode.
 *
 * Note that the mode may change internally (e.g if master wants to
 * receive data, the mode will change internally to transmitter).
 */
void twi_slave_receiver_mode(void);

/**
 * @brief Begin slave operation.
 *
 * Calling this function enables TWI interrupts. If set_mode() was not
 * called prior to calling this function, the slave starts by default
 * in receiver mode.
 * @param addr The 7 bit address to use as the slave address.
 * @param gen_call_response Whether to respond to the general call
 * address on the TWI bus. Use the macro values from twi.h for this
 * argument.
 */
void twi_slave_init(unsigned char addr, unsigned char gen_call_response);

#endif /* TWI_SLAVE_H */
