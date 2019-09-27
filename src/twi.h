#ifndef TWI_H
#define TWI_H

#include <avr/io.h>
/**
 * @file twi.h
 * @author Ranul Pallemulle
 * @date 14/10/2019
 * @brief Basic TWI (I2C) functions.
 *
 * This file contains the definitions of convenience functions that
 * can be used to implement master or slave functionality. These
 * functions are device-agnostic.
 */

#define RESPOND_TO_GEN_CALL 1
#define IGNORE_GEN_CALL 0

/**
 * @brief Set the SCL frequency.
 *
 * This function should be called by a master. If the prescaler value
 * provided is not in the valid range, no action is taken.
 * @param bit_rate The TWI bit rate.
 * @param prescaler The TWI prescaler value. Should be a value in {1,4,16,64}.
 */
inline void twi_set_scl(unsigned char bit_rate, unsigned char prescaler)
{
    TWBR = bit_rate;
    switch (prescaler)
    {
        case 1:
            TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
            break;
        case 4:
            TWSR &= ~(1 << TWPS1);
            TWSR |= (1 << TWPS0);
            break;
        case 16:
            TWSR |= (1 << TWPS1);
            TWSR &= ~(1 << TWPS0);
            break;
        case 64:
            TWSR |= (1 << TWPS1) | (1 << TWPS0);
            break;
    }
}

/**
 * @brief Set the address on a slave device.
 *
 * This function should be called by a slave. If the value for
 * gen_call_enable is invalid, no action is taken.
 * @param addr The 7 bit address of the slave.
 * @param gen_call_enable Whether the slave should respond to the
 * general call address. This should be a value of 1 or 0.
 */
inline void twi_set_self_slave_address(unsigned char addr,
                                       unsigned char gen_call_enable)
{
    if (gen_call_enable != RESPOND_TO_GEN_CALL &&
        gen_call_enable != IGNORE_GEN_CALL)
    {
        return; /* invalid argument */
    }
    TWAR = (addr << 1) | gen_call_enable;
}

/**
 * @brief Put the start signal on the TWI bus.
 *
 * This function should be called by a master.
 */
inline void twi_send_start(void)
{
    /* TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE); */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
}

/**
 * @brief Put the stop signal on the TWI bus.
 *
 * This function should be called by a master.
 */
inline void twi_send_stop(void)
{
    /* TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (1 << TWIE); */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

/**
 * @brief Write a data or address byte to the bus.
 * @param byte The byte to write to the bus.
 */
inline void twi_write_byte(char byte)
{
    TWDR = byte;
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
}

/**
 * @brief Clear the TWINT flag and enable acknowledge.
 */
/* clear TWINT and enable acknowledge */
inline void twi_clear_flag(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
}

/**
 * @brief Disable TWI interrupt.
 */
inline void twi_disable(void)
{
    TWCR &= ~(1 << TWIE);
}

/**
 * @brief Read a data byte and send back an ACK.
 * @param byteptr Pointer to the memory to write the received data.
 */
inline void twi_read_byte_ACK(char *byteptr)
{
    *byteptr = TWDR;
    TWCR     = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
}

/* read a data byte into a char and send back a NACK */
/**
 * @brief Read a data byte and send back a NACK.
 * @param byteptr Pointer to the memory to write the received data.
 */
inline void twi_read_byte_NACK(char *byteptr)
{
    *byteptr = TWDR;
    TWCR     = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
}

#endif /* TWI_H */
