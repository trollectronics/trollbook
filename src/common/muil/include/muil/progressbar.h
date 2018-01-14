/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_PROGRESSBAR_H
#define MUIL_PROGRESSBAR_H

#define MUIL_PROGRESSBAR_PROP_FONT 0
#define MUIL_PROGRESSBAR_PROP_SURFACE 1
#define MUIL_PROGRESSBAR_PROP_BORDER 2
#define MUIL_PROGRESSBAR_PROP_BAR 3
#define MUIL_PROGRESSBAR_PROP_PROGRESS 4
#define MUIL_PROGRESSBAR_PROP_TEXT 5

struct MuilProgressbarProperties {
	DrawFont *font;
	DrawTextSurface *surface;
	DrawRectSet *background;
	DrawLineSet *border;
	DrawRectSet *bar;
	int progress;
	char text[5];
};

MuilWidget *muil_widget_create_progressbar(DrawFont *font);
void *muil_widget_destroy_progressbar(MuilWidget *widget);

void muil_progressbar_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value);
MuilPropertyValue muil_progressbar_get_prop(MuilWidget *widget, int prop);
void muil_progressbar_resize(MuilWidget *widget, int x, int y, int w, int h);
void muil_progressbar_request_size(MuilWidget *widget, int *w, int *h);
void muil_progressbar_render(MuilWidget *widget);

#endif
