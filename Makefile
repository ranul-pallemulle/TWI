SRC=./src
OUT=./out
INCLUDE=./include
STD=gnu99
MCU=atmega328p
DEV=DEVICE_ATMEGA328P
OPT=-Os
CPUFREQ=16000000UL

all : $(OUT)/libtwi.a

$(OUT)/libtwi.a : $(OUT)/twi.o $(OUT)/twi_slave.o $(OUT)/twi_master_poll.o
	avr-ar rcs $@ $^
	rm $^

$(OUT)/twi_master_poll.o : $(SRC)/twi_master_poll.c
	avr-gcc -std=$(STD) -g -Wall -pedantic $(OPT) -DF_CPU=$(CPUFREQ) -c $^ -mmcu=$(MCU) -o $@ -I $(INCLUDE) -D$(DEV)

$(OUT)/twi_slave.o : $(SRC)/twi_slave.c
	avr-gcc -std=$(STD) -g -Wall -pedantic $(OPT) -DF_CPU=$(CPUFREQ) -c $^ -mmcu=$(MCU) -o $@ -I $(INCLUDE) -D$(DEV)

$(OUT)/twi.o : $(SRC)/twi.c
	avr-gcc -std=$(STD) -g -Wall -pedantic $(OPT) -DF_CPU=$(CPUFREQ) -c $^ -mmcu=$(MCU) -o $@

.PHONY : clean
clean :
	rm $(OUT)/twi.o $(OUT)/twi_slave.o $(OUT)/twi_master_poll.o $(OUT)/libtwi.a
