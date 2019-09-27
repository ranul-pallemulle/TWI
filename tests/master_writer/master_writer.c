#include "twi_master_poll.h"
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#define SLAVE 0x10

const char *data = "What's going on?";

void test1_100khz(void)
{
    twi_master_init_100khz(); /* for 100kHz operation */
    twi_master_send_start();
    twi_master_select_slave(SLAVE, MASTER_WRITE);
    twi_master_write_bytes(data, strlen(data));
    twi_master_send_stop();
}

void test1_400khz(void)
{
    twi_master_init_400khz(); /* for 100kHz operation */
    twi_master_send_start();
    twi_master_select_slave(SLAVE, MASTER_WRITE);
    twi_master_write_bytes(data, strlen(data));
    twi_master_send_stop();
}

void test2_400khz(void)
{
    /* CURRENTLY THIS TEST DOESN'T PASS */
    twi_master_init_400khz();
    char istr[4];
    istr[0] = ' ';
    
    int i;
    for (i = 0; i < 20; i++)
    {
	twi_master_send_start();
	twi_master_select_slave(SLAVE, MASTER_WRITE);
	twi_master_write_bytes(data, strlen(data));
	itoa(i, &istr[1], 10);
	twi_master_write_bytes(istr, strlen(istr));
    }
    twi_master_send_stop();
}

void test2_100khz(void)
{
    twi_master_init_100khz();
    char istr[4];
    istr[0] = ' ';

    int i;
    for (i = 0; i < 20; ++i)
    {
	twi_master_send_start();
	twi_master_select_slave(SLAVE, MASTER_WRITE);
	twi_master_write_bytes(data, strlen(data));
	itoa(i, &istr[1], 10);
	twi_master_write_bytes(istr, strlen(istr));
    }
    twi_master_send_stop();
}

void test3_100khz(void)
{
    twi_master_init_100khz();
    char istr[4];
    istr[0] = ' ';

    int i;
    for (i = 0; i < 20; ++i)
    {
	twi_master_send_start();
	twi_master_select_slave(SLAVE, MASTER_WRITE);
	twi_master_write_bytes(data, strlen(data));
	itoa(i, &istr[1], 10);
	twi_master_write_bytes(istr, strlen(istr));
	twi_master_send_stop();
    }
}

void test4_100khz(void)
{
    twi_master_init_100khz();
    char recvd[32];
    int res = twi_master_read_from_device(SLAVE, recvd, 32);
    recvd[31] = '\0';
    if (!res) {
	twi_master_send_start();
	twi_master_select_slave(SLAVE, MASTER_WRITE);
	twi_master_write_bytes(recvd, strlen(recvd));
	twi_master_send_stop();
    }
    else {
	for (int i = 0; i < 20; ++i) {
	    PORTB |= (1 << PB5);
	    _delay_ms(100);
	    PORTB &= ~(1 << PB5);
	    _delay_ms(100);
	}
    }
}

int main(void)
{
    DDRB |= (1 << DDB5);
    test4_100khz();
    for (;;)
    {
	PORTB |= (1 << PB5);
	_delay_ms(500);
	PORTB &= ~(1 << PB5);
	_delay_ms(500);
    }
    return 0;
}
