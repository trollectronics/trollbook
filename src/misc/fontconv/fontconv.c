#include		<stdlib.h>
#include		<stdio.h>

#include		<SDL/SDL.h>
#include		<SDL/SDL_image.h>

int main(int argc, char **argv) {
	FILE *out;
	int i, j, k, glyphs, t, w;
	uint32_t data;
	uint8_t *ptr;
	SDL_Surface *tex;
	
	int glyph_w, glyph_h;
	
	if (argc != 5) {
		fprintf(stderr, "Invalid amount of arguments!\n");
		printf("Usage: font_convert <image> <destination> <glyph width> <glyph height>\n");
		return -1;
	}
	
	glyph_w = atoi(argv[3]);
	glyph_h = atoi(argv[4]);
	
	if ((out = fopen(argv[2], "w")) == NULL) {
		fprintf(stderr, "Failed to open file %s for writing+trunc!\n", argv[2]);
		return -1;
	}
	
	if ((tex = IMG_Load(argv[1])) == NULL) {
		printf("Failed to load %s\n", argv[1]);
		return 0;
	}

	fprintf(stderr, "%i\n", tex->format->BytesPerPixel);

	glyphs = tex->w / glyph_w;
	ptr = tex->pixels;
	w = tex->w + (tex->w % 4);
	for (i = 0; i < glyphs; i++) {
		for (j = 0; j < glyph_h; j++) {
			data = 0;
			for (k = 0; k < glyph_w; k++) {
				t = i * glyph_w + j * w + k;
				if(ptr[t])
					data |= (1 << k);
			//	fprintf(stderr, "%i,%i,%i;; %i, %i, %i\n", i, j, k, t, ptr[t], data);
			}
			fwrite(&data, 1, 1, out);
		}
	}

	fclose(out);
	
	return 0;
}
