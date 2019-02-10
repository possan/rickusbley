#
#             LUFA Library
#     Copyright (C) Dean Camera, 2019.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = at90usb1287
ARCH         = AVR8
BOARD        = MICRO
F_CPU        = 8000000
F_USB        = $(F_CPU)
OPTIMIZATION = fast
TARGET       = RickUSBley
SRC          = audioinput.c descriptors.c ../librick/src/roll.c $(LUFA_SRC_USB)
LUFA_PATH    = ../../AVR-HID/lufa/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -Ofast -I../librick/src
LD_FLAGS     =

MCU  	    	   = atmega32u4
F_CPU   	       = 16000000
AVRDUDE_PROGRAMMER = avr109
AVRDUDE_PORT       = /dev/cu.usbmodem14601

# Default target
all:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk
