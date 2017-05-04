#ifndef DRAW_LINE_H_
#define DRAW_LINE_H_

#include <stddef.h>

typedef struct DrawLine DrawLine;
struct DrawLine {
	int x1;
	int y1;
	int x2;
	int y2;
	unsigned int thickness;
};

typedef struct DrawLineSet DrawLineSet;
struct DrawLineSet {
	DrawLine *line;
	size_t lines;
	unsigned int thickness;
};

DrawLineSet *draw_line_set_new(size_t lines, unsigned int thickness);
void draw_line_set_move(DrawLineSet *set, size_t line, int x1, int y1, int x2, int y2);
void draw_line_set_get(DrawLineSet *set, size_t line, int *x1, int *y1, int *x2, int *y2);
void draw_line_set_draw(DrawLineSet *set, size_t lines);
void draw_line_set_free(DrawLineSet *set);

#endif
