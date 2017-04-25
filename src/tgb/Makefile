# Device number
DEVICE		= attiny48

# Settings for programmer
AVRISP		= buspirate -P /dev/ttyUSB0

# Name of the project
PROGNAME	= outfile

# AVR Fuses
FUSES		= -U lfuse:w:0xee:m -U hfuse:w:0xdd:m

# Compiler and linker flags
CFLAGS		= -Wall -Os -mmcu=$(DEVICE) -std=c99 -ffreestanding -fshort-enums -fpack-struct
ASFLAGS		= 
LDFLAGS		= 

# Filenames
ELFFILE		= $(PROGNAME).elf
HEXFILE		= $(PROGNAME).hex

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

clean:
	@echo "[ RM ] $(HEXFILE) $(ELFFILE) $(OBJFILES)"
	@$(RM) $(HEXFILE) $(ELFFILE) $(OBJFILES)
	@$(RM) -R $(DEPDIR)

install:
	avrdude -p $(shell echo "$(DEVICE)" | sed -E 's/^at(t|m)(iny|ega)([0-9]*)/\1\3/') -c $(AVRISP) -U "flash:w:$(HEXFILE)"

fuses:
	avrdude -p $(shell echo "$(DEVICE)" | sed -E 's/^at(t|m)(iny|ega)([0-9]*)/\1\3/') -c $(AVRISP) $(FUSES)

$(ELFFILE): $(OBJFILES)
	@echo "[ LD ] $(ELFFILE)"
	@avr-gcc $(CFLAGS) -o $@ $(OBJFILES) $(LDFLAGS)

$(HEXFILE): $(ELFFILE)
	@echo "[OCPY] $(HEXFILE)"
	@avr-objcopy -j .text -j .data -O ihex $< $@

$(DEPDIR):
	@mkdir -p $@

# Compile C files
%.c.o: %.c | $(DEPDIR)
	@echo "[ CC ] $@"
	@avr-gcc $(CFLAGS) -c -MD -o $@ $<
	@cp $*.c.d $(df).c.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.c.d >> $(df).c.P; $(RM) $*.c.d

# Compile ASM files with C pre-processor directives
%.S.o: %.S | $(DEPDIR)
	@echo "[ AS ] $@"
	@avr-gcc $(CFLAGS) $(ASFLAGS) -c -MD -o $@ $<
	@cp $*.S.d $(df).S.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.S.d >> $(df).S.P; $(RM) $*.S.d

# Link symbol lists to object files
%.syms.o: %.syms
	@echo "[ LD ] $@"
	@avr-ld -o $@ -r --just-symbols=$<

# Check dependencies
-include $(CFILES:%.c=$(DEPDIR)/%.c.P)
-include $(ASMFILES:%.S=$(DEPDIR)/%.S.P)
