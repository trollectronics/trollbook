_stack = 0x100000;

MEMORY {
	vram(rwx): ORIGIN = 0x80000, LENGTH = 384000
	bios(rwx): ORIGIN = 0xDDC00, LENGTH = 0x100
	stage2(rwx): ORIGIN = 0xDDD00, LENGTH = 0x19000
	ram(rwx): ORIGIN = 0xF6D00, LENGTH = 0x9300
}

SECTIONS {
	.text : {
		*(.text)
		*(.rodata*)
	} > ram
	.data : {
		*(.data)
	} > ram
	.bss :  {
		__bss_start = .;
		*(.bss) 
		*(COMMON)
		__bss_end = .;
	} > ram
}

ENTRY(_start)
