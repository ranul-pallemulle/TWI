#include <avr/io.h>
#include <avr/interrupt.h>
#include "atmega328p_codes.h"
#include "twi.h"

#define PRESC_MASK 0xF8 /* mask prescaler bits in TWSR */

void master_init(unsigned char bitrate, char prescaler) {
  TWBR = bitrate;
  switch (prescaler) {
  case 1:
    TWSR &= ~((1 << TWPS1)|(1 << TWPS0));
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

int master_send_start(void) {
  twi_send_start();
  while (!(TWCR & (1 << TWINT)))
    ;
  if ((TWSR & PRESC_MASK) != START_SENT)
    return -1;
  return 0;
}

int master_send_stop(void) {
  twi_send_stop();
  while (!(TWCR & (1 << TWINT)))
    ;
  return 0;
}

int master_select_slave(unsigned char addr, char readwrite) {
  /*  if (readwrite != 1 || readwrite != 0)
      return -1;*/
  twi_write_byte((addr << 1) | readwrite);
  while (!(TWCR & (1 << TWINT)))
    ;
  if ((TWSR & PRESC_MASK) == SLA_W_SENT_ACK ||
      (TWSR & PRESC_MASK) == SLA_R_SENT_ACK)
    return 0;
  else
    return 1;
}

static int master_write_byte(unsigned char byte) {
  twi_write_byte(byte);
  while (!(TWCR & (1 << TWINT)))
    ;
  char status = TWSR & PRESC_MASK;
  if (status == MAST_DATA_SENT_ACK)
    return 1;
  else if (status == MAST_DATA_SENT_NACK)
    return 0;
  else
    return -1;
}

int master_write_bytes(unsigned char *data, char nbytes) {
  int status;
  while (nbytes > 0) {
    if ((status = master_write_byte(*data++)) != 1)
      break;
    nbytes--;
  }
  return status;
}

int master_read_bytes(unsigned char *data, char nbytes) {
  while (nbytes > 1) {
    twi_read_byte_ACK(data++);
    while (!(TWCR & (1 << TWINT)))
      ;
    if ((TWSR & PRESC_MASK) == MAST_DATA_RECVD_ACK)
      nbytes--;
    else
      return -1;
  }
  if (nbytes == 1) {
    twi_read_byte_NACK(data);
    while (!(TWCR & (1 << TWINT)))
      ;
    if ((TWSR & PRESC_MASK) == MAST_DATA_RECVD_NACK)
      nbytes--;
    else
      return -1;
  }
  return 0;
}

