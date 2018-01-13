/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_PANE_H
#define MUIL_PANE_H

#include <stdbool.h>

typedef struct {
	int x;
	int y;
	int w;
	int h;
	DrawLineSet *border;
	DrawRectSet *background;
	MuilWidget *root_widget;
	bool needs_redraw;
} MuilPane;

struct MuilPaneList {
	MuilPane *pane;
	struct MuilPaneList *next;
};

MuilPane *muil_pane_create(int x, int y, int w, int h, MuilWidget *root_widget);
void *muil_pane_destroy(MuilPane *pane);

void muil_pane_resize(MuilPane *pane, int x, int y, int w, int h);
void muil_pane_set_root_widget(MuilPane *pane, MuilWidget *root_widget);
void muil_pane_render(MuilPane *pane);
void muil_panelist_render(struct MuilPaneList *panelist);

#endif
