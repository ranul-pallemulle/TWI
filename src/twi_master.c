#include <avr/io.h>
#include <avr/interrupt.h>
#include "twi.h"
#include "atmega328p_codes.h"
#include "../include/twi_master.h"

#define NULL 0

struct slave *head;
struct slave *current_slave;

ISR(TWI_vect) {
  switch(TWSR) {
    
  case START_SENT:
  case REP_START_SENT:
    if (current_slave != NULL) {
      if (current_slave->readwrite == READ)
	twi_write_byte((current_slave->addr << 1) | READ);
      else
	twi_write_byte((current_slave->addr << 1) | WRITE);
    }
    break;

  case SLA_W_SENT_ACK:
    
    break;


  }
}
