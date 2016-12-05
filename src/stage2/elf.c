#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "elf.h"
#include "mmu040.h"
#include "terminal.h"
#include "mem.h"

int (*(elf_load(void *elf)))(int argc, char **argv) {
	ElfHeader *header = elf;
	ElfSectionHeader *section_header;
	int i;
	int (*entry)(int argc, char **argv);
	unsigned int count;
	bool write_protected;
	void *p;

	if (header->ident[0] != ELF_MAGIC1 || header->ident[1] != ELF_MAGIC2 || header->ident[2] != ELF_MAGIC3 ||
	    header->ident[3] != ELF_MAGIC4) {
		//term_puts("Invalid ELF magic\n", MEM_PAL_ERR);
		return NULL;
	}
	
	if (header->ident[4] != ELF_CLASS_32BIT) {
		//term_puts("This ELF is not for 32-bit systems\n", MEM_PAL_ERR);
		return NULL;
	}

	if (header->ident[5] != ELF_ENDIAN_BIG) {
		//term_puts("This ELF is not in the correct byte endian\n", MEM_PAL_ERR);
		return NULL;
	}

	if (header->type != ELF_TYPE_EXEC) {
		//term_puts("This ELF is not executable\n", MEM_PAL_ERR);
		return NULL;
	}

	if (header->machine != ELF_MACHINE_M68K) {
		//term_puts("This ELF is not executable by a Motorola 68000 processor\n", MEM_PAL_ERR);
		return NULL;
	}

	if (!header->entry) {
		//term_puts("This ELF has no entry point. Assuming 0x10000000\n", MEM_PAL_WARN);
		return NULL;
	}
	
	section_header = elf + header->section_header_offset;

	for (i = 0; i < header->section_header_entry_count; i++, section_header = ((void *) section_header) + header->section_header_entry_size) {
		if (!section_header->address)
			/* If we're lot loading it, don't pay attention */
			continue;

		if (!section_header->type)
			continue;
		
		if (!(section_header->flags & ELF_SECTION_HEADER_FLAG_ALLOC))
			continue;
		
		write_protected = !(section_header->flags & ELF_SECTION_HEADER_FLAG_WRITE);
		count = (section_header->size + (PAGE_SIZE - 1))/PAGE_SIZE;
		if (section_header->type == ELF_SECTION_HEADER_TYPE_PROGRAM_NOBITS) {
			/*BSS segment*/
			for(i = 0; i < count; i++) {
				p = mmu040_allocate_frame((section_header->address & 0xFFF) + PAGE_SIZE*i, write_protected);
				memset(p, 0, PAGE_SIZE);
			}
			continue;
		}
		
		for(i = 0; i < count; i++) {
			p = mmu040_allocate_frame((section_header->address & 0xFFF) + PAGE_SIZE*i, write_protected);
			memcpy(p, elf + section_header->offset + PAGE_SIZE*i, PAGE_SIZE);
		}
		
	}
	
	entry = (void *) header->entry;
	
	/* Allocate stack */
	mmu040_allocate_frame(UINT_MAX - PAGE_SIZE + 1, false);
	return entry;
}
