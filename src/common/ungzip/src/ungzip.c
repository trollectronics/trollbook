#define	INT_MAX 0x7FFFFFF

#define	STBI_ONLY_PNG
#define	STBI_SUPPORT_ZLIB
#define STBI_NO_PNG
#define	STBI_NO_STDIO
#define STBI_ASSERT
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_LINEAR

#include <mem.h>
#include "stb_image.h"


void *ungzip(void *indata, int inlen, int *outlen) {
	return stbi_zlib_decode_noheader_malloc(indata, inlen, outlen);
}
