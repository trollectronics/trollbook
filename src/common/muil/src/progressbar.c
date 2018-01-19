/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


//#include <string.h>

#include <muil/muil.h>

MuilWidget *muil_widget_create_progressbar(DrawFont *font) {
	MuilWidget *widget;
	if((widget = malloc(sizeof(MuilWidget))) == NULL)
		return NULL;
	if((widget->properties = malloc(sizeof(struct MuilProgressbarProperties))) == NULL) {
		free(widget);
		return NULL;
	}
	widget->event_handler = NULL;

	struct MuilProgressbarProperties *p = widget->properties;
	p->surface = NULL;
	p->font = font;
	p->background = draw_rect_set_new(1);
	p->border = draw_line_set_new(4, 1);
	p->bar = draw_rect_set_new(1);
	p->progress = 0;
	strcpy(p->text, "0%");

	widget->destroy =muil_widget_destroy_progressbar;
	widget->set_prop =muil_progressbar_set_prop;
	widget->get_prop =muil_progressbar_get_prop;
	widget->resize =muil_progressbar_resize;
	widget->request_size =muil_progressbar_request_size;
	widget->render =muil_progressbar_render;
	widget->x = widget->y = widget->w = widget->h = 0;
	widget->enabled = 1;
	widget->needs_redraw = true;

	return widget;
}

void *muil_widget_destroy_progressbar(MuilWidget *widget) {
	struct MuilProgressbarProperties *p = widget->properties;
	draw_rect_set_free(p->background);
	draw_rect_set_free(p->bar);
	draw_line_set_free(p->border);
	draw_text_surface_free(p->surface);
	return muil_widget_destroy(widget);
}

void muil_progressbar_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value) {
	struct MuilProgressbarProperties *p = widget->properties;
	switch(prop) {
		case MUIL_PROGRESSBAR_PROP_PROGRESS:
			if(value.i < 0 || value.i > 100)
				break;
			p->progress = value.i;
			//sprintf(p->text, "%i%%", value.i);
			draw_rect_set_move(p->bar, 0, widget->x + 2, widget->y + 2, widget->x + 2 + ((widget->w - 4)*value.i / 100), widget->y + widget->h - 2);
			//if(p->surface != NULL)
			//	draw_text_surface_free(p->surface);
			//int text_w;
			//int text_h;
			//draw_font_string_geometrics(p->font, p->text, widget->w, &text_w, &text_h);
			//p->surface = draw_text_surface_new(p->font, draw_utf8_chars_in_string(p->text), widget->w, widget->x + (widget->w / 2) - (text_w / 2), widget->y + (widget->h / 2) - (text_h / 2));
			//draw_text_surface_string_append(p->surface, p->text);
			widget->needs_redraw = true;
			break;
	}
}

MuilPropertyValue muil_progressbar_get_prop(MuilWidget *widget, int prop) {
	MuilPropertyValue v = {.p = NULL};
	struct MuilProgressbarProperties *p = widget->properties;
	switch(prop) {
		case MUIL_PROGRESSBAR_PROP_PROGRESS:
			v.i = p->progress;
			break;
		case MUIL_PROGRESSBAR_PROP_FONT:
			v.p = p->font;
			break;
		case MUIL_PROGRESSBAR_PROP_SURFACE:
			v.p = p->surface;
			break;
		case MUIL_PROGRESSBAR_PROP_TEXT:
			v.p = p->text;
			break;
	}
	return v;
}

void muil_progressbar_resize(MuilWidget *widget, int x, int y, int w, int h) {
	if(h == 0 || h == 0)
		return;
	struct MuilProgressbarProperties *p = widget->properties;
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

	//draw_rect_set_move(p->bar, 0, widget->x+2, widget->y+2, widget->x+widget->w-2, widget->y+widget->h-2);
	draw_rect_set_move(p->bar, 0, widget->x + 2, widget->y + 2, widget->x + 2 + ((widget->w - 4)*p->progress / 100), widget->y + widget->h - 2);

	if(p->surface != NULL)
		draw_text_surface_free(p->surface);
	int text_w;
	int text_h;
	draw_font_string_geometrics(p->font, p->text, w, &text_w, &text_h);
	p->surface = draw_text_surface_new(p->font, draw_utf8_chars_in_string(p->text), w, x + (w / 2) - (text_w / 2), y + (h / 2) - (text_h / 2));
	draw_text_surface_string_append(p->surface, p->text);
}

void muil_progressbar_request_size(MuilWidget *widget, int *w, int *h) {
	int ww = 64 + 4;
	if(w)
		*w = ww;
	if(!h)
		return;
	struct MuilLabelProperties *p = widget->properties;
	int text_h = draw_font_glyph_h(p->font);
	*h = text_h + 4;
	*h = 8 + 4;
}

void muil_progressbar_render(MuilWidget *widget) {
	if(widget->needs_redraw) {
		struct MuilProgressbarProperties *p = widget->properties;
		
		draw_set_color(muil_color.widget_background);
		draw_rect_set_draw(p->background, 1);
		
		draw_line_set_draw(p->border, 4);

		//d_render_logic_op(DARNIT_RENDER_LOGIC_OP_XOR);
		draw_set_color(muil_color.selected);
		draw_rect_set_draw(p->bar, 1);
		//d_render_logic_op(DARNIT_RENDER_LOGIC_OP_NONE);
		draw_set_color(muil_color.text);
		//draw_text_surface_draw(p->surface);
		
		widget->needs_redraw = false;
	}
}
