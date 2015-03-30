
CC = arm-none-eabi-

INC = 	-Ikernel/include \
		-Ikernel/portable/GCC/ARM_CM3 \
		-Idevice \
		-Iconfig \
		-Idrivers

VPATH = kernel: \
		kernel/portable/GCC/ARM_CM3: \
		kernel/portable/MemMang: \
		app: \
		device: \
		drivers: \
		syscalls

LDSCRIPT = linker.ld

CFLAGS = -g -c -Wall -Os -mthumb -mcpu=cortex-m3 $(INC) -std=c99
LFLAGS = -g -Wall -Os -mthumb -mcpu=cortex-m3 -nostartfiles --specs=nano.specs -T$(LDSCRIPT)

OUTPUT = app.elf

OBJS = 	list.o \
		queue.o \
		tasks.o \
		timers.o \
		port.o \
		heap_1.o \
		lpc17xx.o \
		system_LPC17xx.o \
		main.o \
		uart.o \
		lcd.o \
		syscalls.o \
		delay.o 

all: $(OBJS)
	$(CC)gcc $(LFLAGS) $(OBJS) -o $(OUTPUT)
	$(CC)size $(OUTPUT)

clean: $(OBJS)
	-rm $(OBJS)

list.o: list.c
	$(CC)gcc $(CFLAGS) $^ -o $@

queue.o: queue.c
	$(CC)gcc $(CFLAGS) $^ -o $@

tasks.o: tasks.c
	$(CC)gcc $(CFLAGS) $^ -o $@

timers.o: timers.c
	$(CC)gcc $(CFLAGS) $^ -o $@

port.o: port.c
	$(CC)gcc $(CFLAGS) $^ -o $@

heap_1.o: heap_1.c
	$(CC)gcc $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC)gcc $(CFLAGS) $^ -o $@

uart.o: uart.c
	$(CC)gcc $(CFLAGS) $^ -o $@

lpc17xx.o: lpc17xx.S
	$(CC)gcc $(CFLAGS) $^ -o $@

system_LPC17xx.o: system_LPC17xx.c
	$(CC)gcc $(CFLAGS) $^ -o $@

uart.o: uart.c
	$(CC)gcc $(CFLAGS) $^ -o $@

syscalls.o: syscalls.c
	$(CC)gcc $(CFLAGS) $^ -o $@

delay.o: delay.c
	$(CC)gcc $(CFLAGS) $^ -o $@

lcd.o: lcd.c
	$(CC)gcc $(CFLAGS) $^ -o $@