/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#include <muil/muil.h>

MuilWidget *muil_widget_create_button(MuilWidget *child) {
	MuilWidget *widget;
	if((widget = malloc(sizeof(MuilWidget))) == NULL)
		return NULL;
	if((widget->properties = malloc(sizeof(struct MuilButtonProperties))) == NULL) {
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
	widget->event_handler->add(widget,muil_button_event_click, MUIL_EVENT_TYPE_MOUSE);
	widget->event_handler->add(widget,muil_button_event_key, MUIL_EVENT_TYPE_KEYBOARD);

	struct MuilButtonProperties *p = widget->properties;
	p->child = child;
	p->activated = 0;
	p->background = draw_rect_set_new(1);
	p->border = draw_line_set_new(8, 1);
	p->active_border = draw_line_set_new(4, 1);
	widget->destroy =muil_widget_destroy_button;
	widget->set_prop =muil_button_set_prop;
	widget->get_prop =muil_button_get_prop;
	widget->resize =muil_button_resize;
	widget->request_size =muil_button_request_size;
	widget->render =muil_button_render;

	widget->x = widget->y = widget->w = widget->h = 0;
	widget->enabled = 1;
	widget->needs_redraw = true;

	return widget;
}

MuilWidget *muil_widget_create_button_text(DrawFont *font, const char *text) {
	MuilWidget *widget, *label;
	label = muil_widget_create_label(font, text);
	widget = muil_widget_create_button(label);
	widget->destroy = muil_widget_destroy_button_recursive;
	return widget;
}

MuilWidget *muil_widget_create_button_image() {
	return NULL;
}

void *muil_widget_destroy_button(MuilWidget *widget) {
	struct MuilButtonProperties *p = widget->properties;
	draw_rect_set_free(p->background);
	draw_line_set_free(p->border);
	draw_line_set_free(p->active_border);
	return muil_widget_destroy(widget);
}

void *muil_widget_destroy_button_recursive(MuilWidget *widget) {
	struct MuilButtonProperties *p = widget->properties;
	p->child->destroy(p->child);
	return muil_widget_destroy_button(widget);
}

void muil_button_event_key(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	if(!widget->enabled)
		return;
	struct MuilButtonProperties *p = widget->properties;
	if(e->keyboard->keysym != KEY(RETURN) && e->keyboard->keysym != KEY(KP_ENTER) && e->keyboard->keysym != KEY(SPACE))
		return;
	switch(type) {
		case MUIL_EVENT_TYPE_KEYBOARD_PRESS:
			p->activated = 1;
			widget->needs_redraw = true;
			break;
		case MUIL_EVENT_TYPE_KEYBOARD_RELEASE:
			p->activated = 0;
			widget->needs_redraw = true;
			break;
	}
}

void muil_button_event_click(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	if(!widget->enabled)
		return;
	struct MuilButtonProperties *p = widget->properties;
	switch(type) {
		case MUIL_EVENT_TYPE_MOUSE_PRESS:
			if(e->mouse->buttons == MUIL_EVENT_MOUSE_BUTTON_LEFT)
				muil_selected_widget = widget;
			p->activated = 1;
			widget->needs_redraw = true;
			break;
		case MUIL_EVENT_TYPE_MOUSE_RELEASE:
			if(((e->mouse->buttons)&MUIL_EVENT_MOUSE_BUTTON_LEFT) == MUIL_EVENT_MOUSE_BUTTON_LEFT || !p->activated)
				break;
			MuilEvent ee;
			/*MuilEventUI e_u={};
			ee.ui=&e_u;*/
			ee.mouse = &muil_e_m_prev;
			widget->event_handler->send(widget, MUIL_EVENT_TYPE_UI_WIDGET_ACTIVATE, &ee);
		case MUIL_EVENT_TYPE_MOUSE_LEAVE:
			p->activated = 0;
			widget->needs_redraw = true;
			break;
	}
}

void muil_button_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value) {
	struct MuilButtonProperties *p = widget->properties;
	MuilWidget *old_child;
	switch(prop) {
		case MUIL_BUTTON_PROP_CHILD:
			old_child = p->child;
			p->child = value.p;
			p->child->resize(p->child, old_child->x, old_child->y, old_child->w, old_child->h);
			if(widget->destroy ==muil_widget_destroy_button_recursive) {
				old_child->destroy(old_child);
				widget->destroy =muil_widget_destroy_button;
			}
			break;
		case MUIL_BUTTON_PROP_ACTIVATED:
			p->activated = value.i;
			widget->needs_redraw = true;
			break;
		case MUIL_BUTTON_PROP_BORDER:
			break;
		case MUIL_BUTTON_PROP_ACTIVE_BORDER:
			break;
	}
}

MuilPropertyValue muil_button_get_prop(MuilWidget *widget, int prop) {
	struct MuilButtonProperties *p = widget->properties;
	MuilPropertyValue v = {.p = NULL};
	switch(prop) {
		case MUIL_BUTTON_PROP_CHILD:
			v.p = p->child;
			break;
		case MUIL_BUTTON_PROP_ACTIVATED:
			v.i = p->activated;
			break;
		case MUIL_BUTTON_PROP_BORDER:
			v.p = p->border;
			break;
		case MUIL_BUTTON_PROP_ACTIVE_BORDER:
			v.p = p->active_border;
			break;
	}
	return v;
}

void muil_button_resize(MuilWidget *widget, int x, int y, int w, int h) {
	struct MuilButtonProperties *p = widget->properties;
	widget->x = x;
	widget->y = y;
	widget->w = w;
	widget->h = h;
	widget->needs_redraw = true;
	
	p->child->resize(p->child, x + 2 +muil_padding, y + 2 +muil_padding, w - 4 -muil_padding * 2, h - 4 -muil_padding * 2);
	
	draw_rect_set_move(p->background, 0, x, y, x + w, y + h);
	
	draw_line_set_move(p->border, 0, x, y, x + w - 1, y);
	draw_line_set_move(p->border, 1, x, y + h, x + w - 1, y + h);
	draw_line_set_move(p->border, 2, x, y, x, y + h - 1);
	draw_line_set_move(p->border, 3, x + w, y, x + w, y + h - 1);
	draw_line_set_move(p->border, 4, x + 2, y + 2, x + w - 3, y + 2);
	draw_line_set_move(p->border, 5, x + 2, y + h - 2, x + w - 3, y + h - 2);
	draw_line_set_move(p->border, 6, x + 2, y + 2, x + 2, y + h - 3);
	draw_line_set_move(p->border, 7, x + w - 2, y + 2, x + w - 2, y + h - 3);

	draw_line_set_move(p->active_border, 0, x, y + 1, x + w - 1, y + 1);
	draw_line_set_move(p->active_border, 1, x, y + h - 1, x + w - 1, y + h - 1);
	draw_line_set_move(p->active_border, 2, x + 1, y, x + 1, y + h - 1);
	draw_line_set_move(p->active_border, 3, x + w - 1, y, x + w - 1, y + h - 1);
}

void muil_button_request_size(MuilWidget *widget, int *w, int *h) {
	struct MuilButtonProperties *p = widget->properties;
	p->child->request_size(p->child, w, h);
	(*w) += 8 +muil_padding * 2;
	(*h) += 8 +muil_padding * 2;
}

void muil_button_render(MuilWidget *widget) {
	if(widget->needs_redraw) {
		struct MuilButtonProperties *p = widget->properties;
		
		draw_set_color(muil_color.window_background);
		draw_rect_set_draw(p->background, 1);
		
		p->child->needs_redraw = true;
		p->child->render(p->child);
		draw_set_color(muil_color.widget_border);
		draw_line_set_draw(p->border, 8);
		if(p->activated)
			draw_line_set_draw(p->active_border, 4);
		
		widget->needs_redraw = false;
	}
}
