_stack = 0x20000000;

MEMORY {
	vram(rwx): ORIGIN = 0x80000, LENGTH = 384000
	bios(rwx): ORIGIN = 0xDDC00, LENGTH = 0x100
	stage2(rwx): ORIGIN = 0xDDD00, LENGTH = 0x19000
	sram(rwx): ORIGIN = 0xF6D00, LENGTH = 0x9300
	virtual(rwx): ORIGIN = 0x80000000, LENGTH = 128M
	dram(rwx): ORIGIN = 0x80000000, LENGTH = 64M
}

SECTIONS {
	.text : {
		*(.text)
		*(.rodata*)
	} > virtual
	
	.data ALIGN(4096): {
		*(.data)
	}
	
	.bss ALIGN(4096): {
		__bss_start = .;
		*(.bss) 
		*(COMMON)
		__bss_end = .;
	}
	end = .;
}

ENTRY(_start)
