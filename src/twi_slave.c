#include <avr/interrupt.h>
#include "twi_slave.h"
#include "status_codes.h"
#include "twi.h"

/**
 * @file twi_slave.c
 * @author Ranul Pallemulle
 * @date 14/10/2019
 * @brief Implementation file for interrupt driven TWI slave
 * receiver/transmitter.
 */

#define NULL 0
#define MIN(x, y) (x < y ? x : y)
#define TRANSMITTER 1
#define RECEIVER 0

static void (*call_back)(char *data, unsigned char len);
static char databuf[MAXBUFFLEN];
static char *buffptr;
static int nbytes = 0; /* number of bytes of data in databuf */
static char mode  = RECEIVER;

void twi_slave_set_callback(void (*func)(char *, unsigned char))
{
    call_back = func;
}

void twi_slave_set_data(const char *data, int len)
{
    twi_disable();
    int true_len = MIN(len, MAXBUFFLEN);
    nbytes                 = len;
    char *ptr     = databuf;
    while (true_len-- > 0)
    {
        *ptr++ = *data++;
    }
}

void twi_slave_transmitter_mode(void)
{
    mode = TRANSMITTER;
}

void twi_slave_receiver_mode(void)
{
    mode = RECEIVER;
}

void twi_slave_init(unsigned char addr, unsigned char gen_call_response)
{
    twi_set_self_slave_address(addr, gen_call_response);
    twi_clear_flag();
}

ISR(TWI_vect)
{
    switch (TWSR)
    {

        case ARBTR_LOST_SLA_W_RECVD_ACK:
        case ARBTR_LOST_GEN_ADDR_RECVD_ACK:
        case GEN_ADDR_RECVD_ACK:
        case SLA_W_RECVD_ACK: /* master wants to send data */
            mode    = RECEIVER;
            buffptr = databuf;
            nbytes  = 0;
            twi_clear_flag(); /* ACK the data byte that will be received */
            break;

        case GEN_ADDR_DATA_RECVD_ACK:
        case SLA_W_DATA_RECVD_ACK: /* master has sent data */
            if (buffptr < &databuf[MAXBUFFLEN - 1])
            {
                twi_read_byte_ACK(buffptr++);
            }
            else if (buffptr == &databuf[MAXBUFFLEN - 1])
            {
                twi_read_byte_NACK(buffptr);
            }
            nbytes++;
            break;

        case GEN_ADDR_DATA_RECVD_NACK:
        case SLA_W_DATA_RECVD_NACK: /* sent back NACK on data received */
            twi_clear_flag();       /* wait to be addressed again */
            break;

        case ARBTR_LOST_SLA_R_RECVD_ACK:
        case SLA_R_RECVD_ACK: /* master wants to receive data */
            mode    = TRANSMITTER;
            buffptr = databuf;
            if (nbytes != 0)
            {
                twi_write_byte(*buffptr++);
            }
            break;

        case SLAV_DATA_SENT_ACK:
            if (buffptr <= &databuf[MAXBUFFLEN - 1])
            {
                twi_write_byte(*buffptr++);
            }
            else
            {
                buffptr = databuf; /* send same data again */
                twi_write_byte(*buffptr++);
            }
            break;

        case STOP_OR_REP_START_RECVD:
            if (mode == RECEIVER && call_back != NULL)
            {
                call_back(databuf, nbytes);
            }
            twi_clear_flag();
            break;

        case BUS_ERROR:
        case SLAV_LAST_BYTE_SENT_ACK:
        case SLAV_DATA_SENT_NACK:
            twi_clear_flag(); /* wait to be addressed again */
            break;
    }
}
