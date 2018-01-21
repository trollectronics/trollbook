#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "elf.h"
#include "mmu040.h"
#include "terminal.h"
#include "printf.h"
#include "mem.h"

int (*(elf_load(void *elf, bool debug)))(int argc, char **argv) {
	ElfHeader *header = elf;
	ElfProgramHeader *program_header;
	int i, j;
	int (*entry)(int argc, char **argv);
	uint32_t alloc_count, file_count;
	bool write_protected;
	void *p;

	if (header->ident[0] != ELF_MAGIC1 || header->ident[1] != ELF_MAGIC2 || header->ident[2] != ELF_MAGIC3 ||
	    header->ident[3] != ELF_MAGIC4) {
		printf("Invalid ELF magic\n");
		return NULL;
	}
	
	if (header->ident[4] != ELF_CLASS_32BIT) {
		printf("This ELF is not for 32-bit systems\n");
		return NULL;
	}

	if (header->ident[5] != ELF_ENDIAN_BIG) {
		printf("This ELF is not in the correct byte endian\n");
		return NULL;
	}

	if (header->type != ELF_TYPE_EXEC) {
		printf("This ELF is not executable\n");
		return NULL;
	}

	if (header->machine != ELF_MACHINE_M68K) {
		printf("This ELF is not executable by a Motorola 68000 processor\n");
		return NULL;
	}

	if (!header->entry) {
		printf("This ELF has no entry point. Assuming 0x10000000\n");
		return NULL;
	}
	
	program_header = elf + header->program_header_offset;
	
	for (i = 0; i < header->program_header_entry_count; i++, program_header = ((void *) program_header) + header->program_header_entry_size) {
		if (program_header->type != ELF_PROGRAM_HEADER_TYPE_LOAD) {
			printf("not loading due to non load type\n");
			continue;
		}
		
		write_protected = !(program_header->flags & ELF_PROGRAM_HEADER_FLAG_W);
		alloc_count = (program_header->memsize + (PAGE_SIZE - 1))/PAGE_SIZE;
		file_count = program_header->filesize;
		
		printf("%s @ 0x%X alloc=%u load=%u\n", write_protected ? "Text" : "Data/BSS", program_header->virtual_address, alloc_count*PAGE_SIZE, file_count);
		for(j = 0; j < alloc_count; j++) {
			p = mmu040_allocate_frame((program_header->virtual_address & ~0xFFF) + PAGE_SIZE*j, debug ? false : write_protected);
			
			if(file_count) {
				if(file_count < PAGE_SIZE) {
					memcpy(p, elf + program_header->offset + PAGE_SIZE*j, file_count);
					file_count = 0;
				} else {
					memcpy(p, elf + program_header->offset + PAGE_SIZE*j, PAGE_SIZE);
					file_count -= PAGE_SIZE;
				}
			}
		}
	}
	
	entry = (void *) header->entry;
	
	/* Allocate stack */
	printf("stack @ 0x%X count=%u\n", UINT_MAX - PAGE_SIZE + 1, PAGE_SIZE);
	mmu040_allocate_frame(UINT_MAX - PAGE_SIZE + 1, false);
	return entry;
}
