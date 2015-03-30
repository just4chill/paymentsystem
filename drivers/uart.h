
#ifndef __uart_h__
#define __uart_h__

#include <stdint.h>

#define IER_RBR	0x01
#define IER_THRE	0x02
#define IER_RLS	0x04
#define IIR_PEND	0x01
#define IIR_RLS	0x03
#define IIR_RDA	0x02
#define IIR_CTI	0x06
#define IIR_THRE	0x01
#define LSR_RDR	0x01
#define LSR_OE	0x02
#define LSR_PE	0x04
#define LSR_FE	0x08
#define LSR_BI	0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80
#define BUFSIZE	0x40

#define UART_RX0_BUFFER_SIZE 64
#define UART_TX0_BUFFER_SIZE 0

#define UART_RX1_BUFFER_SIZE 64
#define UART_TX1_BUFFER_SIZE 0

#define UART_RX3_BUFFER_SIZE 64

#define UART_RX0_BUFFER_MASK ( UART_RX0_BUFFER_SIZE - 1)
#define UART_RX1_BUFFER_MASK ( UART_RX1_BUFFER_SIZE - 1)
#define UART_RX3_BUFFER_MASK ( UART_RX3_BUFFER_SIZE - 1)

extern volatile uint8_t uart0_rx_tail;
extern volatile uint8_t uart0_rx_head;
extern volatile uint8_t uart0_rx_fifo[UART_RX0_BUFFER_SIZE];

extern volatile uint8_t uart1_rx_tail;
extern volatile uint8_t uart1_rx_head;
extern volatile uint8_t uart1_rx_fifo[UART_RX1_BUFFER_SIZE];

extern volatile uint8_t uart3_rx_tail;
extern volatile uint8_t uart3_rx_head;
extern volatile uint8_t uart3_rx_fifo[UART_RX3_BUFFER_SIZE];

extern void uart_init(uint8_t, uint32_t);
extern void uart_print(uint8_t, char *);
extern void uart_puts(uint8_t, char *, uint8_t);

#endif