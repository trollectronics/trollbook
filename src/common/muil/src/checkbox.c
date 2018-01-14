/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#include <muil/muil.h>

MuilWidget *muil_widget_create_checkbox() {
	MuilWidget *widget;
	if((widget = malloc(sizeof(MuilWidget))) == NULL)
		return NULL;
	if((widget->properties = malloc(sizeof(struct MuilCheckboxProperties))) == NULL) {
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
	widget->event_handler->add(widget,muil_checkbox_event_click, MUIL_EVENT_TYPE_MOUSE_PRESS);
	widget->event_handler->add(widget,muil_checkbox_event_key, MUIL_EVENT_TYPE_KEYBOARD);

	struct MuilCheckboxProperties *p = widget->properties;
	p->activated = 0;
	p->background = draw_rect_set_new(1);
	p->border = draw_line_set_new(4, 1);
	p->active_border = draw_line_set_new(2, 1);
	widget->destroy =muil_widget_destroy_checkbox;
	widget->set_prop =muil_checkbox_set_prop;
	widget->get_prop =muil_checkbox_get_prop;
	widget->resize =muil_checkbox_resize;
	widget->request_size =muil_checkbox_request_size;
	widget->render =muil_checkbox_render;

	widget->x = widget->y = widget->w = widget->h = 0;
	widget->enabled = 1;
	widget->needs_redraw = true;

	return widget;
}

void *muil_widget_destroy_checkbox(MuilWidget *widget) {
	struct MuilCheckboxProperties *p = widget->properties;
	draw_rect_set_free(p->background);
	draw_line_set_free(p->border);
	draw_line_set_free(p->active_border);
	return muil_widget_destroy(widget);
}

void muil_checkbox_event_key(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	if(!widget->enabled)
		return;
	struct MuilCheckboxProperties *p = widget->properties;
	if(e->keyboard->keysym != KEY(RETURN) && e->keyboard->keysym != KEY(KP_ENTER) && e->keyboard->keysym != KEY(SPACE))
		return;
	switch(type) {
		case MUIL_EVENT_TYPE_KEYBOARD_PRESS:
			p->activated = !p->activated;
			MuilEvent ee;
			MuilEventUI e_u = {};
			ee.ui = &e_u;
			widget->event_handler->send(widget, MUIL_EVENT_TYPE_UI_WIDGET_ACTIVATE, &ee);
			break;
		case MUIL_EVENT_TYPE_KEYBOARD_RELEASE:
			break;
	}
}

void muil_checkbox_event_click(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	if(!widget->enabled)
		return;
	struct MuilCheckboxProperties *p = widget->properties;
	switch(type) {
		case MUIL_EVENT_TYPE_MOUSE_PRESS:
			if(e->mouse->buttons == MUIL_EVENT_MOUSE_BUTTON_LEFT) {
				muil_selected_widget = widget;
				p->activated = !p->activated;
				MuilEvent ee;
				MuilEventUI e_u = {};
				ee.ui = &e_u;
				widget->event_handler->send(widget, MUIL_EVENT_TYPE_UI_WIDGET_ACTIVATE, &ee);
			}
			break;
			/*case MUIL_EVENT_TYPE_MOUSE_RELEASE:
				if(((e->mouse->buttons)&MUIL_EVENT_MOUSE_BUTTON_LEFT)==MUIL_EVENT_MOUSE_BUTTON_LEFT)
					break;
			case MUIL_EVENT_TYPE_MOUSE_LEAVE:
				p->activated=0;
				break;*/
	}
}

void muil_checkbox_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value) {
	struct MuilCheckboxProperties *p = widget->properties;
	switch(prop) {
		case MUIL_CHECKBOX_PROP_ACTIVATED:
			p->activated = value.i;
			break;
		case MUIL_CHECKBOX_PROP_BORDER:
			break;
		case MUIL_CHECKBOX_PROP_ACTIVE_BORDER:
			break;
	}
}

MuilPropertyValue muil_checkbox_get_prop(MuilWidget *widget, int prop) {
	struct MuilCheckboxProperties *p = widget->properties;
	MuilPropertyValue v = {.p = NULL};
	switch(prop) {
		case MUIL_CHECKBOX_PROP_ACTIVATED:
			v.i = p->activated;
			break;
		case MUIL_CHECKBOX_PROP_BORDER:
			v.p = p->border;
			break;
		case MUIL_CHECKBOX_PROP_ACTIVE_BORDER:
			v.p = p->active_border;
			break;
	}
	return v;
}

void muil_checkbox_resize(MuilWidget *widget, int x, int y, int w, int h) {
	struct MuilCheckboxProperties *p = widget->properties;
	widget->x = x;
	widget->y = y;
	widget->w = w;
	widget->h = h;
	widget->needs_redraw = true;
	
	draw_rect_set_move(p->background, 0, x, y, x + w, y + h);
	
	draw_line_set_move(p->border, 0, x + w / 2 - 4, y + h / 2 - 4, x + w / 2 + 4, y + h / 2 - 4);
	draw_line_set_move(p->border, 1, x + w / 2 - 4, y + h / 2 + 4, x + w / 2 + 4, y + h / 2 + 4);
	draw_line_set_move(p->border, 2, x + w / 2 - 4, y + h / 2 - 4, x + w / 2 - 4, y + h / 2 + 4);
	draw_line_set_move(p->border, 3, x + w / 2 + 4, y + h / 2 - 4, x + w / 2 + 4, y + h / 2 + 4);

	draw_line_set_move(p->active_border, 0, x + w / 2 - 4, y + h / 2 - 4, x + w / 2 + 4, y + h / 2 + 4);
	draw_line_set_move(p->active_border, 1, x + w / 2 + 4, y + h / 2 - 4, x + w / 2 - 4, y + h / 2 + 4);
}

void muil_checkbox_request_size(MuilWidget *widget, int *w, int *h) {
	if(w)
		(*w) = 8;
	if(h)
		(*h) = 8;
}

void muil_checkbox_render(MuilWidget *widget) {
	if(widget->needs_redraw) {
		struct MuilCheckboxProperties *p = widget->properties;
		
		draw_set_color(muil_color.widget_background);
		draw_rect_set_draw(p->background, 1);
		
		draw_set_color(muil_color.widget_border);
		draw_line_set_draw(p->border, 4);
		if(p->activated)
			draw_line_set_draw(p->active_border, 2);
		
		widget->needs_redraw = false;
	}
}
