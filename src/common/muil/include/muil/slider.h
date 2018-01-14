/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_SLIDER_H
#define MUIL_SLIDER_H

#define MUIL_SLIDER_PROP_LINE 1
#define MUIL_SLIDER_PROP_HANDLE 2
#define MUIL_SLIDER_PROP_VALUE 3
#define MUIL_SLIDER_PROP_STEPS 4

struct MuilSliderProperties {
	DrawRectSet *background;
	DrawLineSet *line;
	DrawRectSet *handle;
	unsigned int value;
	unsigned int steps;
};

MuilWidget *muil_widget_create_slider(unsigned int steps);
void *muil_widget_destroy_slider(MuilWidget *widget);

void muil_slider_event_mouse_down(MuilWidget *widget, unsigned int type, MuilEvent *e);
void muil_slider_event_mouse_release(MuilWidget *widget, unsigned int type, MuilEvent *e);

void muil_slider_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value);
MuilPropertyValue muil_slider_get_prop(MuilWidget *widget, int prop);
void muil_slider_resize(MuilWidget *widget, int x, int y, int w, int h);
void muil_slider_request_size(MuilWidget *widget, int *w, int *h);
void muil_slider_render(MuilWidget *widget);

#endif
