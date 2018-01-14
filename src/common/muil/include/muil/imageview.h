/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_IMAGEVIEW_H
#define MUIL_IMAGEVIEW_H

#define MUIL_IMAGEVIEW_PROP_BITMAP 0
#define MUIL_IMAGEVIEW_PROP_BORDER 1
#define MUIL_IMAGEVIEW_PROP_IMAGE_WIDTH 2
#define MUIL_IMAGEVIEW_PROP_IMAGE_HEIGHT 3

struct MuilImageviewProperties {
	DrawRectSet *background;
	DrawBitmap *bitmap;
	DrawLineSet *border;
	int image_w;
	int image_h;
};

MuilWidget *muil_widget_create_imageview();
MuilWidget *muil_widget_create_imageview_raw(int w, int h);
//MuilWidget *muil_widget_create_imageview_file(const char *filename, int w, int h, int pixel_format);
void *muil_widget_destroy_imageview(MuilWidget *widget);

MuilPropertyValue muil_imageview_get_prop(MuilWidget *widget, int prop);
void muil_imageview_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value);
void muil_imageview_request_size(MuilWidget *widget, int *w, int *h);
void muil_imageview_resize(MuilWidget *widget, int x, int y, int w, int h);
void muil_imageview_render(MuilWidget *widget);

#endif
