#ifndef __FAT_H__
#define __FAT_H__

#include <stdint.h>
#include <stdbool.h>

#define	O_WRONLY		2
#define	O_RDONLY		1
#define	O_RDWR			(O_RDONLY | O_WRONLY)
extern uint8_t *sector_buff;

struct FATDirList {
	char		filename[13];
	uint8_t		attrib;
	uint8_t		padding[2];
} __attribute__((packed));

int init_fat();
void fat_close(int fd);
int fat_open(const char *path, int flags);
uint32_t fat_fsize(int fd);
uint32_t fat_ftell(int fd);
bool fat_read_sect(int fd);
bool fat_write_sect(int fd);
bool delete_file(const char *path);
bool create_file(char *path, char *name, uint8_t attrib);
uint8_t fat_get_stat(const char *path);
void fat_set_stat(const char *path, uint8_t stat);
void fat_set_fsize(const char *path, uint32_t size);
int fat_dirlist(const char *path, struct FATDirList *list, int size, int skip);
bool fat_valid();

/* These should be provided by you */
/* Returns < 0 on error */
int write_sector_call(uint32_t sector, uint8_t *data);
int read_sector_call(uint32_t sector, uint8_t *data);


#endif
