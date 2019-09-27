#include "twi_slave.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define slave_addr 0x10
#define GEN_CALL_RESPONSE 1
#include <util/delay.h>

/* echo_slave: master first writes to the slave and then reads from
 * it. Slave responds to the read request using the same buffer that
 * was written to by the master and therefore echos back what was
 * written to it. Simultaneously, the slave blinks an LED to
 * demonstrate that the TWI functionality is interrupt driven. */
int main(void)
{
    twi_slave_receiver_mode();
    twi_slave_init(slave_addr, GEN_CALL_RESPONSE);
    sei();

    DDRB |= (1 << DDB5);
    for (;;)
    {
        PORTB |= (1 << PB5);
        _delay_ms(500);
        PORTB &= ~(1 << PB5);
        _delay_ms(500);
    }
    return 0;
}
