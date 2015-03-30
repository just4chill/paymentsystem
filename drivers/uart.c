
#include "uart.h"
#include "LPC17xx.h"
#include "system_LPC17xx.h"
#include <stdint.h>

#define TEMT (1 << 6) 

//volatile struct uartFIFO * UART0_FIFO;

volatile uint8_t uart0_rx_tail;
volatile uint8_t uart0_rx_head;
volatile uint8_t uart0_rx_fifo[UART_RX0_BUFFER_SIZE];

volatile uint8_t uart1_rx_tail;
volatile uint8_t uart1_rx_head;
volatile uint8_t uart1_rx_fifo[UART_RX1_BUFFER_SIZE];

volatile uint8_t uart3_rx_tail;
volatile uint8_t uart3_rx_head;
volatile uint8_t uart3_rx_fifo[UART_RX3_BUFFER_SIZE];

void UART0_IRQHandler(void)
{
	uint8_t tmphead;
	tmphead = (uart0_rx_head + 1) & UART_RX0_BUFFER_MASK;
	if(tmphead == uart0_rx_tail)
	{
		// Buffer overflow
		uart0_rx_head = 0;
	}
	else
	{
		uart0_rx_head = tmphead;
		uart0_rx_fifo[tmphead] = LPC_UART0->RBR;
	}
}

void UART1_IRQHandler(void)
{
	uint8_t tmphead;
	tmphead = (uart1_rx_head + 1) & UART_RX1_BUFFER_MASK;
	if(tmphead == uart1_rx_tail)
	{
		// Buffer overflow
		uart1_rx_head = 0;
	}
	else
	{
		uart1_rx_head = tmphead;
		uart1_rx_fifo[tmphead] = LPC_UART1->RBR;
	}
}

void UART3_IRQHandler(void)
{
	uint8_t tmphead;
	tmphead = (uart3_rx_head + 1) & UART_RX3_BUFFER_MASK;
	if(tmphead == uart3_rx_tail)
	{
		// Buffer overflow
		uart3_rx_head = 0;
	}
	else
	{
		uart3_rx_head = tmphead;
		uart3_rx_fifo[tmphead] = LPC_UART3->RBR;
	}
}

#define PCLK_UART3_MASK (3 << 18)

void uart_init(uint8_t port, uint32_t baudrate)
{
	uint32_t Fdiv;
	uint32_t pclkdiv, pclk;

	if(port == 0)
	{
		uart0_rx_tail = 0;
		uart0_rx_head = 0;

		LPC_PINCON->PINSEL0 &= ~0x000000F0;
		LPC_PINCON->PINSEL0 |= 0x00000050; /* RxD0 is P0.3 and TxD0 is P0.2 */

		/* By default, the PCLKSELx value is zero, thus, the PCLK for
		all the peripherals is 1/4 of the SystemFrequency. */

		/* Bit 6~7 is for UART0 */
		pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
		switch ( pclkdiv )
		{
			case 0x00:

			default:
				pclk = SystemCoreClock / 4;
				break;
			case 0x01:
				pclk = SystemCoreClock;
				break;
			case 0x02:
				pclk = SystemCoreClock / 2;
				break;
			case 0x03:
				pclk = SystemCoreClock / 8;
				break;
		}

		LPC_UART0->LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
		Fdiv = ( pclk / 16 ) / baudrate ; /*baud rate */

		LPC_UART0->DLM = Fdiv / 256;
		LPC_UART0->DLL = Fdiv % 256;

		LPC_UART0->LCR = 0x03; /* DLAB = 0 */
		LPC_UART0->FCR = 0x07; /* Enable and reset TX and RX FIFO. */

		NVIC_EnableIRQ(UART0_IRQn);
		LPC_UART0->IER = IER_RBR; /* Enable UART0 interrupt */
	}	

	else if(port == 1)
	{
		uart1_rx_tail = 0;
		uart1_rx_head = 0;

		LPC_PINCON->PINSEL4 &= ~0x0000000F;
		LPC_PINCON->PINSEL4 |= 0x0000000A; /* Enable RxD1 P2.1, TxD1 P2.0 */

		/* By default, the PCLKSELx value is zero, thus, the PCLK for
		all the peripherals is 1/4 of the SystemFrequency. */

		/* Bit 8~9 is for UART1 */
		pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;
		switch ( pclkdiv )
		{
			case 0x00:

			default:
				pclk = SystemCoreClock / 4;
				break;
			case 0x01:
				pclk = SystemCoreClock;
				break;
			case 0x02:
				pclk = SystemCoreClock / 2;
				break;
			case 0x03:
				pclk = SystemCoreClock / 8;
				break;
		}

		LPC_UART1->LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
		Fdiv = ( pclk / 16 ) / baudrate ; /*baud rate */

		LPC_UART1->DLM = Fdiv / 256;
		LPC_UART1->DLL = Fdiv % 256;

		LPC_UART1->LCR = 0x03; /* DLAB = 0 */
		LPC_UART1->FCR = 0x07; /* Enable and reset TX and RX FIFO. */

		NVIC_EnableIRQ(UART1_IRQn);
		LPC_UART1->IER = IER_RBR; /* Enable UART0 interrupt */
	}

	else if(port == 3)
	{
	uart3_rx_tail = 0;
	uart3_rx_head = 0;

	pclk = SystemCoreClock / 4;

	// Turn on power to UART2
	LPC_SC->PCONP |=  (1 << 25);

	// Turn on UART2 peripheral clock
	LPC_SC->PCLKSEL1 &= ~(PCLK_UART3_MASK);
	LPC_SC->PCLKSEL1 |=  (0 << 18);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.0 = TXD3, P0.1 = RXD3
	LPC_PINCON->PINSEL0 &= ~0xf;
	LPC_PINCON->PINSEL0 |= ((1 << 1) | (1 << 3));

	LPC_UART3->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
    Fdiv = ( pclk / 16 ) / baudrate ;	// Set baud rate
    LPC_UART3->DLM = Fdiv / 256;
    LPC_UART3->DLL = Fdiv % 256;
    LPC_UART3->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
    LPC_UART3->FCR = 0x07;		// Enable and reset TX and RX FIFO
    NVIC_EnableIRQ(UART3_IRQn);
	LPC_UART3->IER = IER_RBR; /* Enable UART3 interrupt */			
	}
}

void uart_print(uint8_t port, char * buff)
{
	if(port == 0)
	{
		while(*buff != '\0')
		{
			LPC_UART0->THR = *buff++;
		}
		while(!((LPC_UART0->LSR) & TEMT));
	}
	else if(port == 1)
	{
		while(*buff != '\0')
		{
			LPC_UART1->THR = *buff++;
		}
		while(!((LPC_UART1->LSR) & TEMT));
	}
	else if(port == 3)
	{
		while(*buff != '\0')
		{
			LPC_UART3->THR = *buff++;
		}
		while(!((LPC_UART3->LSR) & TEMT));
	}
}

void uart_puts(uint8_t port, char * buff, uint8_t len)
{
	uint8_t i;
	if(port == 0)
	{
		for(i = 0; i < len;i++)
		{
			LPC_UART0->THR = buff[i];
		}
		while(!((LPC_UART0->LSR) & TEMT));
	}
	else if(port == 1)
	{
		for(i = 0; i < len;i++)
		{
			LPC_UART1->THR = buff[i];
		}
		while(!((LPC_UART1->LSR) & TEMT));
	}
	else if(port == 3)
	{
		for(i = 0; i < len;i++)
		{
			LPC_UART3->THR = buff[i];
		}
		while(!((LPC_UART3->LSR) & TEMT));
	}
}