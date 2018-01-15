/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#include <muil/muil.h>

MuilWidget *muil_widget_create_hbox() {
	MuilWidget *widget;
	if((widget = malloc(sizeof(MuilWidget))) == NULL)
		return NULL;
	if((widget->properties = malloc(sizeof(struct MuilHboxProperties))) == NULL) {
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
	widget->event_handler->add(widget,muil_hbox_event_notify_children, MUIL_EVENT_TYPE_MOUSE | MUIL_EVENT_TYPE_UI);

	struct MuilHboxProperties *p = widget->properties;
	p->size = 0;
	p->children = NULL;

	widget->destroy =muil_widget_hbox_destroy;
	widget->set_prop =muil_hbox_set_prop;
	widget->get_prop =muil_hbox_get_prop;
	widget->resize =muil_hbox_resize;
	widget->request_size =muil_hbox_request_size;
	widget->render =muil_hbox_render;
	widget->x = widget->y = widget->w = widget->h = 0;
	widget->enabled = 1;

	return widget;
}

void *muil_widget_hbox_destroy(MuilWidget* widget) {
	struct MuilVboxProperties *p = widget->properties;
	struct MuilWidgetList *c, *next;
	for(c = p->children; c; c = next) {
		next = c->next;
		free(c);
	}
	return muil_widget_destroy(widget);
}

void muil_hbox_event_notify_children(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	struct MuilHboxProperties *p = widget->properties;
	MuilWidgetList *c;
	for(c = p->children; c; c = c->next)
		if(c->widget->event_handler) {
			if(PINR(e->mouse->x, e->mouse->y, c->widget->x, c->widget->y, c->widget->w, c->widget->h)) {
				if((type & (MUIL_EVENT_TYPE_MOUSE_BUTTON | MUIL_EVENT_TYPE_MOUSE_SCROLL)))
					c->widget->event_handler->send(c->widget, type, e);
				else if(!PINR(muil_e_m_prev.x, muil_e_m_prev.y, c->widget->x, c->widget->y, c->widget->w, c->widget->h))
					c->widget->event_handler->send(c->widget, MUIL_EVENT_TYPE_MOUSE_ENTER, e);
			} else if((type == MUIL_EVENT_TYPE_MOUSE_LEAVE || type == MUIL_EVENT_TYPE_UI_EVENT) && PINR(muil_e_m_prev.x,muil_e_m_prev.y, c->widget->x, c->widget->y, c->widget->w, c->widget->h))
				c->widget->event_handler->send(c->widget, MUIL_EVENT_TYPE_MOUSE_LEAVE, e);
			if(type == MUIL_EVENT_TYPE_UI_EVENT)
				c->widget->event_handler->send(c->widget, MUIL_EVENT_TYPE_UI_EVENT, e);
		}
}

void muil_hbox_add_child(MuilWidget *widget, MuilWidget *child, int expand) {
	struct MuilHboxProperties *p = widget->properties;
	MuilWidgetList **c;
	for(c = &(p->children); *c; c = &((*c)->next));
	if((*c = malloc(sizeof(MuilWidgetList))) == NULL)
		return;
	(*c)->next = NULL;
	(*c)->widget = child;
	(*c)->expand = expand;
	p->size++;
	widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
}

void muil_hbox_remove_child(MuilWidget *widget, MuilWidget *child) {
	struct MuilHboxProperties *p = widget->properties;
	MuilWidgetList **c, *c_next;
	for(c = &(p->children); *c; c = &((*c)->next)) {
		if((*c)->widget == child) {
			c_next = (*c)->next;
			free(*c);
			*c = c_next;
			p->size--;
			break;
		}
	}
	widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
}

void muil_hbox_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value) {
	//struct MuilHboxProperties *p=widget->properties;
	switch(prop) {
		case MUIL_HBOX_PROP_CHILDREN:
			break;
		case MUIL_HBOX_PROP_SIZE:
			break;
	}
}

MuilPropertyValue muil_hbox_get_prop(MuilWidget *widget, int prop) {
	struct MuilHboxProperties *p = widget->properties;
	MuilPropertyValue v = {.p = NULL};

	switch(prop) {
		case MUIL_HBOX_PROP_CHILDREN:
			v.p = p->children;
			break;
		case MUIL_HBOX_PROP_SIZE:
			v.i = p->size;
			break;
	}
	return v;
}

void muil_hbox_resize(MuilWidget *widget, int x, int y, int w, int h) {
	if(w == 0 || h == 0)
		return;
	struct MuilHboxProperties *p = widget->properties;
	widget->x = x;
	widget->y = y;
	widget->w = w;
	widget->h = h;
	if(p->size == 0)
		return;
	int i, req_w, req_h, exp_w;
	int *requested = calloc(sizeof(int), p->size);
	MuilWidgetList *c;
	for(c = p->children, i = 0; c; c = c->next) {
		if(!c->expand) {
			req_w = -1;
			req_h = h;
			c->widget->request_size(c->widget, &req_w, &req_h);
			requested[i] = req_w;
			i++;
			w -= req_w +muil_padding * 2;
		}
	}
	if(p->size - i)
		exp_w = w / (p->size - i);
	else
		exp_w = 0;
	x +=muil_padding;
	for(c = p->children, i = 0; c; c = c->next) {
		if(c->expand) {
			c->widget->resize(c->widget, x, y +muil_padding, exp_w -muil_padding, h -muil_padding * 2);
			x += exp_w;
		} else {
			c->widget->resize(c->widget, x, y +muil_padding, requested[i], h -muil_padding * 2);
			x += requested[i] +muil_padding * 2;
			i++;
		}
	}
	free(requested);
}

void muil_hbox_request_size(MuilWidget *widget, int *w, int *h) {
	int ww = 0, hh = 0;
	int req_w, req_h;
	MuilWidgetList *c;
	struct MuilHboxProperties *p = widget->properties;
	for(c = p->children; c; c = c->next) {
		if(c->expand) {
			req_w = -1;
			req_h = -1;
			c->widget->request_size(c->widget, &req_w, &req_h);
			hh = req_h > hh ? req_h : hh;
		} else {
			req_w = *w;
			req_h = -1;
			c->widget->request_size(c->widget, &req_w, &req_h);
			ww += req_w;
			hh = req_h > hh ? req_h : hh;
		}
	}
	if(w)
		*w = ww;
	if(h)
		*h = hh + muil_padding*2;
}

void muil_hbox_render(MuilWidget *widget) {
	if(widget->w == 0 || widget->h == 0)
		return;
	struct MuilHboxProperties *p = widget->properties;
	MuilWidgetList *c;
	for(c = p->children; c != NULL && c->widget != NULL; c = c->next)
		c->widget->render(c->widget);
}
