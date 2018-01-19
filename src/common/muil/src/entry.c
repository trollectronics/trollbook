/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


//#include <string.h>

#include <muil/muil.h>

MuilWidget *muil_widget_create_entry(DrawFont *font) {
	MuilWidget *widget;
	if((widget = malloc(sizeof(MuilWidget))) == NULL)
		return NULL;
	if((widget->properties = malloc(sizeof(struct MuilEntryProperties))) == NULL) {
		free(widget);
		return NULL;
	}
	if((widget->event_handler = malloc(sizeof(MuilEventHandler))) == NULL) {
		free(widget->properties);
		free(widget);
		return NULL;
	}
	widget->event_handler->handlers = NULL;
	widget->event_handler->add = muil_event_add;
	widget->event_handler->remove = muil_event_remove;
	widget->event_handler->send = muil_event_send;
	widget->event_handler->add(widget,muil_entry_event_click, MUIL_EVENT_TYPE_MOUSE);
	widget->event_handler->add(widget,muil_entry_event_key, MUIL_EVENT_TYPE_KEYBOARD);

	struct MuilEntryProperties *p = widget->properties;
	p->surface = NULL;
	p->font = font;
	memset(p->text, 0, MUIL_ENTRY_LENGTH + 1);
	p->offset = p->text;
	p->cursor_pos = 0;
	p->background = draw_rect_set_new(1);
	p->cursor = draw_line_set_new(1, 1);
	p->border = draw_line_set_new(4, 1);

	widget->destroy = muil_widget_destroy_entry;
	widget->set_prop = muil_entry_set_prop;
	widget->get_prop = muil_entry_get_prop;
	widget->resize = muil_entry_resize;
	widget->request_size = muil_entry_request_size;
	widget->render = muil_entry_render;
	widget->x = widget->y = widget->w = widget->h = 0;
	widget->enabled = 1;
	widget->needs_redraw = true;

	return widget;
}

void *muil_widget_destroy_entry(MuilWidget *widget) {
	struct MuilEntryProperties *p = widget->properties;
	draw_rect_set_free(p->background);
	draw_line_set_free(p->cursor);
	draw_line_set_free(p->border);
	draw_text_surface_free(p->surface);
	return muil_widget_destroy(widget);
}

void muil_entry_event_key(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	if(!widget->enabled)
		return;
	struct MuilEntryProperties *p = widget->properties;
	int tw = 0;
	switch(type) {
		case MUIL_EVENT_TYPE_KEYBOARD_PRESS:
			if(e->keyboard->keysym == 8) {
				if(p->cursor_pos <= 0)
					return;
				while(draw_utf8_valid((unsigned char *) &p->text[--p->cursor_pos]));
				p->text[p->cursor_pos] = 0;

				if(p->offset > p->text + draw_utf8_char_length(p->text)) {
					while(draw_font_string_w(p->font, p->offset) < widget->w - 4)
						while(draw_utf8_valid((unsigned char *) p->offset--));
					p->offset += draw_utf8_char_length(p->offset);
				} else if(p->offset == p->text + draw_utf8_char_length(p->text))
					p->offset = p->text;
			} else if(p->cursor_pos >= MUIL_ENTRY_LENGTH - 1 || !e->keyboard->character) {
				return;
			} else if(e->keyboard->character >= 0x20) {
				char str[5] = {0, 0, 0, 0, 0};
				int len = draw_utf8_encode(e->keyboard->character, str, 5);
				strcat(&p->text[p->cursor_pos], str);
				for(; (tw = draw_font_string_w(p->font, p->offset)) > widget->w - 4; p->offset += draw_utf8_char_length(p->offset));
				p->cursor_pos += len;
			}
			draw_text_surface_reset(p->surface);
			draw_text_surface_string_append(p->surface, p->offset);
			tw = draw_font_string_w(p->font, p->offset);
			draw_line_set_move(p->cursor, 0, widget->x + tw + 3, widget->y + 2, widget->x + tw + 3, widget->y + widget->h - 2);
			widget->needs_redraw = true;
			break;
	}
}

void muil_entry_event_click(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	if(!widget->enabled)
		return;
	switch(type) {
		case MUIL_EVENT_TYPE_MOUSE_PRESS:
			if(e->mouse->buttons == MUIL_EVENT_MOUSE_BUTTON_LEFT)
				muil_selected_widget = widget;
			break;
	}
}

void muil_entry_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value) {
	struct MuilEntryProperties *p = widget->properties;
	int tw;
	switch(prop) {
		case MUIL_ENTRY_PROP_TEXT:
			strncpy(p->text, value.p, MUIL_ENTRY_LENGTH);
			p->text[MUIL_ENTRY_LENGTH - 1] = 0;
			p->offset = p->text;
			for(; (tw = draw_font_string_w(p->font, p->offset)) > widget->w - 4; p->offset++);
			p->cursor_pos = strlen(p->text);
			draw_text_surface_reset(p->surface);
			draw_text_surface_string_append(p->surface, p->offset);
			draw_line_set_move(p->cursor, 0, widget->x + tw + 3, widget->y + 2, widget->x + tw + 3, widget->y + widget->h - 2);
			widget->needs_redraw = true;
			break;
	}
}

MuilPropertyValue muil_entry_get_prop(MuilWidget *widget, int prop) {
	MuilPropertyValue v = {.p = NULL};
	struct MuilEntryProperties *p = widget->properties;
	switch(prop) {
		case MUIL_ENTRY_PROP_FONT:
			v.p = p->font;
			break;
		case MUIL_ENTRY_PROP_SURFACE:
			v.p = p->surface;
			break;
		case MUIL_ENTRY_PROP_TEXT:
			v.p = p->text;
			break;
	}
	return v;
}

void muil_entry_resize(MuilWidget *widget, int x, int y, int w, int h) {
	if(h == 0 || h == 0)
		return;
	int tw;
	struct MuilEntryProperties *p = widget->properties;
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

	if(p->surface != NULL)
		draw_text_surface_free(p->surface);
	int text_h = draw_font_glyph_h(p->font);
	p->surface = draw_text_surface_new(p->font, MUIL_ENTRY_LENGTH, w, x + 2, y + (h / 2) - (text_h / 2));
	draw_text_surface_string_append(p->surface, p->offset);
	tw = draw_font_string_w(p->font, p->offset);
	draw_line_set_move(p->cursor, 0, widget->x + tw + 3, widget->y + 2, widget->x + tw + 3, widget->y + widget->h - 2);
}

void muil_entry_request_size(MuilWidget *widget, int *w, int *h) {
	int ww = 64 + 4;
	if(w)
		*w = ww;
	if(!h)
		return;
	struct MuilEntryProperties *p = widget->properties;
	int text_h = draw_font_glyph_h(p->font);

	*h = text_h + 4;
}

void muil_entry_render(MuilWidget *widget) {
	static int blink_semaphore = 0;
	struct MuilEntryProperties *p = widget->properties;
	
	if(widget->needs_redraw) {
		draw_set_color(muil_color.widget_background);
		draw_rect_set_draw(p->background, 1);
		
		draw_set_color(muil_color.widget_border);
		draw_line_set_draw(p->border, 4);
		
		draw_set_color(muil_color.text);
		draw_text_surface_draw(p->surface);
		draw_set_color(muil_color.widget_border);
		
		widget->needs_redraw = false;
	}
	
	if(widget == muil_selected_widget) {
		if(blink_semaphore == 60) {
			blink_semaphore = 0;
			draw_set_color(muil_color.widget_background);
			draw_line_set_draw(p->cursor, 1);
		} else if(blink_semaphore == 30) {
			draw_set_color(muil_color.text);
			draw_line_set_draw(p->cursor, 1);
		}
		blink_semaphore++;
	}
}
