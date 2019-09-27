TWI_LIB_DIR=../../out
TWI_INC_DIR=../../include
STD=gnu99
MCU=atmega328p
OPT=-Os
CPUFREQ=16000000UL

./out/echo_slave.hex : ./out/echo_slave.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

./out/echo_slave.elf : echo_slave.c 
	avr-gcc -std=$(STD) -Wall -pedantic $(OPT) -DF_CPU=$(CPUFREQ) -mmcu=$(MCU) -o $@ $^ -I $(TWI_INC_DIR) -ltwi -L $(TWI_LIB_DIR)

.PHONY : upload_dragon upload_ice clean
upload_dragon : ./out/echo_slave.hex
	avrdude -p m328p -c dragon_isp -e -v -U flash:w:$^

upload_ice : ./out/echo_slave.hex
	avrdude -p m328p -c atmelice_isp -e -v -U flash:w:$^

clean:
	rm ./out/*
