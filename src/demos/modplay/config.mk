# UART settings for programmer
TTYDEV		?=/dev/ttyUSB0
TTYBAUD		?=115200

# Name of the output file
NAME		= modplay

# Linkscript
LINKSCRIPT	= link.ld

# Tools
export TARGET		= m68k-elf-
export CC		= $(TARGET)gcc
export AS		= $(TARGET)as
export LD		= $(TARGET)ld
export AR		= $(TARGET)ar
SREC_CAT	= srec_cat

# Paths
MODULESDIR      = $(shell realpath $(TOPDIR)/libs)
MODULES		= $(patsubst %/,%,$(foreach dir,$(wildcard $(MODULESDIR)/*/Makefile),$(dir $(dir))))
MODULESINCLUDE	= $(addsuffix /include,$(MODULES))

INCLUDEDIRS	= $(shell realpath $(TOPDIR)/include) $(MODULESINCLUDE)

# Compiler and linker flags
INCLUDES	= $(addprefix -I,$(INCLUDEDIRS))

export CFLAGS		:= -Wall -Os -std=c99 -ffreestanding -fno-builtin -nostdlib -static -m68040 $(INCLUDES)
export ASFLAGS		:= -msoft-float
export LDFLAGS		:= -T $(LINKSCRIPT) -lgcc

# Filenames
ELFFILE		= $(NAME).elf
ELFDEBUGFILE		= $(NAME)-debug.elf
HEXFILE		= $(NAME).hex
BINFILE		= $(NAME).bin

# Makefile configurations
MAKEFLAGS	+=	--no-print-directory
