#ifndef ATMEGA328P_CODES_H
#define ATMEGA328P_CODES_H

/* TWSR codes for ATmega328p */

/* Common master codes */
#define START_SENT 0x08
#define REP_START_SENT 0x10
#define ARBTR_LOST 0x38

/* Master transmitter mode codes */
#define SLA_W_SENT_ACK 0x18
#define SLA_W_SENT_NACK 0x20
#define MAST_DATA_SENT_ACK 0x28
#define MAST_DATA_SENT_NACK 0x30

/* Master receiver mode codes */
#define SLA_R_SENT_ACK 0x40
#define SLA_R_SENT_NACK 0x48
#define MAST_DATA_RECVD_ACK 0x50
#define MAST_DATA_RECVD_NACK 0x58

/* Slave receiver mode codes */
#define SLA_W_RECVD_ACK 0x60
#define ARBTR_LOST_SLA_W_RECVD_ACK 0x68
#define GEN_ADDR_RECVD_ACK 0x70
#define ARBTR_LOST_GEN_ADDR_RECVD_ACK 0x78
#define SLA_W_DATA_RECVD_ACK 0x80
#define SLA_W_DATA_RECVD_NACK 0x88
#define GEN_ADDR_DATA_RECVD_ACK 0x90
#define GEN_ADDR_DATA_RECVD_NACK 0x98
#define STOP_OR_REP_START_RECVD 0xA0

/* Slave transmitter mode codes */
#define SLA_R_RECVD_ACK 0xA8
#define ARBTR_LOST_SLA_R_RECVD_ACK 0xB0
#define SLAV_DATA_SENT_ACK 0xB8
#define SLAV_DATA_SENT_NACK 0xC0
#define SLAV_LAST_BYTE_SENT_ACK 0xC8

/* Misc codes */
#define NO_STATE_INFO 0xF8
#define BUS_ERROR 0x00


#endif /* ATMEGA328P_CODES_H */
