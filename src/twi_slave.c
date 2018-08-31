/* 
 * Interrupt driven TWI slave receiver/transmitter. User should set a 
 * callback function if intending to use receiver mode. Likewise, 
 * the set_data function should be used to specify data to be sent
 * if the transmitter mode is used. Call slave_init and enable global
 * interrupts to start slave operation. Alternative hardware can be used
 * by modifying the codes include file or by including a different one.
 *
 * Written by Ranul Pallemulle
 * 07/08/2018
 */

/* NOTES :
 * Change codes include so that file is specified by command line macro definition 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "twi.h" /* for basic twi functions */
#include "atmega328p_codes.h" /* status codes, change for different chip */
#include "../include/twi_slave.h"

static void (*call_back)(unsigned char* data, unsigned char len);
static unsigned char databuf[MAXBUFFLEN];
static unsigned char *buffptr;
static int nbytes = 0; /* number of bytes of data in databuf */
static char mode = RECEIVER;

/* set_callback: sets the user function to be called on all data received */
void set_callback(void (*func)(unsigned char*, unsigned char)) {
  call_back = func;
}

/* set_data: sets databuf to contain data to be sent on next transmission */
/* NOTE: THIS FUNCTION ENABLES EXTERNAL INTERRUPTS */
int set_data(unsigned char *data, unsigned char len) {
  unsigned char *ptr;
  twi_disable();
  if (len > MAXBUFFLEN)
    return -1;
  nbytes = len;
  ptr = databuf;
  while (len-- > 0)
    *ptr++ = *data++;
  return 0;
}

void set_mode(char m) {
  mode = m;
}

/* slave_init: put device in slave receive mode. Argument is 7 bit address */
int slave_init(unsigned char addr, unsigned char genresponse) {
  if (twi_set_self_slave_address(addr, genresponse) == -1)
    return -1;
  twi_clear_flag();
  return 0;
}

/* Interrupt Service Routine */
ISR(TWI_vect) {
  switch(TWSR) {
  
  case ARBTR_LOST_SLA_W_RECVD_ACK:
  case ARBTR_LOST_GEN_ADDR_RECVD_ACK:
  case GEN_ADDR_RECVD_ACK:
  case SLA_W_RECVD_ACK: /* master wants to send data */
    mode = RECEIVER;
    buffptr = databuf;
    nbytes = 0;
    twi_clear_flag(); /* ACK the data byte that will be received */
    break;

  case GEN_ADDR_DATA_RECVD_ACK:
  case SLA_W_DATA_RECVD_ACK: /* master has sent data */
    if (buffptr < &databuf[MAXBUFFLEN-1])
      twi_read_byte_ACK(buffptr++);
    else if (buffptr == &databuf[MAXBUFFLEN-1])
      twi_read_byte_NACK(buffptr);
    nbytes++;
    break;

  case GEN_ADDR_DATA_RECVD_NACK:
  case SLA_W_DATA_RECVD_NACK: /* sent back NACK on data received */
    twi_clear_flag(); /* wait to be addressed again */
    break;

  case ARBTR_LOST_SLA_R_RECVD_ACK:
  case SLA_R_RECVD_ACK: /* master wants to receive data */
    mode = TRANSMITTER;
    buffptr = databuf;
    if (nbytes != 0)
      twi_write_byte(*buffptr++);
    break;

  case SLAV_DATA_SENT_ACK:
    if (buffptr <= &databuf[MAXBUFFLEN-1])
      twi_write_byte(*buffptr++);
    else {
      buffptr = databuf; /* send same data again */
      twi_write_byte(*buffptr++);
    }
    break;

  case STOP_OR_REP_START_RECVD:
    if (mode == RECEIVER && call_back != NULL)
      call_back(databuf, nbytes);
    twi_clear_flag();
    break;
    
  case BUS_ERROR:
  case SLAV_LAST_BYTE_SENT_ACK:
  case SLAV_DATA_SENT_NACK:
    twi_clear_flag(); /* wait to be addressed again */
    break;
    
  }
}
    
    
