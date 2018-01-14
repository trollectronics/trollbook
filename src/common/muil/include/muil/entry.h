/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_ENTRY_H
#define MUIL_ENTRY_H

#define MUIL_ENTRY_LENGTH 256

#define MUIL_ENTRY_PROP_FONT 0
#define MUIL_ENTRY_PROP_SURFACE 1
#define MUIL_ENTRY_PROP_CURSOR_POS 2
#define MUIL_ENTRY_PROP_OFFSET 3
#define MUIL_ENTRY_PROP_BORDER 4
#define MUIL_ENTRY_PROP_CURSOR 5
#define MUIL_ENTRY_PROP_TEXT 6

struct MuilEntryProperties {
	DrawFont *font;
	DrawTextSurface *surface;
	int cursor_pos;
	char *offset;
	DrawRectSet *background;
	DrawLineSet *border;
	DrawLineSet *cursor;
	char text[MUIL_ENTRY_LENGTH+1];
};

MuilWidget *muil_widget_create_entry(DrawFont *font);
void *muil_widget_destroy_entry(MuilWidget *widget);

void muil_entry_event_click(MuilWidget *widget, unsigned int type, MuilEvent *e);
void muil_entry_event_key(MuilWidget *widget, unsigned int type, MuilEvent *e);

void muil_entry_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value);
MuilPropertyValue muil_entry_get_prop(MuilWidget *widget, int prop);
void muil_entry_resize(MuilWidget *widget, int x, int y, int w, int h);
void muil_entry_request_size(MuilWidget *widget, int *w, int *h);
void muil_entry_render(MuilWidget *widget);

#endif
