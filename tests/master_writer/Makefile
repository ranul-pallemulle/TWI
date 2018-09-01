TWI_LIB_DIR=../../out
TWI_INC_DIR=../../include

./out/master_writer.hex : ./out/master_writer.elf
	avr-objcopy -j .text -j .data -O ihex ./out/master_writer.elf ./out/master_writer.hex

./out/master_writer.elf : master_writer.c
	avr-gcc -Os -Wall -mmcu=atmega328p master_writer.c $(TWI_LIB_DIR)/twi.o $(TWI_LIB_DIR)/twi_master_poll.o -o $@ -I $(TWI_INC_DIR)

.PHONY : upload_dragon upload_ice clean
upload_dragon :
	avrdude -p m328p -c dragon_isp -e -v -U flash:w:./out/master_writer.hex -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

upload_ice :
	avrdude -p m328p -c atmelice_isp -e -v -U flash:w:./out/master_writer.hex -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

clean:
	rm ./out/*
