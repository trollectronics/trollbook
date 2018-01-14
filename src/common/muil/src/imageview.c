/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#include <muil/muil.h>

MuilWidget *muil_widget_create_imageview(DrawBitmap *bitmap) {
	MuilWidget *widget;
	if((widget = malloc(sizeof(MuilWidget))) == NULL)
		return NULL;
	if((widget->properties = malloc(sizeof(struct MuilImageviewProperties))) == NULL) {
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

	struct MuilImageviewProperties *p = widget->properties;
	p->bitmap = NULL;
	p->background = draw_rect_set_new(1);
	p->border = draw_line_set_new(4, 1);
	p->image_w = 0;
	p->image_h = 0;

	widget->destroy =muil_widget_destroy_imageview;
	widget->set_prop =muil_imageview_set_prop;
	widget->get_prop =muil_imageview_get_prop;
	widget->resize =muil_imageview_resize;
	widget->request_size =muil_imageview_request_size;
	widget->render =muil_imageview_render;
	widget->x = widget->y = widget->w = widget->h = 0;
	widget->enabled = 1;
	widget->needs_redraw = true;
	
	MuilPropertyValue v = {.p = (void *) bitmap};
	widget->set_prop(widget, MUIL_IMAGEVIEW_PROP_BITMAP, v);
	
	return widget;
}

void *muil_widget_destroy_imageview(MuilWidget *widget) {
	if(!widget)
		return NULL;
	
	struct MuilImageviewProperties *p = widget->properties;
	
	//draw_bitmap_free(p->bitmap);
	draw_rect_set_free(p->background);
	draw_line_set_free(p->border);
	return muil_widget_destroy(widget);
}

MuilWidget *muil_widget_create_imageview_raw(int w, int h) {
	MuilWidget *widget = NULL;
	DrawBitmap *bitmap = NULL;
	
	if(!(bitmap = draw_bitmap_new_raw(w, h)))
		goto fail;
	
	if(!(widget = muil_widget_create_imageview(bitmap)))
		goto fail;
	
	return widget;
	
	fail:
	draw_bitmap_free(bitmap);
	muil_widget_destroy_imageview(widget);
	
	return NULL;
}

//~ MuilWidget *muil_widget_create_imageview_file(const char *filename, int w, int h, int pixel_format) {
	//~ MuilWidget *widget;
	//~ widget = muil_widget_create_imageview();
	//~ struct MuilImageviewProperties *p = widget->properties;
	//~ p->tilesheet = d_render_tilesheet_load(filename, w, h, pixel_format);
	//~ p->tile = d_render_tile_new(1, p->tilesheet);
	//~ p->image_w = w;
	//~ p->image_h = h;
	//~ return widget;
//~ }

MuilPropertyValue muil_imageview_get_prop(MuilWidget *widget, int prop) {
	struct MuilImageviewProperties *p = widget->properties;
	MuilPropertyValue v = {.p = NULL};
	switch(prop) {
		case MUIL_IMAGEVIEW_PROP_BITMAP:
			v.p = p->bitmap;
			break;
		case MUIL_IMAGEVIEW_PROP_IMAGE_WIDTH:
			v.i = p->image_w;
			break;
		case MUIL_IMAGEVIEW_PROP_IMAGE_HEIGHT:
			v.i = p->image_h;
			break;
	}
	return v;
}

void muil_imageview_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value) {
	struct MuilImageviewProperties *p = widget->properties;
	
	switch(prop) {
		case MUIL_IMAGEVIEW_PROP_BITMAP:
			p->bitmap = value.p;
			p->image_w = p->bitmap->w;
			p->image_h = p->bitmap->h;
			
			widget->resize(widget, widget->x, widget->y, p->image_w, p->image_h);
			break;
	}
	
	return;
}

void muil_imageview_request_size(MuilWidget *widget, int *w, int *h) {
	struct MuilImageviewProperties *p = widget->properties;
	if(w)
		*w = p->image_w;
	if(h)
		*h = p->image_h;
}

void muil_imageview_resize(MuilWidget *widget, int x, int y, int w, int h) {
	struct MuilImageviewProperties *p = widget->properties;
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

	draw_bitmap_move(p->bitmap, x, y);
}

void muil_imageview_render(MuilWidget *widget) {
	if(widget->needs_redraw) {
		struct MuilImageviewProperties *p = widget->properties;
		
		draw_set_color(muil_color.window_background);
		draw_rect_set_draw(p->background, 1);
		
		draw_bitmap_draw(p->bitmap);
		draw_set_color(muil_color.widget_border);
		draw_line_set_draw(p->border, 4);
		
		widget->needs_redraw = false;
	}
}
