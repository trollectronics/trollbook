/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_BUTTON_H
#define MUIL_BUTTON_H

#define MUIL_BUTTON_PROP_CHILD 0
#define MUIL_BUTTON_PROP_ACTIVATED 1
#define MUIL_BUTTON_PROP_BORDER 2
#define MUIL_BUTTON_PROP_ACTIVE_BORDER 3

struct MuilButtonProperties {
	MuilWidget *child;
	int activated;
	DrawRectSet *background;
	DrawLineSet *border;
	DrawLineSet *active_border;
};

MuilWidget *muil_widget_create_button(MuilWidget *child);
MuilWidget *muil_widget_create_button_text(DrawFont *font, const char *text);
MuilWidget *muil_widget_create_button_image();

void *muil_widget_destroy_button(MuilWidget *widget);
void *muil_widget_destroy_button_recursive(MuilWidget *widget);

void muil_button_event_click(MuilWidget *widget, unsigned int type, MuilEvent *e);
void muil_button_event_key(MuilWidget *widget, unsigned int type, MuilEvent *e);

void muil_button_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value);
MuilPropertyValue muil_button_get_prop(MuilWidget *widget, int prop);
void muil_button_resize(MuilWidget *widget, int x, int y, int w, int h);
void muil_button_request_size(MuilWidget *widget, int *w, int *h);
void muil_button_render(MuilWidget *widget);

#endif
