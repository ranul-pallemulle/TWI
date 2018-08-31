#define GENCALLRESPOND 1 /* set to 1 to respond to gencall */
#define TRANSMITTER 1 /* slave transmitter mode */
#define RECEIVER 0 /* slave receiver mode */

#ifndef MAXBUFFLEN
#define MAXBUFFLEN 30 /* max number of bytes to receive */
#endif /* MAXBUFFLEN */

#define NULL 0

void set_callback(void(*func)(unsigned char *, unsigned char));

int set_data(unsigned char *data, unsigned char len);

void set_mode(char m);

int slave_init(unsigned char addr, unsigned char genresponse);
