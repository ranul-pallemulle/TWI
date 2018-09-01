#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/twi_slave.h"
#define slave_addr 0x10

#define F_CPU 8000000UL
#include <util/delay.h>

int init(void) {
  set_mode(RECEIVER);
  if (slave_init(slave_addr, GENCALLRESPOND) == -1)
    return -1;
  return 0;
}

/* echo_slave: master first writes to the slave and then reads from it. 
 * Slave responds to read from the same buffer that was written to by the 
 * master and therefore echos back what was written to it */
int main(void) {
  if (init() == -1)
    return -1;
  sei();

  DDRB |= (1 << DDB5);
  for (;;) {
    PORTB |= (1 << PB5);
    _delay_ms(500);
    PORTB &= ~(1 << PB5);
    _delay_ms(500);
  }
  return 0;
}
