# simple AVR Makefile
#
# written by michael cousins (http://github.com/mcous)
# released to the public domain

# Makefile
#
# targets:
#   all:    compiles the source code
#   test:   tests the isp connection to the mcu
#   flash:  writes compiled hex file to the mcu's flash memory
#   fuse:   writes the fuse bytes to the MCU
#   disasm: disassembles the code for debugging
#   clean:  removes all .hex, .elf, and .o files in the source code and library directories
CONF = /etc/avrdude.conf
COM = /dev/ttyUSB0
UPL = sudo avrdude
PRO = arduino

UNAME = $(shell uname -s)

ifeq (UNAME,Darwin)
	CONF = /Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf
	COM = /dev/cu.usbserial-1420
	UPL = avrdude
endif

ifeq (UNAME,windows32)
	CONF = "/c/Arduino/avrdude.conf"
	COM = "\\.\COM1"
	UPL = "/c/Arduino/avrdude"
	PRO = stk500
endif

# parameters (change this stuff accordingly)
# project name
PRJ = main
# avr mcu
MCU = atmega328p
# mcu clock frequency
CLK = 1000000
# avr programmer (and port if necessary)
# e.g. PRG = usbtiny -or- PRG = arduino -P /dev/tty.usbmodem411
PRG = $(PRO) -C$(CONF) -v -P$(COM) -b57600
# fuse values for avr: low, high, and extended
# these values are from an Arduino Uno (ATMega328P)
# see http://www.engbedded.com/fusecalc/ for other MCUs and options
LFU = 0xFF
HFU = 0xDE
EFU = 0x05
# program source files (not including external libraries)
SRC = src/$(PRJ).c \
	src/app/climate_evaluator.c \
	src/app/communication_controller.c \
	src/app/indoor_climate_monitor.c \
	src/app/sensor_controller.c \
	src/app/visible_controller.c \
	src/com/i2c.c src/com/uart.c src/com/spi.c \
	src/sensor/am2320.c \
	src/sensor/ccs811.c \
	src/sensor/ldr.c \
	src/sensor/spl.c \
	src/driver/crc.c \
	src/driver/adc.c \
	src/driver/pwm.c \
	src/driver/init.c 
	
# where to look for external libraries (consisting of .c/.cpp files and .h files)
# e.g. EXT = ../../EyeToSee ../../YouSART
EXT =


#################################################################################################
# \/ stuff nobody needs to worry about until such time that worrying about it is appropriate \/ #
#################################################################################################

# include path
INCLUDE := $(foreach dir, $(EXT), -I$(dir))
# c flags
CWARN = -Wall -Wextra -pedantic
CFLAGS    = $(CWARN) -O2 -DF_CPU=$(CLK) -mmcu=$(MCU) $(INCLUDE)
# any aditional flags for c++
CPPFLAGS =

# executables
AVRDUDE = $(UPL) -c $(PRG) -p $(MCU)
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size --format=avr --mcu=$(MCU)
CC      = avr-gcc

# generate list of objects
CFILES    = $(filter %.c, $(SRC))
#EXTC     := $(foreach dir, $(EXT), $(wildcard $(dir)/*.c))
#CPPFILES  = $(filter %.cpp, $(SRC))
#EXTCPP   := $(foreach dir, $(EXT), $(wildcard $(dir)/*.cpp))
OBJ       = $(addprefix obj/,$(notdir $(CFILES:.c=.o)))
ASM       = $(addprefix asm/,$(notdir $(CFILES:.c=.S)))
#$(EXTC:.c=.o) $(CPPFILES:.cpp=.o) $(EXTCPP:.cpp=.o)

# user targets
# compile all files
all: bin/$(PRJ).hex

# test programmer connectivity
test:
	$(AVRDUDE) -v

# flash program to mcu
flash: all
	$(AVRDUDE) -U flash:w:bin/$(PRJ).hex:i

# write fuses to mcu
fuse:
	$(AVRDUDE) -U lfuse:w:$(LFU):m -U hfuse:w:$(HFU):m -U efuse:w:$(EFU):m

# generate disassembly files for debugging
disasm: $(PRJ).elf
	$(OBJDUMP) -d bin/$(PRJ).elf

# remove compiled files
clean:
	rm -f obj/* bin/* asm/*
	$(foreach dir, $(EXT), rm -f $(dir)/*.o;)

# format files
format:
	clang-format -i --style=LLVM src/app/*  src/com/*  src/driver/*  src/main.c  src/sensor/*

# other targets
# objects from c files
obj/$(PRJ).o: src/$(PRJ).c
	$(CC) $(CFLAGS) -c $< -o $@
obj/%.o: src/*/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# asm from c files
assembly: $(ASM)
asm/$(PRJ).S: src/$(PRJ).c
	$(CC) $(CFLAGS) -S $< -o $@
asm/%.S: src/*/%.c
	$(CC) $(CFLAGS) -S $< -o $@

# elf file
bin/$(PRJ).elf: $(OBJ)
	$(CC) $(CFLAGS) -Wl,-u,vfprintf -lprintf_flt -lm -o bin/$(PRJ).elf $(OBJ)

# hex file
bin/$(PRJ).hex: bin/$(PRJ).elf
	rm -f bin/$(PRJ).hex
	$(OBJCOPY) -j .text -j .data -O ihex bin/$(PRJ).elf bin/$(PRJ).hex
	$(SIZE) bin/$(PRJ).elf
