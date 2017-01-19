#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <stddef.h>
#include "terminal.h"
#include "printf.h"
#include "mmu040.h"
#include "input.h"

#define SRP_URP_DESCRIPTOR_BITS 9

#define ROOT_LEVEL_DESCRIPTOR_BITS 7
#define ROOT_LEVEL_DESCRIPTORS (1 << ROOT_LEVEL_DESCRIPTOR_BITS)
#define POINTER_LEVEL_DESCRIPTOR_BITS 7
#define POINTER_LEVEL_DESCRIPTORS (1 << POINTER_LEVEL_DESCRIPTOR_BITS)
#define PAGE_LEVEL_DESCRIPTOR_BITS 6
#define PAGE_LEVEL_DESCRIPTORS (1 << PAGE_LEVEL_DESCRIPTOR_BITS)

#define ROOT_LEVEL_ADDR_FIELD(a) ((a) >> (POINTER_LEVEL_DESCRIPTOR_BITS + 2))
#define ROOT_LEVEL_FIELD_ADDR(f) ((f) << (POINTER_LEVEL_DESCRIPTOR_BITS + 2))
#if PAGE_SIZE == 4096
#define POINTER_LEVEL_ADDR_FIELD(a) ((a) >> (PAGE_LEVEL_DESCRIPTOR_BITS + 1))
#define POINTER_LEVEL_FIELD_ADDR(f) ((f) << (PAGE_LEVEL_DESCRIPTOR_BITS + 1))
#else
#define POINTER_LEVEL_ADDR_FIELD(a) ((a) >> (PAGE_LEVEL_DESCRIPTOR_BITS + 2))
#define POINTER_LEVEL_FIELD_ADDR(f) ((f) << (PAGE_LEVEL_DESCRIPTOR_BITS + 2))
#endif
#define PAGE_LEVEL_ADDR_FIELD(a) ((a) >> (PAGE_OFFSET_BITS))
#define PAGE_LEVEL_FIELD_ADDR(f) ((f) << (PAGE_OFFSET_BITS))

#define PAGE_DESCRIPTORS_8K 32
#define PAGE_DESCRIPTORS_4K 64

#define UDT_IS_RESIDENT(x) ((x) == MMU040_UPPER_LEVEL_DESCRIPTOR_TYPE_RESIDENT || (x) == MMU040_UPPER_LEVEL_DESCRIPTOR_TYPE_RESIDENT_ALT)
#define PDT_IS_RESIDENT(x) ((x) == MMU040_PAGE_DESCRIPTOR_TYPE_RESIDENT || (x) == MMU040_PAGE_DESCRIPTOR_TYPE_RESIDENT_ALT)

uint32_t allocated_frames;

static Mmu040RootTableDescriptor *root_td = (void *) SDRAM_BASE;
static Mmu040PointerTableDescriptor *pointer_td = (void *) (SDRAM_BASE + ROOT_LEVEL_DESCRIPTORS*sizeof(Mmu040RootTableDescriptor));
static Mmu040PageTableDescriptor *page_td = (void *) (SDRAM_BASE + PAGE_SIZE);

static void _get_table_indices(uint32_t virtual_address, uint32_t *root_table_index, uint32_t *pointer_table_index, uint32_t *page_table_index) {
	*root_table_index = (virtual_address >> (32 - ROOT_LEVEL_DESCRIPTOR_BITS)) & (ROOT_LEVEL_DESCRIPTORS - 1);
	*pointer_table_index = (virtual_address >> (32 - ROOT_LEVEL_DESCRIPTOR_BITS - POINTER_LEVEL_DESCRIPTOR_BITS)) & (POINTER_LEVEL_DESCRIPTORS - 1);
	*page_table_index = (virtual_address >> PAGE_OFFSET_BITS) & (PAGE_LEVEL_DESCRIPTORS - 1);
}

static void *_allocate_frame() {
	void *frame;
	
	allocated_frames++;
	frame = (void *) (SDRAM_BASE + PAGE_SIZE*allocated_frames);
	
	//printf("allocated frame at 0x%X\n", frame);
	
	#if PAGE_SIZE == 4096
	mmu040_zero_4k(frame);
	#else
	mmu040_zero_4k(frame);
	mmu040_zero_4k(frame + 4096);
	#endif
	
	return frame;
}

static Mmu040PointerTableDescriptor *_allocate_pointer_level_table() {
	pointer_td += POINTER_LEVEL_DESCRIPTORS;
	
	if(!(((uint32_t) pointer_td) & (POINTER_LEVEL_DESCRIPTORS - 1))) {
		pointer_td = _allocate_frame();
	}
	
	return pointer_td;
}

static Mmu040PageTableDescriptor *_allocate_page_table() {
	page_td += PAGE_DESCRIPTORS;
	
	if(!(((uint32_t) page_td) & (PAGE_DESCRIPTORS - 1))) {
		page_td = _allocate_frame();
	}
	
	return page_td;
}

void mmu040_init() {
	Mmu040RegTranslationControl tc = {
		.page_size = MMU040_PAGE_SIZE_4K,
		.enable = false,
	};
	
	Mmu040RegTransparentTranslation ttr0 = {
		.enable = true,
		.logical_address_mask = 0x0,
		.logical_address_base = 0x0,
		
		.s_field = MMU040_S_FIELD_MATCH_SUPERVISOR,
		.cache_mode = MMU040_CACHE_MODE_NOCACHE_SERIALIZE,
		.write_protected = false,
	};
	
	Mmu040RegTransparentTranslation ttr1 = {
		.enable = false,
	};
	
	mmu040_zero_4k((void *) SDRAM_BASE);
	mmu040_zero_4k((void *) SDRAM_BASE + PAGE_SIZE);
	allocated_frames = 2;
	
	uint32_t root_table_index, pointer_table_index, page_table_index;
	_get_table_indices(MEM_MMU_TABLE_AREA, &root_table_index, &pointer_table_index, &page_table_index);
	
	root_td[root_table_index].table.table_address = ROOT_LEVEL_ADDR_FIELD((uint32_t) pointer_td);
	root_td[root_table_index].table.upper_level_descriptor_type = MMU040_UPPER_LEVEL_DESCRIPTOR_TYPE_RESIDENT;
	
	pointer_td[pointer_table_index].table.table_address = POINTER_LEVEL_ADDR_FIELD((uint32_t) page_td);
	pointer_td[pointer_table_index].table.upper_level_descriptor_type = MMU040_UPPER_LEVEL_DESCRIPTOR_TYPE_RESIDENT;
	
	page_td[page_table_index].page.physical_address = PAGE_LEVEL_ADDR_FIELD(SDRAM_BASE);
	page_td[page_table_index].page.supervisor = true;
	page_td[page_table_index].page.cache_mode = MMU040_CACHE_MODE_NOCACHE;
	page_td[page_table_index].page.page_descriptor_type = MMU040_PAGE_DESCRIPTOR_TYPE_RESIDENT;
	
	page_td[page_table_index + 1].page.physical_address = PAGE_LEVEL_ADDR_FIELD(SDRAM_BASE + PAGE_SIZE);
	page_td[page_table_index + 1].page.supervisor = true;
	page_td[page_table_index + 1].page.cache_mode = MMU040_CACHE_MODE_NOCACHE;
	page_td[page_table_index + 1].page.page_descriptor_type = MMU040_PAGE_DESCRIPTOR_TYPE_RESIDENT;
	
	Mmu040RegRootPointer srp = {
		.root_pointer = ((uint32_t) root_td) >> SRP_URP_DESCRIPTOR_BITS,
	};
	
	Mmu040RegRootPointer urp = {
		.root_pointer = 0x0,
	};
	
	mmu_set_srp(&srp);
	mmu_set_urp(&urp);
	
	mmu_set_ittr(&ttr0, 0);
	mmu_set_dttr(&ttr0, 0);
	mmu_set_ittr(&ttr1, 1);
	mmu_set_dttr(&ttr1, 1);
	
	mmu_set_tc(&tc);
}


void *mmu040_allocate_frame(uint32_t virtual_address, bool write_protected) {
	uint32_t root_table_index, pointer_table_index, page_table_index;
	Mmu040PointerTableDescriptor *pointer_table;
	Mmu040PageTableDescriptor *page_table;
	void *frame;
	
	_get_table_indices(virtual_address, &root_table_index, &pointer_table_index, &page_table_index);
	
	printf("Mapping frame to 0x%X as %s (%u %u %u)", virtual_address, write_protected ? "R" : "RW", root_table_index, pointer_table_index, page_table_index);
	
	if(UDT_IS_RESIDENT(root_td[root_table_index].table.upper_level_descriptor_type)) {
		pointer_table = (void *) ROOT_LEVEL_FIELD_ADDR(root_td[root_table_index].table.table_address);
	} else {
		pointer_table = _allocate_pointer_level_table();
		root_td[root_table_index].table.table_address = ROOT_LEVEL_ADDR_FIELD((uint32_t) pointer_table);
		root_td[root_table_index].table.upper_level_descriptor_type = MMU040_UPPER_LEVEL_DESCRIPTOR_TYPE_RESIDENT;
	}
	
	if(UDT_IS_RESIDENT(pointer_table[pointer_table_index].table.upper_level_descriptor_type)) {
		page_table = (void *) POINTER_LEVEL_FIELD_ADDR(pointer_table[pointer_table_index].table.table_address);
	} else {
		page_table = _allocate_page_table();
		pointer_table[pointer_table_index].table.table_address = POINTER_LEVEL_ADDR_FIELD((uint32_t) page_table);
		pointer_table[pointer_table_index].table.upper_level_descriptor_type = MMU040_UPPER_LEVEL_DESCRIPTOR_TYPE_RESIDENT;
	}
	
	if(PDT_IS_RESIDENT(page_table[page_table_index].page.page_descriptor_type)) {
		page_table[page_table_index].page.write_protected = write_protected;
		frame = (void *) PAGE_LEVEL_FIELD_ADDR(page_table[page_table_index].page.physical_address);
	} else {
		frame = _allocate_frame();
		
		page_table[page_table_index].page.physical_address = PAGE_LEVEL_ADDR_FIELD((uint32_t) frame);
		page_table[page_table_index].page.supervisor = true;
		page_table[page_table_index].page.cache_mode = MMU040_CACHE_MODE_CACHE_COPY_BACK;
		page_table[page_table_index].page.write_protected = write_protected;
		page_table[page_table_index].page.page_descriptor_type = MMU040_PAGE_DESCRIPTOR_TYPE_RESIDENT;
	}
	
	printf(" frame 0x%X\n", frame);
	return frame;
}

void *mmu040_get_physical_manual(uint32_t virtual_address) {
	uint32_t root_table_index, pointer_table_index, page_table_index;
	Mmu040PointerTableDescriptor *pointer_table;
	Mmu040PageTableDescriptor *page_table;
	void *frame;
	
	_get_table_indices(virtual_address, &root_table_index, &pointer_table_index, &page_table_index);
	
	if(UDT_IS_RESIDENT(root_td[root_table_index].table.upper_level_descriptor_type)) {
		pointer_table = (void *) ROOT_LEVEL_FIELD_ADDR(root_td[root_table_index].table.table_address);
	} else {
		printf("Failed root table lookup\n");
		return NULL;
	}
	
	if(UDT_IS_RESIDENT(pointer_table[pointer_table_index].table.upper_level_descriptor_type)) {
		page_table = (void *) POINTER_LEVEL_FIELD_ADDR(pointer_table[pointer_table_index].table.table_address);
	} else {
		printf("Failed pointer table lookup\n");
		return NULL;
	}
	
	if(PDT_IS_RESIDENT(page_table[page_table_index].page.page_descriptor_type)) {
		frame = (void *) PAGE_LEVEL_FIELD_ADDR(page_table[page_table_index].page.physical_address);
	} else {
		printf("Failed page table lookup\n");
		return NULL;
	}
	return frame;
}

void mmu_bus_error() {
	terminal_set_fg(TERMINAL_COLOR_LIGHT_RED);
	terminal_set_pos(0, 29);
	terminal_puts("*** BIOS PANIC: bus error ***");
	
	for(;;);
}
