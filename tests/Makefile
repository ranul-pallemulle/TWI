TWI_LIB_DIR=../out
TWI_INC_DIR=../include

./out/echo_slave.hex : ./out/echo_slave.elf
	avr-objcopy -j .text -j .data -O ihex ./out/echo_slave.elf ./out/echo_slave.hex

./out/echo_slave.elf : echo_slave.c 
	avr-gcc -Os -Wall -mmcu=atmega328p echo_slave.c $(TWI_LIB_DIR)/twi.o $(TWI_LIB_DIR)/twi_slave.o -o $@ -I $(TWI_INC_DIR)

.PHONY : upload_dragon upload_ice
upload_dragon :
	avrdude -p m328p -c dragon_isp -e -v -U flash:w:./out/echo_slave.hex -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

upload_ice :
	avrdude -p m328p -c atmelice_isp -e -v -U flash:w:./out/echo_slave.hex -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
