/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */

#include <muil/muil.h>

MuilWidget *muil_widget_create_slider(unsigned int steps) {
	MuilWidget *widget;
	if((widget = malloc(sizeof(MuilWidget))) == NULL)
		return NULL;
	if((widget->properties = malloc(sizeof(struct MuilSliderProperties))) == NULL) {
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
	widget->event_handler->add(widget,muil_slider_event_mouse_down, MUIL_EVENT_TYPE_MOUSE_DOWN);
	widget->event_handler->add(widget,muil_slider_event_mouse_release, MUIL_EVENT_TYPE_MOUSE_RELEASE);
	widget->event_handler->add(widget,muil_slider_event_mouse_release, MUIL_EVENT_TYPE_MOUSE_LEAVE);

	struct MuilSliderProperties *p = widget->properties;
	p->background = draw_rect_set_new(1);
	p->line = draw_line_set_new(1 + steps, 1);
	p->handle = draw_rect_set_new(1);
	p->value = 0;
	p->steps = steps;

	widget->destroy =muil_widget_destroy_slider;
	widget->set_prop =muil_slider_set_prop;
	widget->get_prop =muil_slider_get_prop;
	widget->resize =muil_slider_resize;
	widget->request_size =muil_slider_request_size;
	widget->render =muil_slider_render;
	widget->x = widget->y = widget->w = widget->h = 0;
	widget->enabled = 1;
	widget->needs_redraw = true;

	return widget;
}

void *muil_widget_destroy_slider(MuilWidget *widget) {
	struct MuilSliderProperties *p = widget->properties;
	draw_rect_set_free(p->handle);
	draw_rect_set_free(p->background);
	draw_line_set_free(p->line);
	return muil_widget_destroy(widget);
}

void muil_slider_event_mouse_down(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	if(!widget->enabled)
		return;
	int i, value = 0;
	struct MuilSliderProperties *p = widget->properties;
	for(i = 0; i < p->steps; i++)
		if(e->mouse->x > widget->x + 2 + (widget->w - 4) / (p->steps - 1) / 2 + i * ((widget->w - 4) / (p->steps - 1)))
			value++;
	p->value = value;
	widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
}

void muil_slider_event_mouse_release(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	if(type == MUIL_EVENT_TYPE_MOUSE_LEAVE && !e->mouse->buttons)
		return;
	MuilEvent ee;
	MuilEventUI e_u = {};
	ee.ui = &e_u;
	widget->event_handler->send(widget, MUIL_EVENT_TYPE_UI_WIDGET_ACTIVATE, &ee);
}

void muil_slider_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value) {
	struct MuilSliderProperties *p = widget->properties;
	switch(prop) {
		case MUIL_SLIDER_PROP_VALUE:
			p->value = value.i;
			if(p->value >= p->steps)
				p->value = p->steps - 1;
			widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
			break;
		case MUIL_SLIDER_PROP_STEPS:
			p->steps = value.i;
			if(p->value >= p->steps)
				p->value = p->steps - 1;
			widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
			break;
	}
}

MuilPropertyValue muil_slider_get_prop(MuilWidget *widget, int prop) {
	MuilPropertyValue v = {.p = NULL};
	struct MuilSliderProperties *p = widget->properties;
	switch(prop) {
		case MUIL_SLIDER_PROP_VALUE:
			v.i = p->value;
			break;
		case MUIL_SLIDER_PROP_STEPS:
			v.i = p->steps;
			break;
	}
	return v;
}

void muil_slider_resize(MuilWidget *widget, int x, int y, int w, int h) {
	if(h == 0 || h == 0)
		return;
	struct MuilSliderProperties *p = widget->properties;
	widget->x = x;
	widget->y = y;
	widget->w = w;
	widget->h = h;
	widget->needs_redraw = true;
	
	draw_rect_set_move(p->background, 0, x, y, x + w, y + h);
	
	draw_line_set_move(p->line, 0, x + 2, y + h / 2, x + w - 2, y + h / 2);
	int i;
	for(i = 0; i < p->steps; i++) {
		int xx = x + 2 + i * ((w - 4) / (p->steps - 1));
		draw_line_set_move(p->line, i + 1, xx, y + h / 2 - 2, xx, y + h / 2 + 2);
	}
	int xx = x + 2 + p->value * ((w - 4) / (p->steps - 1));
	draw_rect_set_move(p->handle, 0, xx - 2, y + h / 2 - 4, xx + 2, y + h / 2 + 4);
}

void muil_slider_request_size(MuilWidget *widget, int *w, int *h) {
	if(w)
		*w = 64;
	if(h)
		*h = 8;
}

void muil_slider_render(MuilWidget *widget) {
	if(widget->needs_redraw) {
		struct MuilSliderProperties *p = widget->properties;
		
		draw_set_color(muil_color.window_background);
		draw_rect_set_draw(p->background, 1);
		
		draw_set_color(muil_color.widget_border);
		draw_line_set_draw(p->line, 1 + p->steps);
		draw_set_color(muil_color.selected);
		draw_rect_set_draw(p->handle, 1);
		
		widget->needs_redraw = false;
	}
}
