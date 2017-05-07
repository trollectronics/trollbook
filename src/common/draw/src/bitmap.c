#include <stddef.h>
#include <mem.h>
#include <draw/bitmap.h>
#include <draw/color.h>
#include <draw/screen.h>

DrawBitmap *draw_bitmap_new(int w, int h, DrawColor *data) {
	DrawBitmap *bitmap = NULL;
	
	if(!(bitmap = malloc(sizeof(DrawBitmap))))
		goto fail;
	
	bitmap->data = data;
	bitmap->x = 0;
	bitmap->y = 0;
	bitmap->w = w;
	bitmap->h = h;
	bitmap->free_data = false;
	
	return bitmap;
	
	fail:
	return NULL;
}


DrawBitmap *draw_bitmap_new_raw(int w, int h) {
	DrawColor *data = NULL;
	DrawBitmap *bitmap = NULL;
	
	if(!(data = malloc(sizeof(DrawColor)*w*h)))
		goto fail;
	
	if(!(bitmap = draw_bitmap_new(w, h, data)))
		goto fail;
	
	bitmap->free_data = true;
	
	return bitmap;
	
	fail:
	free(data);
	draw_bitmap_free(bitmap);
	return NULL;
}

void draw_bitmap_free(DrawBitmap *bitmap) {
	if(!bitmap)
		return;
	
	if(bitmap->free_data)
		free(bitmap->data);
	
	free(bitmap);
}

void draw_bitmap_move(DrawBitmap *bitmap, int x, int y) {
	if(!bitmap)
		return;
	bitmap->x = x;
	bitmap->y = y;
}

void draw_bitmap_draw(DrawBitmap *bitmap) {
	int x, y;
	
	if(!bitmap)
		return;
	
	for (y = 0; y < bitmap->h; y++) {
		for (x = 0; x < bitmap->w; x++) {
			draw_framebuffer[(bitmap->y + y)*DRAW_SCREEN_W + bitmap->x + x] = bitmap->data[bitmap->w*y + x];
		}
	}
}
