#include <avr/io.h>
#include <avr/interrupt.h>
#include "twi.h" /* for basic twi functions */

static void (*call_back)(unsigned char* data, unsigned char len);

/* set_callback: sets the user function to be called on all data received */
void set_callback(void (*func)(unsigned char*, unsigned char)) {
  call_back = func;
}

/*
void slave_init(unsigned char addr) {
  twi_set_self_slave_address(
