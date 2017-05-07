#include <stdint.h>
#include <draw/screen.h>
#include <draw/color.h>
#include <draw/pixel.h>

DrawColor *draw_framebuffer = (void *) 0x00080000UL;

void draw_pixel(int x, int y) {
	draw_framebuffer[y * DRAW_SCREEN_W + x] = draw_color;
}
