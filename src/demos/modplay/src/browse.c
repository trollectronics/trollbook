#include <muil/muil.h>
#include <draw/font.h>
#include <fat.h>
#include <font.h>
#include <peripheral.h>

static void list_dir(const char *path, MuilWidget *listbox) {
	int files, fd, i, j, k;
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

void button_callback() {
	
}

void browse() {
	struct MuilPaneList panelist;
	MuilWidget *vbox;
	MuilWidget *button;
	MuilWidget *label;
	MuilWidget *listbox;
	DrawFont *font = draw_font_new(smallfont_data, 8, 8);
	
	muil_init(4);

	panelist.pane = muil_pane_create(20, 20, 320, 480 - 40, vbox = muil_widget_create_vbox());
	panelist.next = NULL;

	muil_vbox_add_child(vbox, label = muil_widget_create_label(font, "Load MOD file"), 0);
	muil_vbox_add_child(vbox, listbox = muil_widget_create_listbox(font), 1);
	muil_vbox_add_child(vbox, muil_widget_create_spacer(4), 0);
	muil_vbox_add_child(vbox, muil_widget_create_entry(font), 0);
	muil_vbox_add_child(vbox, button = muil_widget_create_button_text(font, "Play"), 0);
	
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
