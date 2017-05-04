/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#include <muil/muil.h>

#define MESSAGE_BOX_WIDTH 256
#define MESSAGE_BOX_HEIGHT 128

void muil_messagebox(DrawFont *font, const char *text, const char *button_text) {
	static MuilPane *pane = NULL;
	static MuilWidget *label = NULL, *button = NULL, *vbox = NULL;
	if(label)
		label->destroy(label);
	if(button)
		button->destroy(button);
	if(vbox)
		vbox->destroy(vbox);
	if(pane)
		muil_pane_destroy(pane);

	if(!button_text)
		button_text = "Ok";
	label =muil_widget_create_label(font, text);
	button =muil_widget_create_button_text(font, button_text);
	button->event_handler->add(button,muil_messagebox_button_click, MUIL_EVENT_TYPE_UI_WIDGET_ACTIVATE);
	vbox =muil_widget_create_vbox();
	//muil_panelist_dialogue.pane = pane =muil_pane_create(muil_platform.screen_w / 2 - MESSAGE_BOX_WIDTH / 2,muil_platform.screen_h / 2 - MESSAGE_BOX_HEIGHT / 2, MESSAGE_BOX_WIDTH, MESSAGE_BOX_HEIGHT, vbox);
	muil_panelist_dialogue.pane = pane =muil_pane_create(100, 100, MESSAGE_BOX_WIDTH, MESSAGE_BOX_HEIGHT, vbox);
	muil_vbox_add_child(vbox, label, 1);
	muil_vbox_add_child(vbox, button, 0);
}

void muil_messagebox_button_click(MuilWidget *widget, unsigned int type, MuilEvent *e) {
	muil_panelist_dialogue.pane = NULL;
}
