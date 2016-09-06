ADDRESS	= DDC00
NAME	= stage2
TTYDEV	= /dev/ttyUSB0
ASFLAGS	= -m68040

.PHONY: clean install

$(NAME).hex: $(NAME).bin
	hexwrite $@ input@$(ADDRESS)=$< entry_point=$(ADDRESS)

$(NAME).bin: $(NAME).elf
	m68k-elf-objcopy -O binary $< $@

$(NAME).elf: $(NAME).S
	m68k-elf-as $(ASFLAGS) -o $@ $<

clean:
	$(RM) $(NAME).elf $(NAME).bin $(NAME).hex

install:
	cat $(NAME).hex > $(TTYDEV)
