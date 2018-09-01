#define READ 1
#define WRITE 0

#define NULL 0

struct slave {
  struct slave *next;
  unsigned char addr; /* slave address */
  unsigned char *dataptr;
  unsigned char maxbytes; /* length of data array */
  char readwrite; /* read = 1, write = 0 */
  int nbytes; /* number of bytes to be written or received */
  char complete; /* transfer for this slave is done : 1, incomplete: 0 */
};

  /* can do callback outside ISR by polling the complete field (unless can
   * find nicer solution) */

/* master_start: goes through a linked list of slaves doing read or write operations for each.
 * returns 0 if no errors occurred, returns -1 otherwise */
int master_start(struct slave *);
