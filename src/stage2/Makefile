ADDRESS = e0000

gradient.hex: gradient.bin
	hexwrite $@ input@$(ADDRESS)=$< entry_point=$(ADDRESS)

gradient.bin: gradient.elf
	m68k-elf-objcopy -O binary $< $@

gradient.elf: gradient.S
	m68k-elf-as -m68040 -o $@ $<

clean:
	$(RM) gradient.elf gradient.bin gradient.hex

install:
	cat gradient.hex > /dev/ttyUSB0
