#include <avr/io.h>
#include <avr/interrupt.h>
#include "twi_master_poll.h"
#include "status_codes.h"
#include "twi.h"

#define PRESC_MASK 0xF8 /* mask prescaler bits in TWSR */

void twi_master_init_400khz(void)
{
    unsigned char bitrate = (F_CPU / 400000UL - 16) / 2;
    twi_master_init(bitrate, 1);
}

void twi_master_init_100khz(void)
{
    unsigned char bitrate = (F_CPU / 100000UL - 16) / 2;
    twi_master_init(bitrate, 1);
}

void twi_master_init(unsigned char bitrate, unsigned char prescaler)
{
    TWBR = bitrate;
    switch (prescaler)
    {
        case 1:
            TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
            break;
        case 4:
            TWSR |= (1 << TWPS0);
            TWSR &= ~(1 << TWPS1);
            break;
        case 16:
            TWSR &= ~(1 << TWPS0);
            TWSR |= (1 << TWPS1);
            break;
        case 64:
            TWSR |= (1 << TWPS1) | (1 << TWPS0);
            break;
    }
}

int twi_master_send_start(void)
{
    twi_send_start();
    while (!(TWCR & (1 << TWINT)))
    {
    }
    if ((TWSR & PRESC_MASK) != START_SENT &&
        (TWSR & PRESC_MASK) != REP_START_SENT)
    {
        return -1;
    }
    return 0;
}

void twi_master_send_stop(void)
{
    twi_send_stop();
}

int twi_master_select_slave(unsigned char addr, unsigned char readwrite)
{
    if (readwrite != 1 && readwrite != 0)
    {
        return -1;
    }
    twi_write_byte((addr << 1) | readwrite);
    while (!(TWCR & (1 << TWINT)))
    {
    }
    if ((TWSR & PRESC_MASK) == SLA_W_SENT_ACK ||
        (TWSR & PRESC_MASK) == SLA_R_SENT_ACK)
    {
        return 0;
    }
    return -1;
}

int twi_master_write_byte(char byte)
{
    twi_write_byte(byte);
    while (!(TWCR & (1 << TWINT)))
    {
    }
    char status = TWSR & PRESC_MASK;
    if (status == MAST_DATA_SENT_ACK)
    {
        return 1;
    }
    else if (status == MAST_DATA_SENT_NACK)
    {
        return 0;
    }
    return -1;
}

int twi_master_write_bytes(const char *data, int nbytes)
{
    int status;
    while (nbytes > 0)
    {
        if ((status = twi_master_write_byte(*data++)) != 1)
        {
            break;
        }
        nbytes--;
    }
    return status;
}

int twi_master_read_bytes(char *data, int nbytes)
{
    if (nbytes < 1)
    {
        return -1;
    }
    while (nbytes > 1)
    {
        twi_read_byte_ACK(data++);
        while (!(TWCR & (1 << TWINT)))
        {
        }
        if ((TWSR & PRESC_MASK) == MAST_DATA_RECVD_ACK)
        {
            nbytes--;
        }
        else
        {
            return -1;
        }
    }
    twi_read_byte_NACK(data);
    while (!(TWCR & (1 << TWINT)))
    {
    }
    if ((TWSR & PRESC_MASK) == MAST_DATA_RECVD_NACK)
    {
        nbytes--;
    }
    else
    {
        return -1;
    }
    return 0;
}

int twi_master_read_from_device(unsigned char addr, char *data,
                                int nbytes)
{
    int res = 0;
    if (nbytes < 1)
    {
        return -1;
    }
    res = twi_master_send_start();
    if (res)
    {
        return -1;
    }
    res = twi_master_select_slave(addr, MASTER_READ);
    if (res)
    {
        return -1;
    }
    res = twi_master_read_bytes(data, nbytes);
    if (res)
    {
        return -1;
    }
    twi_master_send_stop();
    return 0;
}

int twi_master_read_from_device_register(unsigned char addr, unsigned char reg,
                                         char *data, int nbytes)
{
    int res = 0;
    if (nbytes < 1)
    {
        return -1;
    }
    res = twi_master_send_start();
    if (res)
    {
        return -1;
    }
    res = twi_master_select_slave(addr, MASTER_WRITE);
    if (res)
    {
        return -1;
    }
    res = twi_master_write_byte(reg);
    if (res)
    {
        return -1;
    }
    res = twi_master_send_start();
    if (res)
    {
        return -1;
    }
    res = twi_master_select_slave(addr, MASTER_READ);
    if (res)
    {
        return -1;
    }
    res = twi_master_read_bytes(data, nbytes);
    if (res)
    {
        return -1;
    }
    twi_master_send_stop();
    return 0;
}

int twi_master_write_to_device_register(unsigned char addr, unsigned char reg,
                                        const char *data, int nbytes)
{
    int res = 0;
    if (nbytes < 1)
    {
        return -1;
    }
    res = twi_master_send_start();
    if (res)
    {
        return -1;
    }
    res = twi_master_select_slave(addr, MASTER_WRITE);
    if (res)
    {
        return -1;
    }
    res = twi_master_write_byte(reg);
    if (res)
    {
        return -1;
    }
    res = twi_master_send_start();
    if (res)
    {
        return -1;
    }
    res = twi_master_select_slave(addr, MASTER_WRITE);
    if (res)
    {
        return -1;
    }
    res = twi_master_write_bytes(data, nbytes);
    if (res)
    {
        return -1;
    }
    twi_master_send_stop();
    return 0;
}
