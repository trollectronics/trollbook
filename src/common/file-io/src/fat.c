/*
Copyright (c) 2015 Steven Arnow <s@rdw.se>
Adapted for trollbook bios in 2016 by Axel Isaksson
'fat.c' - This file is part of µCFAT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "fat.h"

#define	MAX_FD_OPEN		4
#define	READ_WORD(x, n)		((uint16_t) ((uint16_t) (x[(n)]) | (((uint16_t) (x[(n) + 1])) << 8)))
#define	READ_DWORD(x, n)	read_dword(x, n)
#define	WRITE_WORD(x, n, w)	((x)[(n)] = (w & 0xFF), (x)[(n) + 1] = ((w) >> 8) & 0xFF)
#define	WRITE_DWORD(x, n, dw)	write_dword(x, n, dw)
#define	GET_ENTRY_CLUSTER(e)	(((READ_WORD(sector_buff, (e) * 32 + 20) << 16) | (READ_WORD(sector_buff, (e) * 32 + 26))) & (fat_state.type != FAT_TYPE_FAT32 ? 0xFFFF : ~0))

int (* write_sector_call)(uint32_t sector, uint8_t *data);
int (* read_sector_call)(uint32_t sector, uint8_t *data);

static uint32_t read_dword(uint8_t *buff, int byte) {
	uint32_t dw;

	dw = READ_WORD(buff, byte);
	dw |= (READ_WORD(buff, byte + 2) << 16);
	return dw;
}

static void write_dword(uint8_t *buff, int byte, uint32_t dw) {
	WRITE_WORD(buff, byte, dw & 0xFFFF);
	WRITE_WORD(buff, byte + 2, dw >> 16);
}

static uint32_t locate_record(const char *path, int *record_index, const char *tail);

uint8_t *sector_buff;


struct {
	bool		valid;
	uint8_t		cluster_size;
	uint16_t	root_directory_size;
	uint32_t	clusters;
	uint32_t	fat_size;
	uint32_t	fat_pos;
	uint32_t	root_dir_pos;
	uint32_t	data_region;
	enum FATType	type;
} fat_state;

struct FATFileDescriptor {
	int32_t		key;
	uint32_t	entry_sector;
	uint32_t	entry_index;
	uint32_t	first_cluster;
	uint32_t	current_cluster;
	uint32_t	fpos;
	uint32_t	file_size;
	bool		write;
};

static struct FATFileDescriptor fat_fd[MAX_FD_OPEN];
static int32_t fat_key = 0;


static int fat_memcmp(uint8_t *s1, uint8_t *s2, uint32_t n) {
	for (; n; s1++, s2++, n--)
		if (*s1 != *s2)
			return *s1 - *s2;
	return 0;
}


static void fat_strncpy(uint8_t *dest, uint8_t *src, uint32_t max) {
	for (; max && *src; src++, dest++, max--)
		*dest = *src;
	return;
}


static bool end_of_chain_mark(uint32_t cluster) {
	if (fat_state.type == FAT_TYPE_FAT16)
		return cluster >= 0xFFF8 ? true : false;
	return cluster >= 0x0FFFFFF8 ? true : false;
}


static int write_sector(uint32_t sector, uint8_t *data) {
	if (!fat_state.valid)
		return -1;
	if (write_sector_call(sector, data) < 0) {
		fat_state.valid = false;
		return -1;
	}
	return 0;
}


static int read_sector(uint32_t sector, uint8_t *data) {
	if (read_sector_call(sector, data) < 0) {
		fat_state.valid = false;
		return -1;
	}
	return 0;
}


int fat_init(int (* read_func)(uint32_t sector, uint8_t *data), int (* write_func)(uint32_t sector, uint8_t *data), uint8_t *buffer) {
	uint8_t *data;
	uint16_t reserved_sectors;
	uint8_t *u8;
	uint8_t tu8;
	int err, i;
	uint32_t fsinfo;
	
	sector_buff = buffer;
	data = sector_buff;
	read_sector_call = read_func;
	write_sector_call = write_func;
	
	if ((err = read_sector(0, data) < 0))
		return err;
	
	if (READ_WORD(data, 11) != 512) {
		//printf("Only 512 bytes per sector is supported\n");
		return -1;
	}
	fat_state.cluster_size = data[13];
	reserved_sectors = READ_WORD(data, 14);
	fat_state.root_directory_size = READ_WORD(data, 17);

	u8 = &data[16];
	if (*u8 != 2) {
		//printf("Unsupported FAT: %i FAT:s in filesystem, only 2 supported\n", *u8);
		return -2;
	}
	
	if (READ_WORD(data, 19)) {
		fat_state.clusters = READ_WORD(data, 19) / fat_state.cluster_size;
	} else {
		fat_state.clusters = READ_DWORD(data, 32) / fat_state.cluster_size;
	}

	if (fat_state.clusters < 4085) {
		return -1;
	} else if (fat_state.clusters < 65525) {
		fat_state.type = FAT_TYPE_FAT16;
	} else {
		fat_state.type = FAT_TYPE_FAT32;
	}

	if (fat_state.type <= FAT_TYPE_FAT16)
		tu8 = data[38];
	else
		tu8 = data[66];
	if (tu8 != 0x28 && tu8 != 0x29) {
		//printf("FAT signature check failed\n");
		return -3;
	}

	if (fat_state.type <= FAT_TYPE_FAT16) {
		fat_state.fat_size = READ_WORD(data, 22);
	} else {
		fat_state.fat_size = READ_DWORD(data, 36);
	}
	
	fat_state.fat_pos = reserved_sectors;
	fat_state.root_dir_pos = fat_state.fat_pos + fat_state.fat_size * 2;
	fat_state.data_region = fat_state.root_dir_pos + READ_WORD(data, 17) * 32 / 512;
	if (fat_state.type == FAT_TYPE_FAT32) {
		fat_state.data_region = fat_state.root_dir_pos;
		fat_state.root_dir_pos = fat_state.fat_pos + fat_state.fat_size * 2 + fat_state.cluster_size * (READ_DWORD(data, 44) - 2);

		/* Invalidate free space counter */
		fsinfo = READ_WORD(data, 48);
		read_sector(fsinfo, data);
		if (READ_DWORD(data, 0) == 0x41615252 && READ_WORD(data, 510) == 0xAA55) {
			WRITE_DWORD(data, 488, 0xFFFFFFFF);
			write_sector(fsinfo, data);
		}
	}


	for (i = 0; i < MAX_FD_OPEN; i++)
		fat_fd[i].key = -1;
	fat_state.valid = true;

	return 0;
}


static void fname_to_fatname(char *name, char *fname) {
	int i, j;

	for (i = 0; i < 8 && name[i] != '.' && name[i]; i++)
		fname[i] = name[i];
	if (i < 8 && name[i]) {
		for (j = i; j < 8; j++)
			fname[j] = ' ';
		i++;
		for (; j < 11 && name[i]; j++, i++)
			fname[j] = name[i];
		for (; j < 11; j++)
			fname[j] = ' ';
	} else if (i == 8 && name[i] == '.') {
		i++, j = 8;
		for (; j < 11 && name[i]; j++, i++)
			fname[j] = name[i];
		for (; j < 11; j++)
			fname[j] = ' ';
	} else {
		for (; i < 11; i++)
			fname[i] = ' ';
	}

	return;
}


static uint32_t cluster_to_sector(uint32_t cluster) {
	if (!cluster)
		return 0;
	return (cluster - 2) * fat_state.cluster_size + fat_state.data_region;
}

static uint32_t sector_to_cluster(uint32_t sector) {
	if (!sector)
		return 0;
	return (sector - fat_state.data_region) / fat_state.cluster_size + 2;
}


static uint32_t next_cluster(uint32_t prev_cluster) {
	uint32_t cluster_pos, cluster_sec;

	if (fat_state.type == FAT_TYPE_FAT16) {
		cluster_pos = prev_cluster << 1;
		cluster_sec = cluster_pos >> 9;
		cluster_pos &= 0x1FF;
		cluster_sec += fat_state.fat_pos;
		if (read_sector(cluster_sec, sector_buff) < 0)
			return 0;
			
		cluster_sec = READ_WORD(sector_buff, cluster_pos);
	} else {
		cluster_pos = prev_cluster << 2;
		cluster_sec = cluster_pos >> 9;
		cluster_pos &= 0x1FF;
		cluster_sec += fat_state.fat_pos;
		if (read_sector(cluster_sec, sector_buff) < 0)
			return 0;

		cluster_sec = READ_DWORD(sector_buff, cluster_pos) & 0x0FFFFFFF;
	}

	if (end_of_chain_mark(cluster_sec))
		return 0;

	return cluster_sec;
}


static void dealloc_fat(uint32_t fat_entry) {
	uint32_t sector, index;
	int shift_sec, shift_index;
	if (fat_state.type == FAT_TYPE_FAT16) {
		shift_sec = 8;
		shift_index = 1;
	} else {
		shift_sec = 7;
		shift_index = 2;
	}

	for (;;) {
		if (end_of_chain_mark(fat_entry) || !fat_entry)
			return;
		sector = (fat_entry >> shift_sec) + fat_state.fat_pos;
		if (read_sector(sector, sector_buff) < 0)
			return;
		valid_sector:

		index = (fat_entry << shift_index) & 0x1FF;
		fat_entry = READ_WORD(sector_buff, index);
		if (shift_index == 1) {
			fat_entry = READ_WORD(sector_buff, index);
			WRITE_WORD(sector_buff, index, 0);
		} else {
			fat_entry = READ_DWORD(sector_buff, index);
			WRITE_DWORD(sector_buff, index, 0);
		}
		if (end_of_chain_mark(fat_entry) || ((fat_entry >> shift_sec) + fat_state.fat_pos != sector)) {
			write_sector(sector, sector_buff);
			write_sector(sector + fat_state.fat_size, sector_buff);
		} else
			goto valid_sector;
	}
}


static uint32_t locate_record(const char *path, int *record_index, const char *tail) {
	char component[13], fatname[11];
	int i = 0;
	uint32_t cur_sector;
	bool recurse = false, file = false;

	cur_sector = fat_state.root_dir_pos;

	found_component:
	while (*path == '/')
		path++;
	if (!*path) {
		if (tail) {
			path = tail;
			if (recurse)
				goto recurse;
		} else if (!recurse)
			return 0;
		else {
			*record_index = i;
			return cur_sector;
		}
	} else if (recurse) {
		recurse:
		cur_sector = GET_ENTRY_CLUSTER(i);
		cur_sector = cluster_to_sector(cur_sector);
	}
	fat_strncpy((uint8_t *) component, (uint8_t *) path, 13);
	for (i = 0; *path && *path != '/'; i++)
		path++;
	if (*path)
		path++;
	if (i < 13)
		component[i] = 0;
	if (file && *path)	/* A non-directory mid-path */
		return 0;
	fname_to_fatname(component, fatname);
	for (;;) {
		if (read_sector(cur_sector, sector_buff) < 0)
			return 0;

		for (i = 0; i < 16; i++) {
			if ((sector_buff[i * 32 + 11] & 0xF) == 0xF)	/* Long filename entry */
				continue;
			if (!sector_buff[i * 32])	/* End of list */
				return 0;
			if (sector_buff[i * 32] == 0xE5)
				/* Deleted record, skip */
				continue;
			/* This is a valid file */

			if (!fat_memcmp((uint8_t *) fatname, &sector_buff[i * 32], 11)) {
				recurse = true;
				file = (sector_buff[i * 32 + 11] & 0x10) ? false : true;
				goto found_component;
			}
		}
		
		if (!recurse) {	/* Don't cross cluster boundary for root directory */
			if (fat_state.type == FAT_TYPE_FAT16) {
				cur_sector++;	/* Lets just cross our fingers and hope we don't overrun */
				continue;
			}
		}

		if (cur_sector / fat_state.cluster_size != (cur_sector + 1) / fat_state.cluster_size) {
			cur_sector = cluster_to_sector(next_cluster(sector_to_cluster(cur_sector)));
			if (!cur_sector)
				return 0;
		} else
			cur_sector++;
	}
}


static uint32_t alloc_cluster(uint32_t entry_sector, uint32_t entry_index, uint32_t old_cluster) {
	uint32_t i, j;
	uint32_t cluster = 0, avail_cluster;
	int fat_size, ent_per_sec, eocm, mask, shift;
	
	if (fat_state.type == FAT_TYPE_FAT16) {
		fat_size = 2;
		ent_per_sec = 256;
		eocm = 0xFFFF;
		mask = 0xFF;
		shift = 1;
	} else {
		fat_size = 4;
		ent_per_sec = 128;
		eocm = 0xFFFFFFF;
		mask = 0x7F;
		shift = 2;
	}


	for (i = 0; i < fat_state.fat_size; i++) {
		if (read_sector(fat_state.fat_pos + i, sector_buff) < 0)
			return 0;

		for (j = 0; j < 512; j += fat_size) {
			avail_cluster = (fat_size == 2) ? READ_WORD(sector_buff, j) : READ_DWORD(sector_buff, j);
			if (!avail_cluster) {
				cluster = i * ent_per_sec + j / fat_size;
				if (fat_size == 2)
					WRITE_WORD(sector_buff, j, eocm);
				else
					WRITE_DWORD(sector_buff, j, eocm);
				write_sector(fat_state.fat_pos + i, sector_buff);
				write_sector(fat_state.fat_pos + fat_state.fat_size + i, sector_buff);
				if (old_cluster) {
					read_sector(fat_state.fat_pos + old_cluster / ent_per_sec, sector_buff);
					if (fat_size == 2)
						WRITE_WORD(sector_buff, (old_cluster & mask) << shift, cluster);
					else
						WRITE_DWORD(sector_buff, (old_cluster & mask) << shift, cluster);
					write_sector(fat_state.fat_pos + i, sector_buff);
					if (write_sector(fat_state.fat_pos + fat_state.fat_size + i, sector_buff) < 0)
						return 0;
				}

				goto allocated;
			}
		}
	}
	
	allocated:
	if (!old_cluster) {
		read_sector(entry_sector, sector_buff);
		WRITE_WORD(sector_buff, entry_index * 32 + 20, cluster >> 16);
		WRITE_WORD(sector_buff, entry_index * 32 + 26, cluster);
		write_sector(entry_sector, sector_buff);
	}

	return cluster;
}


static uint32_t do_alloc_entry(uint32_t entry_sector, uint32_t entry_index, uint32_t prev_cluster) {
	uint32_t sector;
	sector = cluster_to_sector(alloc_cluster(entry_sector, entry_index, prev_cluster));
	sector_buff[0] = 0xE5;
	sector_buff[11] = 0;
	sector_buff[32] = 0;
	sector_buff[32 + 11] = 0;
	write_sector(sector, sector_buff);
	return sector;
}


static uint32_t alloc_entry(uint32_t parent_entry_sector, uint32_t parent_entry_index, uint32_t first_cluster, int *index) {
	int i, j;
	uint32_t sector, old_cluster, old_sector = 0;

	sector = cluster_to_sector(first_cluster);
	if (!sector) {
		*index = 0;
		return do_alloc_entry(parent_entry_sector, parent_entry_index, first_cluster);
	}

	for (;;) {
		for (i = 0; i < fat_state.cluster_size; i++) {
			if (read_sector(sector + i, sector_buff) < 0)
				return 0;
			for (j = 0; j < 16; j++) {
				if (old_sector) {
					sector_buff[j * 32 + 11] = 0;
					sector_buff[j * 32] = 0;
					if (write_sector(sector, sector_buff) < 0)
						return 0;
					return old_sector;
				}

				if (sector_buff[j * 32] == 0xE5) {
					*index = j;
					return sector;
				} else if (!sector_buff[j * 32]) {
					*index = j;
					old_sector = sector;
				}
			}
		}

		old_cluster = first_cluster;
		first_cluster = next_cluster(first_cluster);
		if (!first_cluster) {
			*index = 0;
			return do_alloc_entry(parent_entry_sector, parent_entry_index, old_cluster);
		}
	}
}


static uint32_t alloc_entry_root(int *index) {
	int i, j;
	uint32_t sector, old_sector;
	bool alloc = false;

	if (fat_state.type == FAT_TYPE_FAT32) {
		return alloc_entry(0, 0, sector_to_cluster(fat_state.root_dir_pos), index);
	}

	sector = fat_state.root_dir_pos;
	for (i = 0; i < fat_state.root_directory_size / 16; i++) {
		if (read_sector(sector, sector_buff) < 0)
			return 0;

		for (j = 0; j < 16; j++) {
			if (!alloc) {
				if (sector_buff[j * 32] == 0xE5) {
					*index = j;
					return sector;
				} else if (!sector_buff[j * 32]) {
					*index = j;
					alloc = true;
					old_sector = sector;
				}
			} else {
				sector_buff[j * 32] = 0;
				sector_buff[j * 32 + 11] = 0;
				if (write_sector(sector, sector_buff) < 0)
					return 0;
					
				return old_sector;
			}
		}

		sector++;
	}

	return 0;
}


void fat_close(int fd) {
	int i;
	
	for (i = 0; i < MAX_FD_OPEN; i++)
		if (fat_fd[i].key == fd) {
			fat_fd[i].key = -1;
			/* TODO: Write access/modify/stuff */
			return;
		}
}


int fat_open(const char *path, int flags) {
	int i, index;
	uint32_t sector;

	if (!fat_state.valid)
		return -1;

	for (i = 0; i < MAX_FD_OPEN; i++)
		if (fat_fd[i].key < 0)
			break;
	if (i == MAX_FD_OPEN)
		return -1;
	
	if (!(sector = locate_record(path, &index, NULL)))
		return -1;
	if (read_sector(sector, sector_buff) < 0)
		return -1;

	if (sector_buff[index * 32 + 11] & 0x10)
		/* Don't allow opening a directory */
		return -1;
	fat_fd[i].write = flags&O_WRONLY?true:false;
	fat_fd[i].entry_sector = sector;
	fat_fd[i].entry_index = index;
	fat_fd[i].first_cluster = GET_ENTRY_CLUSTER(index);
	if (fat_fd[i].first_cluster == 0) {
		if (fat_fd[i].write)
			fat_fd[i].first_cluster = alloc_cluster(fat_fd[i].entry_sector, fat_fd[i].entry_index, 0);
		fat_fd[i].file_size = 0;
	} else {
		fat_fd[i].file_size = read_dword(sector_buff, index * 32 + 28);
	}

	fat_fd[i].current_cluster = fat_fd[i].first_cluster;
	fat_fd[i].fpos = 0;
	fat_fd[i].key = fat_key++;
	return fat_fd[i].key;
}


uint32_t fat_fsize(int fd) {
	int i;
	
	if (fd < 0)
		return 0;
	for (i = 0; i < MAX_FD_OPEN; i++)
		if (fat_fd[i].key == fd)
			break;
	if (i == MAX_FD_OPEN)
		return 0;
	return fat_fd[i].file_size;
}


uint32_t fat_ftell(int fd) {
	int i;

	if (fd < 0)
		return 0;
	for (i = 0; i < MAX_FD_OPEN; i++)
		if (fat_fd[i].key == fd)
			break;
	if (i == MAX_FD_OPEN)
		return 0;
	return fat_fd[i].fpos;
}


bool fat_read_sect(int fd) {
	int i;
	uint32_t old_cluster, sector;
	
	if (fd < 0)
		return false;
	for (i = 0; i < MAX_FD_OPEN; i++)
		if (fat_fd[i].key == fd)
			break;
	if (i == MAX_FD_OPEN)
		return false;
	if (!fat_fd[i].current_cluster)
		return false;
	if (fat_fd[i].fpos == fat_fd[i].file_size)
		return false;
	sector = cluster_to_sector(fat_fd[i].current_cluster) + ((fat_fd[i].fpos >> 9) % fat_state.cluster_size);
	
	fat_fd[i].fpos += 512;
	if (fat_fd[i].file_size < fat_fd[i].fpos)
		fat_fd[i].fpos = fat_fd[i].file_size & (~0x1FF);
	if (!(fat_fd[i].fpos % (fat_state.cluster_size * 512))) {
		old_cluster = fat_fd[i].current_cluster;
		fat_fd[i].current_cluster = next_cluster(fat_fd[i].current_cluster);
		if (!fat_fd[i].current_cluster && fat_fd[i].write)
			fat_fd[i].current_cluster = alloc_cluster(fat_fd[i].entry_sector, fat_fd[i].entry_index, old_cluster);
	}
	
	if (read_sector(sector, sector_buff) < 0)
		return false;

	return true;
}


bool fat_write_sect(int fd) {
	int i;
	uint32_t old_cluster, sector;

	if (fd < 0)
		return false;
	for (i = 0; i < MAX_FD_OPEN; i++)
		if (fat_fd[i].key == fd)
			break;
	if (i == MAX_FD_OPEN)
		return false;
	if (!fat_fd[i].write)
		return false;
	if (!fat_fd[i].current_cluster)
		return false;
	sector = cluster_to_sector(fat_fd[i].current_cluster);
	sector += (fat_fd[i].fpos >> 9) % fat_state.cluster_size;
	write_sector(sector, sector_buff);
	fat_fd[i].fpos += 512;
	if (!((fat_fd[i].fpos >> 9) % fat_state.cluster_size)) {
		old_cluster = fat_fd[i].current_cluster;
		fat_fd[i].current_cluster = next_cluster(fat_fd[i].current_cluster);
		if (!fat_fd[i].current_cluster && fat_fd[i].write)
			fat_fd[i].current_cluster = alloc_cluster(fat_fd[i].entry_sector, fat_fd[i].entry_index, old_cluster);
	}

	read_sector(fat_fd[i].entry_sector, sector_buff);
	WRITE_DWORD(sector_buff, fat_fd[i].entry_index * 32 + 28, fat_fd[i].fpos);
	if (write_sector(fat_fd[i].entry_sector, sector_buff) < 0)
		return 0;
	return true;
}


/* Anta att vi aldrig försöker ta bort roten */
static bool folder_empty(uint32_t cluster) {
	int i, j;
	uint32_t sector;

	for (;;) {
		sector = cluster_to_sector(cluster);
		if (!sector)
			return true;
		for (i = 0; i < fat_state.cluster_size; i++) {
			if (read_sector(sector, sector_buff) < 0)
				return false;
			for (j = 0; j < 16; j++) {
				if (sector_buff[j * 32 + 11] == 0xF)
					continue;
				if (sector_buff[j * 32] == 0)
					return true;
				if (sector_buff[j * 32] != 0xE5 && sector_buff[j * 32] && sector_buff[j * 32] != '.' && sector_buff[j * 32] != ' ')
					return false;
			}
			sector++;
		}
		cluster = next_cluster(cluster);
	}

	return true;
}


bool fat_delete_file(const char *path) {
	uint32_t i;
	int index = 0;
	uint32_t sector, cluster;
	if (!fat_state.valid)
		return false;
	sector = locate_record(path, &index, NULL);
	if (!sector)
		return false;
	for (i = 0; i < MAX_FD_OPEN; i++)
		if (fat_fd[i].entry_sector == sector && (int) fat_fd[i].entry_index == index && fat_fd[i].key >= 0)
			return false;
	read_sector(sector, sector_buff);
	if (sector_buff[index * 32 + 11] & 0x10) {
		cluster = GET_ENTRY_CLUSTER(index);
		if (!folder_empty(cluster))
			return false;
		read_sector(sector, sector_buff);
		sector_buff[index * 32 + 11] = 0;
		sector_buff[index * 32] = 0xE5;
		write_sector(sector, sector_buff);
		dealloc_fat(cluster);
		return true;
	} else {
		sector_buff[index * 32 + 11] = 0;
		sector_buff[index * 32] = 0xE5;
		cluster = GET_ENTRY_CLUSTER(index);
		write_sector(sector, sector_buff);
		dealloc_fat(cluster);
		return true;
	}
}


bool fat_create_file(char *path, char *name, uint8_t attrib) {
	uint32_t sector, pcluster, cluster;
	int index = 0, pindex = 0, i;
	if (!path) {
		if (locate_record(name, &i, NULL))
			return false;
	} else {
		if (locate_record(path, &i, name))
			return false;
	}

	if (path && *path == '/' && !*(path + 1))
		path = NULL;
	if (!path) {
		sector = alloc_entry_root(&index);
		pcluster = 0;
	} else {
		sector = locate_record(path, &pindex, NULL);
		read_sector(sector, sector_buff);
		pcluster = GET_ENTRY_CLUSTER(pindex);
		if (!sector)
			return false;
		read_sector(sector, sector_buff);
		sector = alloc_entry(sector, pindex, GET_ENTRY_CLUSTER(pindex), &index);
	}

	if (!sector)
		return false;
	read_sector(sector, sector_buff);
	for (i = 0; i < 32; i++)
		sector_buff[index * 32 + i] = 0;
	fname_to_fatname(name, (char *) &sector_buff[index * 32]);
	sector_buff[index * 32 + 11] = attrib;
	if (write_sector(sector, sector_buff) < 0)
		return false;
	
	if (attrib & 0x10) {
		pindex = index;
		sector = alloc_entry(sector, pindex, 0, &index);
		read_sector(sector, sector_buff);
		for (i = 0; i < 11; i++)
			sector_buff[index * 32 + i] = ' ';
		sector_buff[index * 32] = '.';
		cluster = sector_to_cluster(sector);
		WRITE_WORD(sector_buff, index * 32 + 20, cluster >> 16);
		WRITE_WORD(sector_buff, index * 32 + 26, cluster & 0xFFFF);
		sector_buff[index * 32 + 11] = 0x10;
		write_sector(sector, sector_buff);
		
		sector = alloc_entry(sector, pindex, cluster, &index);
		read_sector(sector, sector_buff);
		for (i = 0; i < 11; i++)
			sector_buff[index * 32 + i] = ' ';
		sector_buff[index * 32] = '.';
		sector_buff[index * 32 + 1] = '.';
		WRITE_WORD(sector_buff, index * 32 + 20, pcluster >> 16);
		WRITE_WORD(sector_buff, index * 32 + 26, pcluster & 0xFFFF);
		sector_buff[index * 32 + 11] = 0x10;
		write_sector(sector, sector_buff);
	}

	return true;
}


uint8_t fat_get_stat(const char *path) {
	uint32_t sector;
	int index = 0;
	if (!(sector = locate_record(path, &index, NULL)))
		return 0xFF;
	read_sector(sector, sector_buff);
	return sector_buff[(index << 5) + 11];
}


void fat_set_stat(const char *path, uint8_t stat) {
	uint32_t sector;
	int index = 0;
	if (!(sector = locate_record(path, &index, NULL)))
		return;
	read_sector(sector, sector_buff);
	sector_buff[(index << 5) + 11] = stat;
	write_sector(sector, sector_buff);
	return;
}


void fat_set_fsize(const char *path, uint32_t size) {
	uint32_t sector;
	int index = 0;
	if (!(sector = locate_record(path, &index, NULL)))
		return;
	read_sector(sector, sector_buff);
	WRITE_DWORD(sector_buff, (index * 32) + 28, size);
	write_sector(sector, sector_buff);
	return;
}


int fat_dirlist(const char *path, struct FATDirList *list, int size, int skip) {
	uint32_t sector, cluster, found;
	int index = 0, i, j, k, l;
	bool root = false;

	if (!path);
	else {
		while (*path == '/') path++;
		if (!*path)
			path = 0;
	}
	if (!path) {
		sector = fat_state.root_dir_pos;
		root = true;
	} else {
		if (!(sector = locate_record(path, &index, NULL)))
			return -1;
		read_sector(sector, sector_buff);
		if (!(sector_buff[index * 32 + 11] & 0x10))
			return -1;
		cluster = GET_ENTRY_CLUSTER(index);
		if (fat_state.type != FAT_TYPE_FAT32)
			cluster &= 0xFFFF;
		sector = cluster_to_sector(cluster);
	}

	found = 0;
	for (;;) {
		cluster = sector_to_cluster(sector);
		for (i = 0; i < fat_state.cluster_size; i++) {
			if ((read_sector(sector + i, sector_buff)) < 0)
				return 0;
			for (j = 0; j < 16; j++) {
				if (found >= (uint32_t) size)
					return found;
				if (sector_buff[j * 32 + 11] == 0xF)
					continue;
				if (sector_buff[j * 32] == 0xE5)
					continue;
				if (!sector_buff[j * 32])
					return found;
				if (skip) {
					skip--;
					continue;
				}

				for (k = 0; k < 8; k++) {
					if (sector_buff[j * 32 + k] == ' ')
						break;
					list[found].filename[k] = sector_buff[j * 32 + k];
				}
				
				if (sector_buff[j * 32 + 8] != ' ') {
					list[found].filename[k] = '.';
					k++;
				}

				for (l = 0; l < 3; k++, l++) {
					if (sector_buff[j * 32 + 8 + l] == ' ')
						break;
					list[found].filename[k] = sector_buff[j * 32 + 8 + l];
				}
				
				list[found].filename[k] = 0;

				list[found].attrib = sector_buff[j * 32 + 11];
				found++;
			}
			
		}
		
		if (root && fat_state.type == FAT_TYPE_FAT16) {
			sector += fat_state.cluster_size;
		} else {
			if (!(cluster = next_cluster(cluster)))
				return found;
			sector = cluster_to_sector(cluster);	
		}
	}
}


bool fat_valid() {
	return fat_state.valid;
}

FATType fat_type() {
	return fat_state.type;
}

int fat_get_label(char *buf) {
	int err;
	int i;
	
	if(!buf)
		return -1;
	*buf = 0;
	
	if((err = read_sector(0, sector_buff) < 0))
		return err;
	
	if(fat_state.type == FAT_TYPE_FAT16) {
		for(i = 43; i < 54; i++)
			*buf++ = sector_buff[i];
		*buf = 0;
	} else if (fat_state.type == FAT_TYPE_FAT32) {
		for(i = 71; i < 82; i++)
			*buf++ = sector_buff[i];
		*buf = 0;
	} else
		return -1;
	
	return 0;
}
