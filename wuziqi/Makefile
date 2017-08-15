
CROSS_COMPILE=arm-linux-
CC=$(CROSS_COMPILE)gcc
CC_FLAGS= -c -I ./include
LD=$(CROSS_COMPILE)ld
LD_FLAGS= -T start.lds
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJCOPY_FLAGS= -O binary
OBJDUMP=$(CROSS_COMPILE)objdump
OBJDUMP_FLAGS=-D

TAG = start
OBJS = start.o main.o buzzer.o led.o lib.o key.o
OBJS += adc.o gpio.o uart.o vsprintf.o
OBJS += mmu.o vector.o reset.o wdt.o gic.o 
OBJS += pwm.o rtc.o lcd.o bl.o tp.o i2c.o wuziqi.o
LIBC += libc.a

#all: mkimage $(OBJS)
all: $(OBJS)
	@#$(LD) $(LD_FLAGS) $^ -o $(TAG)
	$(LD) $(LD_FLAGS) $(OBJS) $(LIBC) -o $(TAG)
	@#$(LD) $(LD_FLAGS) $(OBJS) -o $(TAG)
	$(OBJCOPY) $(OBJCOPY_FLAGS) $(TAG) $(TAG).bin
	$(OBJDUMP) $(OBJDUMP_FLAGS) $(TAG) > $(TAG).dis

mkimage:
	gcc tools/mkbmp.c -o tools/mkbmp	
	tools/mkbmp logo.bmp

%.o:%.s
	$(CC) $(CC_FLAGS) $< -o $@
%.o:%.S
	$(CC) $(CC_FLAGS) $< -o $@
%.o:%.c
	$(CC) $(CC_FLAGS) $< -o $@

clean:
	rm -rf $(OBJS) start start.bin start.dis


