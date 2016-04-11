all:
	m68k-elf-as -m68060 -o bootrom.elf bootrom.S
	#m68k-elf-as -m68060 -o bootrom.elf arne.s
	#m68k-elf-as -m68060 -o bootrom.elf hexload.s
	m68k-elf-objcopy -O binary bootrom.elf bootrom.bin
	xxd -g 4 -c 4 -p < bootrom.bin | tr "\n" ' '
	@echo
	hexwrite ../hdl/bootrom.hex record_width=4 word_size=4 input=bootrom.bin
