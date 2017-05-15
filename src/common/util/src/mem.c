#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "mem.h"

void *memset(void *pointer, int c, size_t n) {
	uint8_t *p1 = pointer;
	while(n) {
		p1[--n] = c;
	}
	return NULL;
}

void *memcpy(void *dest, void *src, size_t n) {
	uint8_t *p1 = dest, *p2 = src;
	while(n) {
		n--;
		p1[n] = p2[n];
	}
	
	return NULL;
}

char *strcat(char *dest, const char *src) {
	char *ret = dest;
	while(*dest++);
	dest--;
	while((*dest++ = *src++));
	return ret;
}

size_t strlen(const char *c) {
	size_t i = 0;

	while(*c++)
		i++;
	return i;
}

size_t strnlen(const char *c, size_t maxlen) {
	size_t i;

	for (i = 0; i < maxlen && c[i]; i++);
	return i;
}

int isalpha(int c) {
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return 1;
	return 0;
}

int isdigit(int c) {
	if((c >= '0' && c <= '9'))
		return 1;
	return 0;
}

char *strncpy(char *dest, const char *src, size_t max) {
	size_t i;

	for (i = 0; i < max && src[i]; i++)
		dest[i] = src[i];
	if (i == max)
		dest[i - 1] = 0;
	else
		dest[i] = 0;
	return dest;
}

char *strcpy(char *dest, const char *src) {
	char *ret = dest;
	while((*dest++ = *src++));
	return ret;
}


int strcmp(const char *s1, const char *s2) {
	int i;
	for (i = 0; s1[i] && s2[i] && s1[i] == s2[i]; i++);
	if (s1[i] != s2[i])
		return ((signed) s1[i]) - ((signed) s2[i]);
	return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
	size_t i;
	if(!n)
		return 0;
	for (i = 0; i < (n - 1) && s1[i] && s2[i] && s1[i] == s2[i]; i++);
	if (s1[i] != s2[i])
		return ((signed) s1[i]) - ((signed) s2[i]);
	return 0;
}

char *strchr(char *str, char chr) {
	for (; *str; str++)
		if (*str == chr)
			return str;
	return NULL;
}

int memcmp(const void *s1, const void *s2, size_t n) {
	const char *ss1 = s1, *ss2 = s2;
	size_t i;
	if(!n)
		return 0;
	for (i = 0; i < (n - 1) && ss1[i] == ss2[i]; i++);
	if (ss1[i] != ss2[i])
		return ((signed) ss1[i]) - ((signed) ss2[i]);
	return 0;
}

extern uint8_t end;

void *sbrk(intptr_t increment) {
	static void * heap_end = &end;
	void *old_heap_end = heap_end;
	uint32_t tmp, new, pages, i;
	
	if(increment > 0) {
		tmp = ((((uint32_t) heap_end) + (PAGE_SIZE - 1)) & ~PAGE_MASK);
		new = ((((uint32_t) heap_end) + increment + (PAGE_SIZE - 1)) & ~PAGE_MASK);
		pages = (new - tmp)/PAGE_SIZE;
		
		for(i = 0; i < pages; i++) {
			//mmu_alloc_at((void *) (tmp + i*PAGE_SIZE), true, false);
			memset((void *) (tmp + i*PAGE_SIZE), 0, PAGE_SIZE);
		}			
	} else if(increment < 0) {
		//TODO: implement
	}
	
	heap_end += increment;
	return old_heap_end;
}

void *calloc(size_t nmemb, size_t size) {
	void *ret;
	if((ret = malloc(nmemb*size)))
		memset(ret, 0, nmemb*size);
	
	return ret;
}

/**
 * malloc - Implementation of malloc et al
 * malloc.c - Memory allocation library functions
 * @author Axel Isaksson <axelis@kth.se>
 * @copyright MIT/X11 License, see COPYING
 * 
 * Contains the library functions malloc(3), free(3) and realloc(3).
 * Several algorithms can be used for allocation, see README
 * 
 */


/*Macros*/
#define ALIGN(x) ((x + (sizeof(long) - 1)) & ~(sizeof(long) - 1))
#define ALIGNWITH(x, y) ((x + ((y) - 1)) & ~((y) - 1))
#define MIN(x, y) ((x) > (y) ? (y) : (x))

#ifndef STRATEGY
#define STRATEGY 4
#endif

/*This is all the difference between best fit and worst fit*/
#if STRATEGY == 3
#define FIT_COMPARE >
#else
#define FIT_COMPARE <
#endif

#ifndef NRQUICKLISTS
#define NRQUICKLISTS 4
#endif

typedef struct Header Header;
struct Header {
	size_t size;
	Header *next;
};

/*Pointer to the linked lists with free blocks*/
static Header *freelist[NRQUICKLISTS + 1];
/*Keep statistics of total used mem*/
size_t malloc_used_mem = 0;


/**
 * Allocate a new block on the heap from the system.
 * The block will be aligned to a page boundary and the block
 * size will be rounded upwards to the next page.
 * 
 * @param size Size in bytes excluding header
 * @return Newly allocated block of ram, beginning with a header
 */
static Header *getmore(size_t size) {
	Header *header;
	
	size = ALIGNWITH(size + sizeof(Header), PAGE_SIZE);
	malloc_used_mem += size;
	header = sbrk(size);
	if(header == (void *) - 1)
		return NULL;
	header->size = size - sizeof(Header);
	
	return header;
}


/**
 * Allocate memory
 * 
 * @param size Size in bytes to be allocated
 * @return Pointer to avaible block of memory, or NULL on failure
 */
void *malloc(size_t size) {
	Header **iter, *header;
	#if STRATEGY != 1
	Header **tmp;
	#endif
	#if STRATEGY == 4
	int list, i;
	
	for(list = 0; list < NRQUICKLISTS && size > (8 << list); list++);
	#endif
	
	if(!size)
		return NULL;
	
	for(;;) {
		#if STRATEGY == 1
		/*First fit*/
		for(iter = &freelist[NRQUICKLISTS]; *iter && (*iter)->size < size; iter = &(*iter)->next);
		#elif STRATEGY == 4
		/*Quick fit*/
		for(iter = &freelist[list]; *iter && (*iter)->size < size; iter = &(*iter)->next);
		#else
		/*Best/worst fit*/
		tmp = NULL;
		for(iter = &freelist[NRQUICKLISTS]; *iter; iter = &(*iter)->next)
			if((*iter)->size >= size && (!tmp || (*iter)->size FIT_COMPARE (*tmp)->size))
				tmp = iter;
		if(tmp)
			iter = tmp;
		#endif
		if(*iter) {
			header = *iter;
			if(ALIGN(header->size) > (ALIGN(size) + sizeof(Header))) {
				/*Split the block up*/
				Header *newheader = (Header *)(((char *) (header + 1)) + ALIGN(size));
				newheader->size = ALIGN(header->size) - (ALIGN(size) + sizeof(Header));
				header->size = ALIGN(size);
				newheader->next = header->next;
				*iter = newheader;
			} else
				*iter = header->next;
			
			return header + 1;
		} else {
			/*No free, large enough blocks availble, get more from system*/
			if((header = getmore(size))) {
				#if STRATEGY == 4
				if(list < NRQUICKLISTS) {
					/*Split quicklist-blocks into smaller pieces at once*/
					int sz = header->size + sizeof(Header);
					tmp = &header;
					for(i = 0; i < sz/((8 << list) + sizeof(Header)); i++) {
						(*tmp)->size = (8 << list);
						(*tmp)->next = (Header *) (((char *) *tmp) + (8 << list) + sizeof(Header));
						tmp = &(*tmp)->next;
					}
					*tmp = freelist[list];
					freelist[list] = header;
					continue;
				}
				#endif
				free(header + 1);
			} else
				return NULL;
		}
	}
}


/**
 * Change the size of an allocated memory block.
 * Can both grow and shrink. Will allocate a new block,
 * move the data and free the old block.
 * 
 * @param ptr Pointer to a previously allocated block of memory
 * @param size New requested size of the block
 * @return Pointer to the new memory block, or NULL on failure
 */
void *realloc(void *ptr, size_t size) {
	void *tmp;
	Header *header;
	
	if(!ptr)
		return malloc(size);
	
	if(!size) {
		free(ptr);
		return NULL;
	}
	/*Should directly split block when shrinking without malloc+copy, but too lazy*/
	tmp = malloc(size);
	header = ((Header *) ptr) - 1;
	memcpy(tmp, ptr, MIN(header->size, size));
	free(ptr);
	return tmp;
}


/**
 * Free allocated memory
 * 
 * @param ptr Pointer to a previously allocated block of memory
 */
void free(void *ptr) {
	Header **iter, *header;
	bool hasmerged = false;
	#if STRATEGY == 4
	int list;
	#endif
	if(!ptr)
		return;
	
	header = ((Header*) ptr) - 1;
	#if STRATEGY == 4
	for(list = 0; list < NRQUICKLISTS && header->size > (8 << list); list++);
	if(list < NRQUICKLISTS) {
		/*Quicklist blocks are not merged*/
		header->next = freelist[list];
		freelist[list] = header;
		return;
	}
	#endif
	
	redo:
	for(iter = &freelist[NRQUICKLISTS]; *iter; iter = &((*iter)->next)) {
		if(((char *) *iter) + (*iter)->size + sizeof(Header) == (void *) header) {
			/*Merge lower bound*/
			(*iter)->size += sizeof(Header) + header->size;
			header = *iter;
			*iter = (*iter)->next;
			goto redo;
		} else if(((char *) header) + sizeof(Header) + header->size == (void *) *iter) {
			/*Merge upper bound*/
			header->size += sizeof(Header) + (*iter)->size;
			header->next = (*iter)->next;
			*iter = header;
			hasmerged = true;
		}
	}
	if(!hasmerged) {
		header->next = freelist[NRQUICKLISTS];
		freelist[NRQUICKLISTS] = header;
	}
}
