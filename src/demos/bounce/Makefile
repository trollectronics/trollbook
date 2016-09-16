# UART settings for programmer
TTYDEV		?=/dev/ttyUSB0
TTYBAUD		?=115200

# Name of the output file
NAME	= bounce

# Linkscript
LINKSCRIPT	:= link.ld

# Tools
CC	= m68k-elf-gcc
AS	= m68k-elf-as
LD	= m68k-elf-ld
AR	= m68k-elf-ar

# Compiler and linker flags
CFLAGS		= -Wall -Os -std=c99 -ffreestanding -fno-builtin -nostdlib -static -m68040
ASFLAGS		= -msoft-float
LDFLAGS		= -T $(LINKSCRIPT) -lgcc

# Filenames
ELFFILE		= $(NAME).elf
HEXFILE		= $(NAME).hex

# Find all source files automatically
CFILES          = $(wildcard *.c)
ASFILES         = $(wildcard *.S)
SYMSFILES	= $(wildcard *.syms)

# Object file names
OBJFILES        = $(CFILES:.c=.c.o)
OBJFILES        +=$(ASFILES:.S=.S.o)
OBJFILES	+=$(SYMSFILES:.syms=.syms.o)

# Hidden directory for dependency files
DEPDIR = .deps
df = $(DEPDIR)/$(*F)

.PHONY: all clean install
.SUFFIXES:

all: $(HEXFILE)

install:
	stty -F $(TTYDEV) $(TTYBAUD)
	cat $(NAME).hex > $(TTYDEV)

clean:
	$(RM) $(HEXFILE) $(ELFFILE) $(OBJFILES)
	$(RM) -R $(DEPDIR)

$(ELFFILE): $(OBJFILES)
	$(CC) $(CFLAGS) -o $@ $(OBJFILES) $(LDFLAGS)

$(HEXFILE): $(ELFFILE)
	m68k-elf-objcopy -O ihex $< $@
	srec_cat -O $@ -Intel $@ -Intel

$(DEPDIR):
	@mkdir -p $@

# Compile C files
%.c.o: %.c | $(DEPDIR)
	$(CC) $(CFLAGS) -c -MD -o $@ $<
	@cp $*.c.d $(df).c.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.c.d >> $(df).c.P; $(RM) $*.c.d

# Compile ASM files with C pre-processor directives
%.S.o: %.S | $(DEPDIR)
	$(CC) $(CFLAGS) $(ASFLAGS) -c -MD -o $@ $<
	@cp $*.S.d $(df).S.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.S.d >> $(df).S.P; $(RM) $*.S.d

# Link symbol lists to object files
%.syms.o: %.syms
	$(LD) -o $@ -r --just-symbols=$<

# Check dependencies
-include $(CFILES:%.c=$(DEPDIR)/%.c.P)
-include $(ASFILES:%.S=$(DEPDIR)/%.S.P)
