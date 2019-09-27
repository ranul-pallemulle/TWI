TWI_LIB_DIR=../../out
TWI_INC_DIR=../../include
STD=gnu99
MCU=atmega328p
OPT=-Os
CPUFREQ=16000000UL

./out/master_writer.hex : ./out/master_writer.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

./out/master_writer.elf : master_writer.c
	avr-gcc -std=$(STD) -g -Wall -pedantic $(OPT) -DF_CPU=$(CPUFREQ) -mmcu=$(MCU) -o $@ $^ -I $(TWI_INC_DIR) -ltwi -L $(TWI_LIB_DIR)

.PHONY : upload_dragon upload_ice clean
upload_dragon : ./out/master_writer.hex
	avrdude -p m328p -c dragon_isp -e -v -U flash:w:$^

upload_ice : ./out/master_writer.hex
	avrdude -p m328p -c atmelice_isp -e -v -U flash:w:$^

clean:
	rm ./out/*
