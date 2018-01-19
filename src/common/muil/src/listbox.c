/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


//#include <string.h>

#include <muil/muil.h>

MuilWidget *muil_widget_create_listbox(DrawFont *font) {
	MuilWidget *widget;
	if((widget = malloc(sizeof(MuilWidget))) == NULL)
		return NULL;
	if((widget->properties = malloc(sizeof(struct MuilListboxProperties))) == NULL) {
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
	widget->event_handler->add(widget,muil_listbox_event_mouse, MUIL_EVENT_TYPE_MOUSE);
	struct MuilListboxProperties *p = widget->properties;
	p->list = NULL;
	p->offset = NULL;
	p->font = font;
	p->background = draw_rect_set_new(1);
	p->border = draw_line_set_new(4, 1);
	p->selected_rect = draw_rect_set_new(1);
	p->scrollbar = draw_rect_set_new(1);
	p->size = 0;
	p->scroll = 0;
	p->scroll_max = 0;
	p->selected = -1;

	widget->destroy =muil_widget_destroy_listbox;
	widget->set_prop =muil_listbox_set_prop;
	widget->get_prop =muil_listbox_get_prop;
	widget->resize =muil_listbox_resize;
	widget->request_size =muil_listbox_request_size;
	widget->render =muil_listbox_render;
	widget->x = widget->y = widget->w = widget->h = 0;
	widget->enabled = 1;
	widget->needs_redraw = true;

	return widget;
}

void *muil_widget_destroy_listbox(MuilWidget *widget) {
	struct MuilListboxProperties *p = widget->properties;
	muil_listbox_clear(widget);
	draw_rect_set_free(p->background);
	draw_line_set_free(p->border);
	draw_rect_set_free(p->selected_rect);
	draw_rect_set_free(p->scrollbar);
	return muil_widget_destroy(widget);
}

void muil_listbox_add(MuilWidget *widget, const char *text) {
	struct MuilListboxProperties *p = widget->properties;
	struct MuilListboxList **l;
	for(l = &(p->list); *l; l = &((*l)->next));
	if((*l = malloc(sizeof(struct MuilListboxList))) == NULL)
		return;
	(*l)->next = NULL;
	(*l)->surface = NULL;
	strncpy((*l)->text, text, 127);
	(*l)->text[127] = 0;
	p->size++;
	muil_listbox_scroll(widget, p->scroll);
	widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
}

void muil_listbox_clear(MuilWidget *widget) {
	struct MuilListboxProperties *p = widget->properties;
	struct MuilListboxList *l, *next;
	for(l = p->list; l; l = next) {
		next = l->next;
		draw_text_surface_free(l->surface);
		free(l);
	}
	p->list = NULL;
	p->offset = NULL;
	p->size = 0;
	p->scroll = 0;
	p->scroll_max = 0;
	p->selected = -1;
	widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
}

char *muil_listbox_get(MuilWidget *widget, int index) {
	struct MuilListboxProperties *p = widget->properties;
	struct MuilListboxList *l;
	int i;
	for(l = p->list, i = 0; l->next && i < index; l = l->next, i++);
	return l->text;
}

void muil_listbox_set(MuilWidget *widget, int index, const char *text) {
	struct MuilListboxProperties *p = widget->properties;
	struct MuilListboxList *l;
	int i;
	for(l = p->list, i = 0; l->next && i < index; l = l->next, i++);
	strcpy(l->text, text);
	widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
}

int muil_listbox_index_of(MuilWidget *widget, const char *text) {
	struct MuilListboxProperties *p = widget->properties;
	struct MuilListboxList *l;
	int i;
	for(l = p->list, i = 0; l; l = l->next, i++)
		if(!strcmp(text, l->text))
			return i;
	return -1;
}

void muil_listbox_scroll(MuilWidget *widget, int pos) {
	struct MuilListboxProperties *p = widget->properties;
	int text_h;
	struct MuilListboxList *l, *ll;
	int i = 0;
	
	if(!(p->size && widget->w > 0 && widget->h > 0))
		return;
	
	int oldscroll = p->scroll;
	
	for(l = p->list; l; l = l->next) {
		text_h = 2;
		for(ll = l; ll; ll = ll->next) {
			int this_h;
			draw_font_string_geometrics(p->font, ll->text, widget->w -muil_padding * 5, NULL, &this_h);
			text_h += this_h;
		}
		if((pos >= 0 && i >= pos))
			break;
		if(text_h < widget->h - 2)
			break;
		i++;
	}
	p->offset = l;
	p->scroll = i;

	pos = -1;
	for(; l; l = l->next) {
		text_h = 2;
		for(ll = l; ll; ll = ll->next) {
			int this_h;
			draw_font_string_geometrics(p->font, ll->text, widget->w -muil_padding * 5, NULL, &this_h);
			text_h += this_h;
		}
		if(text_h < widget->h - 2)
			break;
		i++;
	}
	p->scroll_max = i;
	if(p->scroll != oldscroll)
		widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
}

void muil_listbox_event_mouse(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	struct MuilListboxProperties *p = widget->properties;
	static int scroll_y = -1;
	if(!widget->enabled)
		return;
	if(type == MUIL_EVENT_TYPE_MOUSE_SCROLL) {
		p->scroll += e->mouse->wheel;
		p->scroll = p->scroll < 0 ? 0 : p->scroll;
		muil_listbox_scroll(widget, p->scroll);
	} else if(e->mouse->x >= widget->x + widget->w -muil_padding * 3 && e->mouse->x < widget->x + widget->w -muil_padding) {
		if(type == MUIL_EVENT_TYPE_MOUSE_PRESS) {
			int sb_y1, sb_y2;
			draw_rect_set_get(p->scrollbar, 0, NULL, &sb_y1, NULL, &sb_y2);
			if(e->mouse->y < sb_y1) {
				p->scroll--;
				p->scroll = p->scroll < 0 ? 0 : p->scroll;
				muil_listbox_scroll(widget, p->scroll - 1);
				scroll_y = -1;
			} else if(e->mouse->y >= sb_y2) {
				p->scroll++;
				muil_listbox_scroll(widget, p->scroll);
				scroll_y = -1;
			} else
				scroll_y = e->mouse->y - sb_y1;

		} else if(type == MUIL_EVENT_TYPE_MOUSE_DOWN && e->mouse->buttons & MUIL_EVENT_MOUSE_BUTTON_LEFT && scroll_y > -1) {
			int sb_y1, sb_y2;
			draw_rect_set_get(p->scrollbar, 0, NULL, &sb_y1, NULL, &sb_y2);
			int delta = e->mouse->y - (widget->y +muil_padding / 2) - scroll_y;
			int scroll;
			if((widget->h -muil_padding - (sb_y2 - sb_y1)) == 0)
				scroll = 0;
			else
				scroll = p->scroll_max * (delta < 0 ? 0 : delta) / (widget->h -muil_padding - (sb_y2 - sb_y1));
			muil_listbox_scroll(widget, scroll < 0 ? 0 : scroll);
			return;
		}
	} else if(type == MUIL_EVENT_TYPE_MOUSE_PRESS && e->mouse->buttons & MUIL_EVENT_MOUSE_BUTTON_LEFT) {
		struct MuilListboxList *l;
		int i = p->scroll;
		int item_y = widget->y + 2;
		for(l = p->offset; l; l = l->next, i++) {
			int item_h;
			draw_font_string_geometrics(p->font, l->text, widget->w -muil_padding * 5, NULL, &item_h);
			if(e->mouse->y > item_y && e->mouse->y < item_y + item_h) {
				p->selected = i;
				widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
				widget->event_handler->send(widget, MUIL_EVENT_TYPE_UI_WIDGET_ACTIVATE, NULL);
				break;
			}
			item_y += item_h;
		}
	}
}

void muil_listbox_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value) {
	struct MuilListboxProperties *p = widget->properties;
	switch(prop) {
		case MUIL_LISTBOX_PROP_FONT:
			p->font = value.p;
			break;
		case MUIL_LISTBOX_PROP_SELECTED:
			if(value.i >= -1 && value.i < p->size) {
				p->selected = value.i;
				widget->resize(widget, widget->x, widget->y, widget->w, widget->h);
			}
			break;
	}
}

MuilPropertyValue muil_listbox_get_prop(MuilWidget *widget, int prop) {
	struct MuilListboxProperties *p = widget->properties;
	MuilPropertyValue v = {.p = NULL};
	switch(prop) {
		case MUIL_LISTBOX_PROP_FONT:
			v.p = p->font;
			break;
		case MUIL_LISTBOX_PROP_SIZE:
			v.i = p->size;
			break;
		case MUIL_LISTBOX_PROP_SCROLL:
			v.i = p->scroll;
			break;
		case MUIL_LISTBOX_PROP_SELECTED:
			v.i = p->selected;
			break;
	}
	return v;
}

void muil_listbox_resize(MuilWidget *widget, int x, int y, int w, int h) {
	if(h == 0 || h == 0)
		return;
	struct MuilListboxProperties *p = widget->properties;
	widget->x = x;
	widget->y = y;
	widget->w = w;
	widget->h = h;
	widget->needs_redraw = true;
	
	draw_rect_set_move(p->background, 0, x, y, x + w, y + h);
	
	draw_line_set_move(p->border, 0, x, y, x + w, y);
	draw_line_set_move(p->border, 1, x, y + h, x + w, y + h);
	draw_line_set_move(p->border, 2, x, y, x, y + h);
	draw_line_set_move(p->border, 3, x + w, y, x + w, y + h);

	if(!p->offset)
		p->offset = p->list;

	struct MuilListboxList *l = p->offset;
	int item_y = y + 2;
	int i = p->scroll;
	while(l) {
		draw_text_surface_free(l->surface);
		l->surface = NULL;
		int item_h;
		draw_font_string_geometrics(p->font, l->text, w -muil_padding * 5, NULL, &item_h);
		if(item_y + item_h > y + h - 2)
			break;
		l->surface = draw_text_surface_new(p->font, 128, w -muil_padding * 4, x +muil_padding, item_y);
		draw_text_surface_string_append(l->surface, l->text);
		if(i == p->selected)
			draw_rect_set_move(p->selected_rect, 0, x +muil_padding, item_y, x + w -muil_padding * 3, item_y + item_h);
		item_y += item_h;
		l = l->next;
		i++;
	}
	int scroll = p->size ? (widget->h -muil_padding) * p->scroll / p->size : 0;
	int scroll_left = p->size ? (widget->h -muil_padding) * (p->scroll_max) / p->size : 0;
	draw_rect_set_move(p->scrollbar, 0, widget->x + widget->w - (muil_padding * 2 +muil_padding / 2), widget->y +muil_padding / 2 + scroll, widget->x + widget->w -muil_padding, widget->y + widget->h -muil_padding / 2 - scroll_left + scroll);
}

void muil_listbox_request_size(MuilWidget *widget, int *w, int *h) {
	if(w)
		*w = 128;
	if(h)
		*h = 64;
}

void muil_listbox_render(MuilWidget *widget) {
	if(widget->needs_redraw) {
		struct MuilListboxProperties *p = widget->properties;
		
		draw_set_color(muil_color.widget_background);
		draw_rect_set_draw(p->background, 1);
		
		draw_set_color(muil_color.widget_border);
		draw_line_set_draw(p->border, 4);
		if(!p->offset)
			return;
		
		struct MuilListboxList *l;
		int i = p->scroll;
		for(l = p->offset; l; l = l->next, i++) {
			if(!l->surface)
				break;
			draw_set_color(muil_color.text);
			draw_text_surface_draw(l->surface);
		}
		
		if(p->selected >= p->scroll && p->selected < i) {
			//d_render_logic_op(DARNIT_RENDER_LOGIC_OP_XOR);
			draw_set_color(muil_color.selected);
			draw_rect_set_draw(p->selected_rect, 1);
			//d_render_logic_op(DARNIT_RENDER_LOGIC_OP_NONE);
		}
		
		draw_set_color(muil_color.widget_border);
		draw_rect_set_draw(p->scrollbar, 1);
		
		widget->needs_redraw = false;
	}
}
