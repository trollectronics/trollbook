/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_VBOX_H
#define MUIL_VBOX_H

#include "box.h"

#define MUIL_VBOX_PROP_CHILDREN 0
#define MUIL_VBOX_PROP_SIZE 1

struct MuilVboxProperties {
	MuilWidgetList *children;
	int size;
};

MuilWidget *muil_widget_create_vbox();
void *muil_widget_vbox_destroy(MuilWidget* widget);

void muil_vbox_event_notify_children(MuilWidget *widget, unsigned int type, MuilEvent *e);

void muil_vbox_add_child(MuilWidget *widget, MuilWidget *child, int expand);
void muil_vbox_remove_child(MuilWidget *widget, MuilWidget *child);
void muil_vbox_set_prop(MuilWidget *widget, int prop, MuilPropertyValue value);
MuilPropertyValue muil_vbox_get_prop(MuilWidget *widget, int prop);
void muil_vbox_resize(MuilWidget *widget, int x, int y, int w, int h);
void muil_vbox_request_size(MuilWidget *widget, int *w, int *h);
void muil_vbox_render(MuilWidget *widget);

#endif
