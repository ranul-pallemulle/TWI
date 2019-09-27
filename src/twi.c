#include "twi.h"

extern inline void twi_set_scl(unsigned char bit_rate, unsigned char prescaler);
extern inline void twi_set_self_slave_address(unsigned char addr,
					      unsigned char gen_call_enable);
extern inline void twi_send_start(void);
extern inline void twi_send_stop(void);
extern inline void twi_write_byte(char byte);
extern inline void twi_clear_flag(void);
extern inline void twi_disable(void);
extern inline void twi_read_byte_ACK(char *byteptr);
extern inline void twi_read_byte_NACK(char *byteptr);
