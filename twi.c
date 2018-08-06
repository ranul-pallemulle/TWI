#include <avr/io.h>
/* Basic TWI functions */

/* twi_set_scl: set the SCL frequency. Arguments are TWI bit rate and prescaler */
void twi_set_scl(unsigned char bit_rate, unsigned char prescaler) {
  TWBR = bit_rate;
  /* look up how to use prescaler parameter */
}

/* twi_set_self_slave_address: set address and enable/disable (1/0) respond to gen call address */
void twi_set_self_slave_address(unsigned char addr, unsigned char gen_call_enable) {
  TWAR = (addr << 1) | gen_call_enable;
}

/* twi_send_start: puts a start condition on the bus */
int twi_send_start(void) {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE) ;
  return 0;
}

/* twi_send_stop: puts a stop condition on the bus */
int twi_send_stop(void) {
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (1 << TWIE) ;
  return 0;
}

/* twi_write_byte: put a byte of data or slave address on the bus */
int twi_write_byte(unsigned char byte) {
  TWDR = byte;
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) ;
  return 0;
}

/* twi_read_byte_ACK: read a data byte into a char and send back an ACK */
int twi_read_byte_ACK(unsigned char *byteptr) {
  *byteptr = TWDR;
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
  return 0;
}

/* twi_read_byte_NACK: read a data byte into a char and send back a NACK */
int twi_read_byte_NACK(unsigned char *byteptr) {
  *byteptr = TWDR;
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
  return 0;
}