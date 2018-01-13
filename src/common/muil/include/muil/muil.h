/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_H
#define MUIL_H

#include <stdbool.h>
#include <draw/line.h>
#include <draw/rect.h>
#include <draw/text.h>
#include <draw/bitmap.h>
#include <draw/font.h>
#include <draw/color.h>
#include <draw/utf8.h>

#include <mem.h>

#define MUIL_PADDING_DEFAULT 4
extern int muil_padding;

typedef union {
	void *p;
	int i;
} MuilPropertyValue;

typedef struct MuilColor MuilColor;
struct MuilColor {
	DrawColor window_border;
	DrawColor window_background;
	DrawColor widget_border;
	DrawColor widget_background;
	DrawColor text;
	DrawColor selected;
};

extern MuilColor muil_color;

typedef struct MuilWidget {
	void *(*destroy)(struct MuilWidget *);
	void (*set_prop)(struct MuilWidget *, int, MuilPropertyValue);
	MuilPropertyValue (*get_prop)(struct MuilWidget *, int);
	void (*resize)(struct MuilWidget *, int, int, int, int);
	void (*request_size)(struct MuilWidget *, int *, int *);
	void (*render)(struct MuilWidget *);
	void *properties;
	struct MuilEventHandler *event_handler;
	int x;
	int y;
	int w;
	int h;
	bool enabled;
	bool needs_redraw;
} MuilWidget;

extern MuilWidget *muil_selected_widget;

#include "event.h"

#include "pane.h"
#include "label.h"
#include "vbox.h"
#include "hbox.h"
#include "button.h"
#include "checkbox.h"
#include "entry.h"
#include "progressbar.h"
#include "listbox.h"
#include "spacer.h"
#include "imageview.h"
#include "slider.h"

extern struct MuilPaneList muil_panelist_dialogue;

#include "messagebox.h"

void muil_init(int padding);
void *muil_widget_destroy(MuilWidget *widget);

#endif
