# UART settings for programmer
TTYDEV		?=/dev/ttyUSB0
TTYBAUD		?=115200

# Name of the output file
NAME	= stage2

# Linkscript
LINKSCRIPT	:= link.ld

# Tools
TARGET	= m68k-elf-
CC	= $(TARGET)gcc
AS	= $(TARGET)as
LD	= $(TARGET)ld
AR	= $(TARGET)ar
SREC_CAT= srec_cat

# Compiler and linker flags
CFLAGS		= -Wall -Os -std=c99 -ffreestanding -fno-builtin -nostdlib -static -m68040 -I$(TOPDIR)/include
ASFLAGS		= -msoft-float
LDFLAGS		= -T $(LINKSCRIPT) -lgcc

# Filenames
ELFFILE		= $(NAME).elf
HEXFILE		= $(NAME).hex
BINFILE		= $(NAME).bin

# Makefile configurations
MAKEFLAGS	+=	--no-print-directory
