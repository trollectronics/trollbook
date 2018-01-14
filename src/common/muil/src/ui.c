/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#include <muil/muil.h>
#include <peripheral.h>

int muil_padding;
MuilWidget *muil_selected_widget;
struct MuilPaneList muil_panelist_dialogue;

MuilColor muil_color = {
	.window_border = 8,
	.window_background = 7,
	.widget_border = 8,
	.widget_background = 15,
	.text = 0,
	.selected = 1,
};

void muil_init(int padding) {
	muil_padding = padding;
	muil_selected_widget = NULL;
	muil_e_m_prev.x =muil_e_m_prev.y =muil_e_m_prev.buttons =muil_e_m_prev.wheel = 0;
	muil_panelist_dialogue.next = NULL;
	muil_panelist_dialogue.pane = NULL;
	//d_input_unicode(1);
	
	//*((volatile uint32_t *) (PERIPHERAL_VGA_BASE + 0x20)) = 0x0;
	//for(int i = 0; i < 8*16; i++)
	//	*((volatile uint32_t *) (PERIPHERAL_VGA_BASE + 0x24)) = 0x3;
}

void *muil_widget_destroy(MuilWidget *widget) {
	struct MuilEventHandlerList *h, *next;
	if(widget->event_handler)
		for(h = widget->event_handler->handlers; h; h = next) {
			next = h->next;
			free(h);
		}
	free(widget->event_handler);
	free(widget->properties);
	free(widget);
	return NULL;
}
