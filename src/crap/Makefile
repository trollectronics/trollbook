gradient.hex: gradient.bin
	hexwrite $@ input@ddb00=$< entry_point=ddb00

gradient.bin: gradient.elf
	m68k-elf-objcopy -O binary $< $@

gradient.elf: gradient.S
	m68k-elf-as -m68060 -o $@ $<

clean:
	$(RM) gradient.elf gradient.bin gradient.hex
