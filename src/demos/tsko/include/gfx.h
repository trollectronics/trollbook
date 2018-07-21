#ifndef GFX_H_
#define GFX_H_

void gfx_set_lowres();
void gfx_buffer_flip();
void gfx_blit(void *image, int width, int x, int y);
void gfx_blit_fast(void *image, unsigned width, unsigned height, unsigned x, unsigned y);

#endif
