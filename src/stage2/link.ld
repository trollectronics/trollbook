_stack = 0x100000;

MEMORY {
	vram(rwx): ORIGIN = 0x80000, LENGTH = 384000
	ram(rwx): ORIGIN = 0xDDC00, LENGTH = 140288
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
