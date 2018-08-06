#include <avr/io.h>
#include <avr/interrupt.h>
#include "twi.h" /* for basic twi functions */
#include "atmega328p_codes.h" /* status codes */

#define TRUE 1
#define FALSE 0

#ifndef MAXBUFFLEN
#define MAXBUFFLEN 30 /* max number of bytes to receive */
#endif /* MAXBUFFLEN */

static void (*call_back)(unsigned char* data, unsigned char len);
static unsigned char databuf[MAXLEN];
static unsigned char *buffptr;

/* set_callback: sets the user function to be called on all data received */
void set_callback(void (*func)(unsigned char*, unsigned char)) {
  call_back = func;
}

/* slave_init: put device in slave receive mode. Argument is 7 bit address */
void slave_init(unsigned char addr) {
  twi_set_self_slave_address(addr, FALSE); /* disabled respond to gen call addr */
  twi_clear_flag();
}

/* Interrupt Service Routine */
ISR(TWI_vect) {
  switch(TWSR) {

  case ARBTR_LOST_SLA_W_RECVD_ACK:
  case ARBTR_LOST_GEN_ADDR_RECVD_ACK:
  case GEN_ADDR_RECVD_ACK:
  case SLA_W_RECVD_ACK: /* master wants to send data */
    buffptr = databuf;
    twi_clear_flag();
    break;

  case GEN_ADDR_DATA_RECVD_ACK:
  case SLA_W_DATA_RECVD_ACK: /* master has sent data */
    if(buffptr < &databuf[MAXLEN-1])
      twi_read_byte_ACK(buffptr++);
    else if (buffptr == &databuf[MAXLEN-1])
      twi_read_byte_NACK(buffptr);
    break;

  case GEN_ADDR_DATA_RECVD_NACK:
  case SLA_W_DATA_RECVD_NACK: /* sent back NACK on data received */
    twi_clear_flag(); /* wait to be addressed again */
    break;

  case ARBTR_LOST_SLA_R_RECVD_ACK:
  case SLA_R_RECVD_ACK: /* master wants to receive data */
    buffptr = databuf;
    twi_write_byte(*buffptr++); /* if databuf empty, might be a garbage value */
    break;

  case SLAV_DATA_SENT_ACK:
    
    
    
