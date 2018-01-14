/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_CHECKBOX_H
#define MUIL_CHECKBOX_H

#define MUIL_CHECKBOX_PROP_ACTIVATED 0
#define MUIL_CHECKBOX_PROP_BORDER 1
#define MUIL_CHECKBOX_PROP_ACTIVE_BORDER 2

struct MuilCheckboxProperties {
	int activated;
	DrawRectSet *background;
	DrawLineSet *border;
	DrawLineSet *active_border;
};

MuilWidget *muil_widget_create_checkbox();
void *muil_widget_destroy_checkbox(MuilWidget *widget);

void muil_checkbox_event_click(MuilWidget *widget, unsigned int type, MuilEvent *e);
void muil_checkbox_event_key(MuilWidget *widget, unsigned int type, MuilEvent *e);

void muil_checkbox_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value);
MuilPropertyValue muil_checkbox_get_prop(MuilWidget *widget, int prop);
void muil_checkbox_resize(MuilWidget *widget, int x, int y, int w, int h);
void muil_checkbox_request_size(MuilWidget *widget, int *w, int *h);
void muil_checkbox_render(MuilWidget *widget);

#endif
