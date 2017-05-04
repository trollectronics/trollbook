#include <muil/muil.h>
#include <draw/font.h>
#include "vgafont.h"

void ui() {
	struct MuilPaneList panelist;
	MuilWidget *vbox;
	MuilWidget *button;
	MuilWidget *label;
	DrawFont *font = draw_font_new(vgafont_data, 8, 16);
	
	muil_init(4);

	panelist.pane = muil_pane_create(10, 10, 300, 150, vbox = muil_widget_create_vbox());
	panelist.next = NULL;

	muil_vbox_add_child(vbox, label = muil_widget_create_label(font, "test"), 1);
	muil_vbox_add_child(vbox, muil_widget_create_entry(font), 0);
	muil_vbox_add_child(vbox, button = muil_widget_create_button_text(font, "lol"), 0);

	//button->event_handler->add(button, button_callback, MUIL_EVENT_TYPE_UI_WIDGET_ACTIVATE);
	
	muil_events(&panelist, true);
	
	for(;;) {
		muil_events(&panelist, false);
	}
}
