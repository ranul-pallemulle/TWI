#include <avr/io.h>
#include <avr/interrupt.h>
#include "twi.h"
#include "status_codes.h"
#include "../include/twi_master.h"

struct slave *current;

int master_init(unsigned char bit_rate, unsigned char prescaler) {
  if (twi_set_scl(bit_rate, prescaler) == -1)
    return -1;
  return 0;
}

int master_start(struct slave *head) {
  current = head;
  if (twi_send_start() == -1)
    return -1;
  return 0;
}

int master_write_next_byte(struct slave *slav) {
  if (slav->dataptr == NULL)
    return -1;
  if (slav->nbytes > 0) {
    twi_write_byte(*slav->dataptr++);
    slav->nbytes--;
  }
  return slav->nbytes; /* number of bytes yet to be sent */
}

int master_read_next_byte(struct slave *slav) {
  if (slav->dataptr == NULL)
    return -1;
  if (slav->nbytes < slav->maxbytes) {
    twi_read_byte_ACK(slav->dataptr++);
    slav->nbytes++;
    return slav->nbytes;
  }
  else {
    twi_read_byte_NACK(slav->dataptr);
    return 0;
  }
}

void master_stop(void) {
  twi_send_stop(); /* change to handle error return later */
}

ISR(TWI_vect) {
  switch(TWSR) {
  int nb;
    
  case REP_START_SENT:
  case START_SENT:
    if (current->readwrite == WRITE)
      twi_write_byte((current->addr << 1) | WRITE);
    else if (current->readwrite == READ)
      twi_write_byte((current->addr << 1) | READ);
    break;
    
  case SLA_W_SENT_ACK:
    if ((nb = master_write_next_byte(current)) == 0) {
	if (current->callback != NULL)
	  current->callback();
    }
    if (nb == 0 || nb == -1) {
      if (current->next != NULL)
	master_start(current->next);
      else
	master_stop();
    }
    break;
    
  case MAST_DATA_SENT_ACK:
    if ((nb = master_write_next_byte(current)) == 0) { /* slave ACKed last byte */
	if (current->callback != NULL)
	  current->callback();
    }
    if (nb == 0 || nb == -1) {
      if (current->next != NULL)
	master_start(current->next);
      else
	master_stop();
    }
    break;

  case MAST_DATA_SENT_NACK:
    if (current->next != NULL)
      master_start(current->next);
    else
      master_stop();
    break;

  case SLA_R_SENT_ACK:
    if ((nb = master_read_next_byte(current)) == 0)



  }
}
