SRC=./src
OUT=./out
MCU=atmega328p

all : $(OUT)/twi.o $(OUT)/twi_slave.o

$(OUT)/twi_slave.o : $(SRC)/twi_slave.c
	avr-gcc -c $(SRC)/twi_slave.c -mmcu=$(MCU) -o $@

$(OUT)/twi.o : $(SRC)/twi.c
	avr-gcc -c $(SRC)/twi.c -mmcu=$(MCU) -o $@

.PHONY : clean
clean :
	rm $(OUT)/twi.o $(OUT)/twi_slave.o
