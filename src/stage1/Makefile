MAKEFLAGS	+=	--no-print-directory



hexload.bin: hexload.elf
	m68k-elf-objcopy -O binary $< $@
	xxd -g 4 -c 4 -p < $@ | tr "\n" ' '
	@echo
	hexwrite ../../hdl/bootrom.hex record_width=4 word_size=4 input=$@
	
hexload.elf: hexload.s
	m68k-elf-as -m68040 -o $@ $<

clean:
	$(RM) hexload.elf hexload.bin
