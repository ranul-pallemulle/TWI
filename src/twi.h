#ifndef TWI_H
#define TWI_H
/* Basic TWI functions */

int twi_set_scl(unsigned char bit_rate, unsigned char prescaler);

int twi_set_self_slave_address(unsigned char addr, unsigned char gen_call_enable);

int twi_send_start(void);

int twi_send_stop(void);

int twi_write_byte(unsigned char byte);

int twi_clear_flag(void);

int twi_disable(void);

int twi_read_byte_ACK(unsigned char *byteptr);

int twi_read_byte_NACK(unsigned char *byteptr);

#endif /* TWI_H */
