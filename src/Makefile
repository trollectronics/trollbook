all:
	m68k-elf-as -m68040 -o bootrom.elf bootrom.S
	m68k-elf-objcopy -O binary bootrom.elf bootrom.bin
