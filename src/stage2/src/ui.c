#include <muil/muil.h>
#include <draw/font.h>
#include "font.h"

void ui() {
	struct MuilPaneList panelist;
	MuilWidget *vbox;
	MuilWidget *button;
	MuilWidget *label;
	MuilWidget *listbox;
	DrawFont *font = draw_font_new(smallfont_data, 8, 8);
	
	muil_init(4);

	panelist.pane = muil_pane_create(100, 100, 320, 256, vbox = muil_widget_create_vbox());
	panelist.next = NULL;

	muil_vbox_add_child(vbox, label = muil_widget_create_label(font, "Arnes sommarstuga luktar korv."), 0);
	muil_vbox_add_child(vbox, listbox = muil_widget_create_listbox(font), 1);
	muil_vbox_add_child(vbox, muil_widget_create_spacer(4), 0);
	muil_vbox_add_child(vbox, muil_widget_create_entry(font), 0);
	muil_vbox_add_child(vbox, button = muil_widget_create_button_text(font, "lol"), 0);

	//button->event_handler->add(button, button_callback, MUIL_EVENT_TYPE_UI_WIDGET_ACTIVATE);
	muil_events(&panelist, true);
	
	for(;;) {
		muil_events(&panelist, false);
	}
}
