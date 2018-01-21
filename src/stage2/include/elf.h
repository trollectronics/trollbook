#ifndef _ELF_H
#define	_ELF_H

#include <stdint.h>
#include <stdbool.h>

#define	ELF_MAGIC1		0x7F
#define	ELF_MAGIC2		'E'
#define	ELF_MAGIC3		'L'
#define	ELF_MAGIC4		'F'


typedef enum ElfClass ElfClass;
enum ElfClass {
	ELF_CLASS_NONE = 0,
	ELF_CLASS_32BIT,
	ELF_CLASS_64BIT,
};

typedef enum ElfEndian ElfEndian;
enum ElfEndian {
	ELF_ENDIAN_NONE = 0,
	ELF_ENDIAN_LITTLE,
	ELF_ENDIAN_BIG,
};

typedef enum ElfType ElfType;
enum ElfType {
	ELF_TYPE_NONE = 0,
	ELF_TYPE_REL,
	ELF_TYPE_EXEC,
	ELF_TYPE_DYN,
	ELF_TYPE_CORE,
	ELF_TYPES,
};

/*This list is nowhere complete, but conains a couple of architectures
 * that may show up in practical applications */
typedef enum ElfMachine ElfMachine;
enum ElfMachine {
	ELF_MACHINE_NONE = 0,
	ELF_MACHINE_M32,
	ELF_MACHINE_SPARC,
	ELF_MACHINE_I386,
	ELF_MACHINE_M68K,
	ELF_MACHINE_M88K,
	ELF_MACHINE_I860,
	ELF_MACHINE_MIPS,
	
	ELF_MACHINE_PPC = 20,
	ELF_MACHINE_PPC64,
	
	ELF_MACHINE_ARM = 40,
	ELF_MACHINE_IA_64 = 50,
	ELF_MACHINE_X86_64 = 62,
	ELF_MACHINE_AVR = 83,
};

typedef enum ElfSectionHeaderFlag ElfSectionHeaderFlag;
enum ElfSectionHeaderFlag {
	ELF_SECTION_HEADER_FLAG_WRITE = 0x1,
	ELF_SECTION_HEADER_FLAG_ALLOC = 0x2,
	ELF_SECTION_HEADER_FLAG_EXECUTABLE = 0x4,
	ELF_SECTION_HEADER_FLAG_MERGE = 0x8,
	ELF_SECTION_HEADER_FLAG_STRINGS = 0x10,
	ELF_SECTION_HEADER_FLAG_INFO_LINK = 0x20,
	ELF_SECTION_HEADER_FLAG_LINK_ORDER = 0x40,
	ELF_SECTION_HEADER_FLAG_OS_SPECIFIC = 0x80,
	ELF_SECTION_HEADER_FLAG_GROUP = 0x100,
	ELF_SECTION_HEADER_FLAG_THREAD_LOCAL = 0x200,
};

typedef enum ElfSectionHeaderType ElfSectionHeaderType;
enum ElfSectionHeaderType {
	ELF_SECTION_HEADER_TYPE_NONE = 0,
	ELF_SECTION_HEADER_TYPE_PROGRAM_BITS,
	ELF_SECTION_HEADER_TYPE_SYMBOL_TABLE,
	ELF_SECTION_HEADER_TYPE_STRING_TABLE,
	ELF_SECTION_HEADER_TYPE_RELOCATION_ADDENTS,
	ELF_SECTION_HEADER_TYPE_HASH_TABLE,
	ELF_SECTION_HEADER_TYPE_DYNAMIC,
	ELF_SECTION_HEADER_TYPE_NOTE,
	ELF_SECTION_HEADER_TYPE_PROGRAM_NOBITS, /*Sections like the BSS segment contain no bits*/
	ELF_SECTION_HEADER_TYPE_RELOCATION,
	ELF_SECTION_HEADER_TYPE_SHLIB,
	ELF_SECTION_HEADER_TYPE_DYNSYM,
	ELF_SECTION_HEADER_TYPE_INIT_ARRAY,
	ELF_SECTION_HEADER_TYPE_FINISH_ARRAY,
	ELF_SECTION_HEADER_TYPE_PREINIT_ARRAY,
	ELF_SECTION_HEADER_TYPE_GROUP,
	ELF_SECTION_HEADER_TYPE_EXTENDED_SECTION_INDEX,
	ELF_SECTION_HEADER_TYPE,
};

typedef enum ElfProgramHeaderFlag ElfProgramHeaderFlag;
enum ElfProgramHeaderFlag {
	ELF_PROGRAM_HEADER_FLAG_X = 0x1,
	ELF_PROGRAM_HEADER_FLAG_W = 0x2,
	ELF_PROGRAM_HEADER_FLAG_R = 0x4,
};

typedef enum ElfProgramHeaderType ElfProgramHeaderType;
enum ElfProgramHeaderType {
	ELF_PROGRAM_HEADER_TYPE_NONE,
	ELF_PROGRAM_HEADER_TYPE_LOAD,
	ELF_PROGRAM_HEADER_TYPE_DYNAMIC,
	ELF_PROGRAM_HEADER_TYPE_INTERP,
	ELF_PROGRAM_HEADER_TYPE_NOTE,
	ELF_PROGRAM_HEADER_TYPE_SHLIB,
	ELF_PROGRAM_HEADER_TYPE_PHDR,
	ELF_PROGRAM_HEADER_TYPE_TLS,
	ELF_PROGRAM_HEADER_TYPE_NUM,
	
	ELF_PROGRAM_HEADER_TYPE_OS_SPECIFIC = 0x60000000UL,
	ELF_PROGRAM_HEADER_TYPE_PROCESSOR_SPECIFIC = 0x70000000UL,
};

typedef struct ElfHeader ElfHeader;
struct ElfHeader {
	uint8_t ident[16];
	uint16_t type;
	uint16_t machine;
	uint32_t version;
	uint32_t entry;
	uint32_t program_header_offset;
	uint32_t section_header_offset;
	uint32_t flags;
	uint16_t header_size;
	uint16_t program_header_entry_size;
	uint16_t program_header_entry_count;
	uint16_t section_header_entry_size;
	uint16_t section_header_entry_count;
	uint16_t section_index_string;
};

typedef struct ElfSectionHeader ElfSectionHeader;
struct ElfSectionHeader {
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t address;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t address_align;
	uint32_t entry_size;
};

typedef struct ElfProgramHeader ElfProgramHeader;
struct ElfProgramHeader {
	uint32_t type;
	uint32_t offset;
	uint32_t virtual_address;
	uint32_t physical_address;
	uint32_t filesize;
	uint32_t memsize;
	uint32_t flags;
	uint32_t align;
};

int (*(elf_load(void *elf, bool debug)))(int argc, char **argv);

#endif
