/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#include <muil/muil.h>

MuilWidget *muil_widget_create_spacer() {
	MuilWidget *widget;
	if((widget = malloc(sizeof(MuilWidget))) == NULL)
		return NULL;
	if((widget->properties = malloc(sizeof(struct MuilSpacerProperties))) == NULL) {
		free(widget);
		return NULL;
	}

	if((widget->event_handler = malloc(sizeof(MuilEventHandler))) == NULL) {
		free(widget->properties);
		free(widget);
		return NULL;
	}
	widget->event_handler->handlers = NULL;
	widget->event_handler->add =muil_event_add;
	widget->event_handler->remove =muil_event_remove;
	widget->event_handler->send =muil_event_send;

	struct MuilSpacerProperties *p = widget->properties;
	p->set_w = 0;
	p->set_h = 0;

	widget->destroy =muil_widget_destroy;
	widget->set_prop =muil_spacer_set_prop;
	widget->get_prop =muil_spacer_get_prop;
	widget->resize =muil_spacer_resize;
	widget->request_size =muil_spacer_request_size;
	widget->render =muil_spacer_render;
	widget->x = widget->y = widget->w = widget->h = 0;
	widget->enabled = 1;

	return widget;
}

MuilWidget *muil_widget_create_spacer_size(int w, int h) {
	MuilWidget *widget;
	widget =muil_widget_create_spacer();
	struct MuilSpacerProperties *p = widget->properties;
	p->set_w = w;
	p->set_h = h;
	return widget;
}

MuilPropertyValue muil_spacer_get_prop(MuilWidget *widget, int prop) {
	MuilPropertyValue v = {.p = NULL};
	return v;
}

void muil_spacer_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value) {
	return;
}

void muil_spacer_request_size(MuilWidget *widget, int *w, int *h) {
	struct MuilSpacerProperties *p = widget->properties;
	if(w)
		*w = p->set_w;
	if(h)
		*h = p->set_h;
}

void muil_spacer_resize(MuilWidget *widget, int x, int y, int w, int h) {
	widget->x = x;
	widget->y = y;
	widget->w = w;
	widget->h = h;
}

void muil_spacer_render(MuilWidget *widget) {
	return;
}
