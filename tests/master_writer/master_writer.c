#include <avr/io.h>
#include <avr/interrupt.h>
#include "twi_master_poll.h"

#define F_CPU 8000000UL
#include <util/delay.h>
#define addr 0x10

char data[] = "What's going on?\n";

int main(void) {
  DDRB |= (1 << DDB5);
  
  master_init(32,1); /* for 100kHz operation */
  if (master_send_start() != 0) /* example of a check that can be done on the other functions here too */
    return -1;
  master_select_slave(addr,WRITE);
    
  for (;;) {
    PORTB |= (1 << PB5);
    _delay_ms(500);
    PORTB &= ~(1 << PB5);
    _delay_ms(500);
    
    master_write_bytes((unsigned char *)data,sizeof data);
    master_send_start();
    master_select_slave(addr,WRITE);
  }

  return 0;
}
