#include <muil/muil.h>
#include <draw/font.h>
#include <fat.h>
#include <font.h>
#include <peripheral.h>
#include "main.h"

static void list_dir(const char *path, MuilWidget *listbox) {
	int files, i, j;
	static char filename[32];
	
	uint8_t stat;
	struct FATDirList list[8];
	char *buf, *tmp;
	
	for(files = 0; (i = fat_dirlist(path, list, 8, files)); files += i) {
		for (j = i - 1; j >= 0; j--) {
			if(list[j].filename[0]) {
				stat = list[j].attrib;
				
				//skip volume labels
				if(stat & 0x8)
					continue;
				
				if(list[j].filename[0] == '.')
					continue;
				
				buf = filename;
				
				//~ for(k = 5; k != ~0; k--) {
					//~ if(stat & (0x1 << k))
						//~ *buf++ = attribs[k];
					//~ else
						//~ *buf++ = '-';
				//~ }
				if(stat & 0x10) {
					*buf++ = '[';
				}
				//~ } else {
					//~ pathcat((char *) pathbuf, path, list[j].filename);
					//~ fd = fat_open(pathbuf, O_RDONLY);
					//~ *buf++ = '\t';
					//~ //print_filesize(fat_fsize(fd));
					//~ *buf++ = '0';
					//~ *buf++ = '\t';
					//~ fat_close(fd);
				//~ }
				tmp = list[j].filename;
				
				while(*tmp) {
					*buf++ = *tmp++;
				}
				if(stat & 0x10) {
					*buf++ = ']';
				}
				*buf = 0;
				
				muil_listbox_add(listbox, filename);
			}
		}
	}
}

static struct MuilPaneList panelist;
static MuilWidget *vbox;
static MuilWidget *button;
static MuilWidget *label;
static MuilWidget *listbox;
static MuilWidget *entry;

void button_callback() {
	MuilPropertyValue v;
	char path[16] = "/";
	
	v = listbox->get_prop(listbox, MUIL_LISTBOX_PROP_SELECTED);
	if(v.i < 0)
		return;
	
	strcat(path, muil_listbox_get(listbox, v.i));
	
	play(path);
	muil_pane_resize(panelist.pane, panelist.pane->x, panelist.pane->y, panelist.pane->w, panelist.pane->h);
}

void browse() {
	panelist.pane = muil_pane_create(20, 20, 320, 480 - 40, vbox = muil_widget_create_vbox());
	panelist.next = NULL;

	muil_vbox_add_child(vbox, label = muil_widget_create_label(font_small, "Load MOD file"), 0);
	muil_vbox_add_child(vbox, listbox = muil_widget_create_listbox(font_small), 1);
	muil_vbox_add_child(vbox, muil_widget_create_spacer(4), 0);
	muil_vbox_add_child(vbox, entry = muil_widget_create_entry(font_small), 0);
	muil_vbox_add_child(vbox, button = muil_widget_create_button_text(font_small, "Play"), 0);
	
	list_dir("/", listbox);
	
	button->event_handler->add(button, button_callback, MUIL_EVENT_TYPE_UI_WIDGET_ACTIVATE);
	muil_events(&panelist, true);
	
	for(;;) {
		volatile uint32_t *interrupt_hw = (volatile uint32_t *) PERIPHERAL_INTERRUPT_BASE;
		
		muil_events(&panelist, true);
		
		while(!interrupt_hw[32 + 10]);
		interrupt_hw[32 + 10] = 0x0;
	}
}
