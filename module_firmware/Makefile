include hardware_type

ifeq ($(HARDWARE),atmega328)
	BOARD_TAG = uno
endif

ifeq ($(HARDWARE),attiny88)
	BOARD_TAG = attinyx8
	BOARD_SUB = 88
	AVRDUDE_CONF = /usr/share/arduino/conf/avrdude.conf
	BOARDS_TXT = ATTinyCore/avr/boards.txt

	ALTERNATE_CORE = ATTinyCore/avr
	ALTERNATE_CORE_PATH = ATTinyCore/avr
	F_CPU=1000000L
endif

include /usr/share/arduino/Arduino.mk

burn_hardware:
ifeq ($(HARDWARE),atmega328)
	cp build-uno/*.hex arduino.hex
	sudo avrdude -v -Cavrdude.conf -pm328p -clinuxgpio -e -Uflash:w:arduino.hex
	rm -rf arduino.hex
endif

ifeq ($(HARDWARE),attiny88)
	cp build-attinyx8-88/*.hex attiny88.hex
	sudo avrdude -v -Cavrdude.conf -pattiny88 -clinuxgpio -e -Uflash:w:attiny88.hex
	rm -rf attiny88.hex
endif
