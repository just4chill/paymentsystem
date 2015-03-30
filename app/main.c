
#include "LPC17xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include "system_LPC17xx.h"
#include "uart.h"
#include "lcd.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "keypad.h"

void prvSetupHardware( void );


static void scanCard(void * pvParameters)
{
	uint32_t count = 0;
	uint8_t tag_no[20];
	for(;;)
	{
		if(uart3_rx_head == 12)
		{
			memcpy(tag_no, uart3_rx_fifo, 12);
			uart3_rx_head = 0;
			lcd_write_instruction_4d(0xC0);
			for(uint8_t i = 11;i > 0;i--)
			{
				lcd_write_character_4d(tag_no[i]);
			}
			vTaskDelay(1000);
			lcd_write_instruction_4d(0xC0);
			lcd_print("                ");
		}
	}
}

static void scanKey(void * pvParameters)
{
	uint8_t key_val = 0;
	uint8_t key_buff[20];
	uint8_t key_count = 0;
	for(;;)
	{
		key_val = read_keypad();
		if(key_val != 0)
		{
			lcd_write_instruction_4d(0x80);
			lcd_write_character_4d(key_val);
		}
	}
}

int main(void)
{
	/* Setup the Hardware */
	prvSetupHardware();

	/* Create the Tasks */
	xTaskCreate(scanCard,
			(signed portCHAR *)"scanCard",
			128,
			NULL,
			3,
			NULL);

	xTaskCreate(scanKey,
			(signed portCHAR *)"scanKey",
			128,
			NULL,
			4,
			NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Never reach here */
	return 0;
}

void prvSetupHardware( void )
{
	/* Initialte Clock */
	SystemInit();
	SystemCoreClockUpdate();
	uart_init(0, 115200);			// Debug port
	uart_init(1, 9600);				// Modem
	uart_init(3, 9600);				// RFID Card reader
	uart_print(0, "System started\r\n");
	lcd_init();
	lcd_print("Booting..");
	_delay_ms(100);
	lcdClear();
	lcd_write_instruction_4d(0x80);
	lcd_print(" Payment System  ");
}

