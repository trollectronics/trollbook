/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_LABEL_H
#define MUIL_LABEL_H

#define MUIL_LABEL_PROP_FONT 0
#define MUIL_LABEL_PROP_SURFACE 1
#define MUIL_LABEL_PROP_TEXT 2

struct MuilLabelProperties {
	DrawRectSet *background;
	DrawFont *font;
	DrawTextSurface *surface;
	char *text;
};

MuilWidget *muil_widget_create_label(DrawFont *font, const char *text);
void *muil_widget_destroy_label(MuilWidget *widget);

void muil_label_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value);
MuilPropertyValue muil_label_get_prop(MuilWidget *widget, int prop);
void muil_label_resize(MuilWidget *widget, int x, int y, int w, int h);
void muil_label_request_size(MuilWidget *widget, int *w, int *h);
void muil_label_render(MuilWidget *widget);

#endif
