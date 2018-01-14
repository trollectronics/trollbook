/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_LISTBOX_H
#define MUIL_LISTBOX_H

#define MUIL_LISTBOX_PROP_FONT 0
#define MUIL_LISTBOX_PROP_LIST 2
#define MUIL_LISTBOX_PROP_SCROLL_OFFSET 3
#define MUIL_LISTBOX_PROP_BORDER 4
#define MUIL_LISTBOX_PROP_SIZE 5
#define MUIL_LISTBOX_PROP_SCROLL 6
#define MUIL_LISTBOX_PROP_SELECTED 7
#define MUIL_LISTBOX_PROP_SELECTED_RECT 8

struct MuilListboxProperties {
	DrawFont *font;
	struct MuilListboxList *list;
	struct MuilListboxList *offset;
	DrawRectSet *background;
	DrawLineSet *border;
	DrawRectSet *scrollbar;
	int size;
	int scroll;
	int scroll_max;
	int selected;
	DrawRectSet *selected_rect;
};

struct MuilListboxList {
	char text[128];
	DrawTextSurface *surface;
	struct MuilListboxList *next;
};

MuilWidget *muil_widget_create_listbox(DrawFont *font);
void *muil_widget_destroy_listbox(MuilWidget *widget);

void muil_listbox_event_mouse(MuilWidget *widget, unsigned int type, MuilEvent *e);

void muil_listbox_add(MuilWidget *widget, const char *text);
void muil_listbox_clear(MuilWidget *widget);
char *muil_listbox_get(MuilWidget *widget, int index);
void muil_listbox_set(MuilWidget *widget, int index, const char *text);
int muil_listbox_index_of(MuilWidget *widget, const char *text);
void muil_listbox_scroll(MuilWidget *widget, int pos);

void muil_listbox_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value);
MuilPropertyValue muil_listbox_get_prop(MuilWidget *widget, int prop);
void muil_listbox_resize(MuilWidget *widget, int x, int y, int w, int h);
void muil_listbox_request_size(MuilWidget *widget, int *w, int *h);
void muil_listbox_render(MuilWidget *widget);

#endif
