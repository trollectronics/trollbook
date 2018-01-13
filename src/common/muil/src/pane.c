/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#include <muil/muil.h>

MuilPane *muil_pane_create(int x, int y, int w, int h, MuilWidget *root_widget) {
	MuilPane *pane;
	if((pane = malloc(sizeof(MuilPane))) == NULL)
		return NULL;
	pane->x = x;
	pane->y = y;
	pane->w = w;
	pane->h = h;
	pane->background = draw_rect_set_new(1);
	pane->border = draw_line_set_new(4, 2);
	pane->root_widget = NULL;
	pane->needs_redraw = true;
	muil_pane_resize(pane, x, y, w , h);
	muil_pane_set_root_widget(pane, root_widget);

	return pane;
}

void *muil_pane_destroy(MuilPane *pane) {
	draw_line_set_free(pane->border);
	draw_rect_set_free(pane->background);
	free(pane);
	return NULL;
}

void muil_pane_resize(MuilPane *pane, int x, int y, int w, int h) {
	pane->x = x;
	pane->y = y;
	pane->w = w;
	pane->h = h;
	pane->needs_redraw = true;

	draw_rect_set_move(pane->background, 0, x, y, x + w, y + h);
	draw_line_set_move(pane->border, 0, x, y, x + w, y);
	draw_line_set_move(pane->border, 1, x, y + h, x + w, y + h);
	draw_line_set_move(pane->border, 2, x, y, x, y + h);
	draw_line_set_move(pane->border, 3, x + w, y, x + w, y + h);
	if(pane->root_widget != NULL)
		pane->root_widget->resize(pane->root_widget, x +muil_padding, y +muil_padding, w -muil_padding * 2, h -muil_padding * 2);
}

void muil_pane_set_root_widget(MuilPane *pane, MuilWidget *root_widget) {
	pane->root_widget = root_widget;
	if(pane->root_widget != NULL)
		pane->root_widget->resize(pane->root_widget, pane->x +muil_padding, pane->y +muil_padding, pane->w -muil_padding * 2, pane->h -muil_padding * 2);
}

void muil_pane_render(MuilPane *pane) {
	if(pane->needs_redraw) {
		draw_set_color(muil_color.window_background);
		draw_rect_set_draw(pane->background, 1);
		draw_set_color(muil_color.window_border);
		draw_line_set_draw(pane->border, 4);
		pane->needs_redraw = false;
	}

	if(pane->root_widget != NULL)
		pane->root_widget->render(pane->root_widget);
}

void muil_panelist_render(struct MuilPaneList *panelist) {
	for(; panelist; panelist = panelist->next)
		muil_pane_render(panelist->pane);
}
