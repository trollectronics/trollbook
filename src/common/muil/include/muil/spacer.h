/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_SPACER_H
#define MUIL_SPACER_H

struct MuilSpacerProperties {
	int set_w;
	int set_h;
};

MuilWidget *muil_widget_create_spacer();
MuilWidget *muil_widget_create_spacer_size(int w, int h);

MuilPropertyValue muil_spacer_get_prop(MuilWidget *widget, int prop);
void muil_spacer_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value);
void muil_spacer_request_size(MuilWidget *widget, int *w, int *h);
void muil_spacer_resize(MuilWidget *widget, int x, int y, int w, int h);
void muil_spacer_render(MuilWidget *widget);

#endif
