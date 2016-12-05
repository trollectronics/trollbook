#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>
#include "terminal.h"
#include "printf.h"
#include "mmu040.h"

#define SRP_URP_DESCRIPTOR_BITS 9

#define ROOT_LEVEL_DESCRIPTOR_BITS 7
#define ROOT_LEVEL_DESCRIPTORS (1 << ROOT_LEVEL_DESCRIPTOR_BITS)
#define POINTER_LEVEL_DESCRIPTOR_BITS 7
#define POINTER_LEVEL_DESCRIPTORS (1 << POINTER_LEVEL_DESCRIPTOR_BITS)

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
	*page_table_index = (virtual_address >> PAGE_OFFSET_BITS) & (PAGE_SIZE - 1);
}

static void *_allocate_frame() {
	//TODO: optimize with faster memset
	void *frame;
	
	allocated_frames++;
	frame = (void *) (SDRAM_BASE + PAGE_SIZE*allocated_frames);
	
	printf("allocated frame at 0x%X\n", frame);
	
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
	
	root_td[root_table_index].table.table_address = (((uint32_t) pointer_td) >> ROOT_LEVEL_DESCRIPTOR_BITS);
	root_td[root_table_index].table.upper_level_descriptor_type = MMU040_UPPER_LEVEL_DESCRIPTOR_TYPE_RESIDENT;
	
	pointer_td[pointer_table_index].table.table_address = (((uint32_t) page_td) >> POINTER_LEVEL_DESCRIPTOR_BITS);
	pointer_td[pointer_table_index].table.upper_level_descriptor_type = MMU040_UPPER_LEVEL_DESCRIPTOR_TYPE_RESIDENT;
	
	page_td[page_table_index].page.physical_address = (SDRAM_BASE >> PAGE_OFFSET_BITS);
	page_td[page_table_index].page.supervisor = true;
	page_td[page_table_index].page.cache_mode = MMU040_CACHE_MODE_NOCACHE;
	page_td[page_table_index].page.page_descriptor_type = MMU040_PAGE_DESCRIPTOR_TYPE_RESIDENT;
	
	page_td[page_table_index + 1].page.physical_address = ((SDRAM_BASE + PAGE_SIZE) >> PAGE_OFFSET_BITS);
	page_td[page_table_index + 1].page.supervisor = true;
	page_td[page_table_index + 1].page.cache_mode = MMU040_CACHE_MODE_NOCACHE;
	page_td[page_table_index + 1].page.page_descriptor_type = MMU040_PAGE_DESCRIPTOR_TYPE_RESIDENT;
	
	Mmu040RegRootPointer srp = {
		.root_pointer = ((uint32_t) root_td) >> SRP_URP_DESCRIPTOR_BITS,
	};
	mmu_set_srp(&srp);
	
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
	
	if(UDT_IS_RESIDENT(root_td[root_table_index].table.upper_level_descriptor_type)) {
		pointer_table = (void *) (root_td[root_table_index].table.table_address << ROOT_LEVEL_DESCRIPTOR_BITS);
	} else {
		pointer_table = _allocate_pointer_level_table();
		root_td[root_table_index].table.table_address = (((uint32_t) pointer_table) >> ROOT_LEVEL_DESCRIPTOR_BITS);
		root_td[root_table_index].table.upper_level_descriptor_type = MMU040_UPPER_LEVEL_DESCRIPTOR_TYPE_RESIDENT;
	}
	
	if(UDT_IS_RESIDENT(pointer_table[pointer_table_index].table.upper_level_descriptor_type)) {
		page_table = (void *) (pointer_table[pointer_table_index].table.table_address << POINTER_LEVEL_DESCRIPTOR_BITS);
	} else {
		page_table = _allocate_page_table();
		pointer_table[pointer_table_index].table.table_address = (((uint32_t) page_table) >> POINTER_LEVEL_DESCRIPTOR_BITS);
		pointer_table[pointer_table_index].table.upper_level_descriptor_type = MMU040_UPPER_LEVEL_DESCRIPTOR_TYPE_RESIDENT;
	}
	
	if(PDT_IS_RESIDENT(page_table[page_table_index].page.page_descriptor_type)) {
		page_table[page_table_index].page.write_protected = write_protected;
		frame = (void *) (page_table[page_table_index].page.physical_address << PAGE_OFFSET_BITS);
	} else {
		frame = _allocate_frame();
		
		page_table[page_table_index].page.physical_address =  (((uint32_t) frame) >> PAGE_OFFSET_BITS);
		page_table[page_table_index].page.supervisor = true;
		page_table[page_table_index].page.cache_mode = MMU040_CACHE_MODE_CACHE_COPY_BACK;
		page_table[page_table_index].page.write_protected = write_protected;
		page_table[page_table_index].page.page_descriptor_type = MMU040_PAGE_DESCRIPTOR_TYPE_RESIDENT;
	}
	
	return frame;
}


void mmu_bus_error() {
	terminal_set_fg(TERMINAL_COLOR_LIGHT_RED);
	terminal_set_pos(0, 29);
	terminal_puts("*** BIOS PANIC: bus error ***");
	
	for(;;);
}
