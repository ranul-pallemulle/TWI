#define READ 1
#define WRITE 0

/* master_init: set the bit-rate register and clock prescaler */
void master_init(unsigned char bitrate, char prescaler);

/* master_send_start: puts start condition on the bus.
 * Returns -1 if an error occurred, else returns 0
 */
int master_send_start(void);

/* master_send_stop: puts stop condition on the bus. */
void master_send_stop(void);

/* master_select_slave: puts slave address and read/write on the bus.
 * Use the macros at the top of this file to set the second argument.
 * Returns -1 if the second argument is incorrect. Returns 1 if an error
 * occurred (including if slave didn't respond. Else, returns 0.
 */
int master_select_slave(unsigned char addr, char readwrite);

/* master_write_bytes: write nbytes bytes from data to the bus.
 * Returns 1 if the slave ACKed the last byte sent. Returns 0 if
 * the slave NACKed any bytes. Returns -1 if any error occurred
 * during any of the byte transfers.
 */
int master_write_bytes(unsigned char *data, char nbytes);

/* master_read_bytes: read nbytes bytes to data from the bus.
 * Returns -1 if an error occurred during any of the byte transfers.
 * Returns 0 else.
 */
int master_read_bytes(unsigned char *data, char nbytes);
