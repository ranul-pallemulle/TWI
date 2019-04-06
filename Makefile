SRC=./src
OUT=./out
# MCU=atmega328p
MCU=atmega644

all : $(OUT)/twi.o $(OUT)/twi_slave.o $(OUT)/twi_master_poll.o

$(OUT)/twi_master_poll.o : $(SRC)/twi_master_poll.c
	avr-gcc -c $(SRC)/twi_master_poll.c -mmcu=$(MCU) -o $@ -I ./include/

$(OUT)/twi_slave.o : $(SRC)/twi_slave.c
	avr-gcc -c $(SRC)/twi_slave.c -mmcu=$(MCU) -o $@ -I ./include/

$(OUT)/twi.o : $(SRC)/twi.c
	avr-gcc -c $(SRC)/twi.c -mmcu=$(MCU) -o $@

.PHONY : clean
clean :
	rm $(OUT)/twi.o $(OUT)/twi_slave.o
