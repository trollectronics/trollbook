# UART settings for programmer
TTYDEV		?=/dev/ttyUSB0
TTYBAUD		?=115200

# Name of the output file
NAME		= trololo

# Linkscript
LINKSCRIPT	= link.ld

# Tools
TARGET		= m68k-elf-
CC		= $(TARGET)gcc
AS		= $(TARGET)as
LD		= $(TARGET)ld
AR		= $(TARGET)ar
SREC_CAT	= srec_cat

# Paths
MODULESDIR      = $(TOPDIR)/libs
MODULES		= $(patsubst %/,%,$(foreach dir,$(wildcard $(MODULESDIR)/*/Makefile),$(dir $(dir))))
MODULESINCLUDE	= $(addsuffix /include,$(MODULES))

INCLUDEDIRS	= $(TOPDIR)/include $(MODULESINCLUDE)

# Compiler and linker flags
INCLUDES	= $(addprefix -I,$(INCLUDEDIRS))

CFLAGS		= -Wall -Os -std=c99 -ffreestanding -fno-builtin -nostdlib -static -m68040 $(INCLUDES)
ASFLAGS		= -msoft-float
LDFLAGS		= -T $(LINKSCRIPT) -lgcc

# Filenames
ELFFILE		= $(NAME).elf
ELFDEBUGFILE		= $(NAME)-debug.elf
HEXFILE		= $(NAME).hex
BINFILE		= $(NAME).bin

# Makefile configurations
MAKEFLAGS	+=	--no-print-directory
