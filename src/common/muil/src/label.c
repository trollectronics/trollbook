/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#include <muil/muil.h>

MuilWidget *muil_widget_create_label(DrawFont *font, const char *text) {
	MuilWidget *widget;
	if((widget = malloc(sizeof(MuilWidget))) == NULL)
		return NULL;
	if((widget->properties = malloc(sizeof(struct MuilLabelProperties))) == NULL) {
		free(widget);
		return NULL;
	}
	struct MuilLabelProperties *p = widget->properties;
	p->surface = NULL;
	p->font = font;
	p->background = draw_rect_set_new(1);
	widget->event_handler = NULL;
	widget->destroy =muil_widget_destroy_label;
	widget->set_prop =muil_label_set_prop;
	widget->get_prop =muil_label_get_prop;
	widget->resize =muil_label_resize;
	widget->request_size =muil_label_request_size;
	widget->render =muil_label_render;
	widget->x = widget->y = widget->w = widget->h = 0;
	widget->enabled = 1;
	widget->needs_redraw = true;

	MuilPropertyValue v = {.p = (void *) text};
	widget->set_prop(widget, MUIL_LABEL_PROP_TEXT, v);
	return widget;
}

void *muil_widget_destroy_label(MuilWidget *widget) {
	struct MuilLabelProperties *p = widget->properties;
	draw_rect_set_free(p->background);
	draw_text_surface_free(p->surface);
	return muil_widget_destroy(widget);
}

void muil_label_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value) {
	struct MuilLabelProperties *p = widget->properties;
	switch(prop) {
		case MUIL_LABEL_PROP_TEXT:
			p->text = value.p;
			muil_label_resize(widget, widget->x, widget->y, widget->w, widget->h);
			break;
		case MUIL_LABEL_PROP_FONT:
			p->font = value.p;
			break;
	}
}

MuilPropertyValue muil_label_get_prop(MuilWidget *widget, int prop) {
	struct MuilLabelProperties *p = widget->properties;
	MuilPropertyValue v = {.p = NULL};
	switch(prop) {
		case MUIL_LABEL_PROP_FONT:
			v.p = p->font;
			break;
		case MUIL_LABEL_PROP_SURFACE:
			v.p = p->surface;
			break;
		case MUIL_LABEL_PROP_TEXT:
			v.p = p->text;
			break;
	}
	return v;
}

void muil_label_resize(MuilWidget *widget, int x, int y, int w, int h) {
	if(h == 0 || h == 0)
		return;
	struct MuilLabelProperties *p = widget->properties;
	widget->x = x;
	widget->y = y;
	widget->w = w;
	widget->h = h;
	widget->needs_redraw = true;
	
	if(p->surface != NULL)
		draw_text_surface_free(p->surface);
	int text_w;
	int text_h;
	draw_font_string_geometrics(p->font, p->text, w, &text_w, &text_h);
	p->surface = draw_text_surface_new(p->font, draw_utf8_chars_in_string(p->text), w, x + (w / 2) - (text_w / 2), y + (h / 2) - (text_h / 2));
	draw_text_surface_string_append(p->surface, p->text);
	draw_rect_set_move(p->background, 0, x, y, x + w, y + h);
}

void muil_label_request_size(MuilWidget *widget, int *w, int *h) {
	struct MuilLabelProperties *p = widget->properties;
	int ww = ((w == NULL) || (*w == -1)) ? draw_font_string_w(p->font, p->text) +muil_padding : *w;
	if(ww == 0 || *h == 0)
		return;

	int text_w;
	int text_h;
	draw_font_string_geometrics(p->font, p->text, ww, &text_w, &text_h);
	*h = text_h;
	if(w && *w == -1)
		*w = text_w +muil_padding;
}

void muil_label_render(MuilWidget *widget) {
	if(widget->needs_redraw) {
		struct MuilLabelProperties *p = widget->properties;
		
		draw_set_color(muil_color.window_background);
		draw_rect_set_draw(p->background, 1);
		
		
		draw_set_color(muil_color.text);
		draw_text_surface_draw(p->surface);
		
		widget->needs_redraw = false;
	}
}
