MAKEFLAGS	+=	--no-print-directory

hexload:
	m68k-elf-as -m68040 -o bootrom.elf hexload.s
	make bootrom.bin

binload:
	m68k-elf-as -m68060 -o bootrom.elf bootrom.S
	#m68k-elf-as -m68060 -o bootrom.elf arne.s
	make bootrom.bin

hexload-new:
	m68k-elf-as -m68040 -o bootrom.elf hexload-new.s
	make bootrom.bin

bootrom.bin: bootrom.elf
	m68k-elf-objcopy -O binary bootrom.elf bootrom.bin
	xxd -g 4 -c 4 -p < bootrom.bin | tr "\n" ' '
	@echo
	hexwrite ../hdl/bootrom.hex record_width=4 word_size=4 input=bootrom.bin
	
