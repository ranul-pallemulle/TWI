/* twi_master: an interrupt driven TWI master receiver/transmitter. 
 * A slave structure exists for each slave device to be communicated 
 * with on the bus. Master processes a linked list of the slaves until
 * no more slaves are left. Then master issues a stop signal. A given
 * slave may appear more than once in the list (e.g. to do a
 * write and then, after a repeated start condition, a read). A single 
 * data buffer exists for all transfers to limit storage use.
 * Another option is to have a pointer to data as a member rather than 
 * a buffer itself.
 */

#define READ 1
#define WRITE 0

struct slave {
  unsigned char addr; /* slave address */
  char readwrite; /* read = 1, write = 0 */
  int nbytes; /* number of bytes to be written or received */
  char complete; /* transfer for this slave is done : 1, incomplete: 0 */
};

  /* can do callback outside ISR by polling the complete field (unless can
   * find nicer solution) */

/* master_start: goes through a linked list of slaves doing read or write operations for each.
 * returns 0 if no errors occurred, returns -1 otherwise */
int master_start(struct slave *);
