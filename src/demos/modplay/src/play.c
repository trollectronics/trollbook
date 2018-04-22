#include <stdbool.h>
#include <stddef.h>
#include <rickmod.h>
#include <terminal.h>
#include <fat.h>
#include <printf.h>
#include <mem.h>
#include <sound.h>
#include <interrupt.h>
#include <peripheral.h>
#include <muil/muil.h>
#include "main.h"

#define ABS(a) ((a) > 0 ? (a) : -(a))

static volatile uint16_t *sound_buffer[] = {
	(volatile uint16_t *) (LLRAM_BASE + 800*480 + 1024),
	(volatile uint16_t *) (LLRAM_BASE + 800*480 + 2048),
};

static int buffer;
static bool done;
static struct RickmodState *rm;

static void audio_callback(int number) {
	volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	volatile uint32_t *interrupt_hw = (volatile uint32_t *) PERIPHERAL_INTERRUPT_BASE;
	
	rm_mix_u8(rm, (uint8_t *) sound_buffer[buffer], 512);
	
	if(rm_end_reached(rm))
		done = true;
	
	interrupt_hw[32 + PERIPHERAL_ID_AUDIO] = 0x0;
	buffer = (sound_hw[1] & 0x1);
}

static void timer_callback(int number) {
	volatile uint32_t *timer_hw = (volatile uint32_t *) PERIPHERAL_TIMER_BASE;
	volatile uint32_t *interrupt_hw = (volatile uint32_t *) PERIPHERAL_INTERRUPT_BASE;
	terminal_putc('.');
	muil_color.selected = ~muil_color.selected;
	timer_hw[1] = 0x1;
	interrupt_hw[32 + PERIPHERAL_ID_TIMER] = 0x0;
}

static struct {
	MuilWidget *vbox;
	MuilWidget *label_title;
	MuilWidget *list_samples;
	MuilWidget *hbox_chan;
	MuilWidget *progress_level[2];
	MuilWidget *button_stop;
	
	struct MuilPaneList pane_list;
} player_ui;

void player_init() {
	player_ui.pane_list.pane = muil_pane_create(200, 100, 400, 280, player_ui.vbox = muil_widget_create_vbox());
	player_ui.pane_list.next = NULL;
	
	muil_vbox_add_child(player_ui.vbox, player_ui.label_title = muil_widget_create_label(font_small, "MOD file"), 0);
	muil_vbox_add_child(player_ui.vbox, player_ui.list_samples = muil_widget_create_listbox(font_small), 1);
	
	muil_vbox_add_child(player_ui.vbox, player_ui.hbox_chan = muil_widget_create_hbox(), 0);
	muil_hbox_add_child(player_ui.hbox_chan, player_ui.progress_level[0] = muil_widget_create_progressbar(font_small), 1);
	muil_hbox_add_child(player_ui.hbox_chan, player_ui.progress_level[1] = muil_widget_create_progressbar(font_small), 1);
	
	muil_vbox_add_child(player_ui.vbox, player_ui.button_stop = muil_widget_create_button_text(font_small, "Stop"), 0);
}

static void player_ui_volumes_bargraphs() {
	uint16_t vol;
	int vol_l, vol_r;
	MuilPropertyValue v;
	
	vol = sound_buffer[buffer][0];
	vol_l = (vol >> 8) & 0xFF;
	vol_r = vol & 0xFF;
	
	vol_l = 100*(ABS(vol_l - 128))/128;
	vol_r = 100*(ABS(vol_r - 128))/128;
	
	v.i = vol_l;
	player_ui.progress_level[0]->set_prop(player_ui.progress_level[0], MUIL_PROGRESSBAR_PROP_PROGRESS, v);
	v.i = vol_r;
	player_ui.progress_level[1]->set_prop(player_ui.progress_level[1], MUIL_PROGRESSBAR_PROP_PROGRESS, v);
}

void play(const char *filename) {
	int i, j, fd;
	uint32_t size;
	uint32_t *src, *dst;
	void *mod_buffer;
	MuilPropertyValue v;
	
	terminal_clear();
	printf("open file %s\n", filename);
	fd = fat_open(filename, O_RDONLY);
	size = fat_fsize(fd);
	
	printf("allocating %u bytes\n", size);
	mod_buffer = malloc(size);
	printf("successfully allocated %u bytes\n", size);
	dst = mod_buffer;
	for(j = 0; j < size; j += 512) {
		fat_read_sect(fd);
		src = (uint32_t *) fat_buf;
		for(i = 0; i < 512/4; i++) {
			*dst++ = *src++;
		}
		printf("\r%u/%u kB", j >> 10, size >> 10);
	}
	fat_close(fd);
	printf("\n");
	
	sound_setup((void *) (LLRAM_BASE + 800*480 + 1024));
	
	rm = rm_init(48000, mod_buffer, size);
	rm_repeat_set(rm, false);
	//printf("MOD: %s\n", rm->name);
	v.p = rm->name;
	player_ui.label_title->set_prop(player_ui.label_title, MUIL_LABEL_PROP_TEXT, v);
	
	muil_listbox_clear(player_ui.list_samples);
	for(i = 0; i < 31; i++) {
		muil_listbox_add(player_ui.list_samples, rm->sample[i].name);
	}
	
	//volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	//sound_hw[2] = (uint32_t) LLRAM_BASE + 800*480;
	
	rm_mix_u8(rm, (uint8_t *) sound_buffer[0], 512);
	rm_mix_u8(rm, (uint8_t *) sound_buffer[1], 512);
	
	done = false;
	terminal_clear();
	
	muil_pane_resize(player_ui.pane_list.pane, player_ui.pane_list.pane->x, player_ui.pane_list.pane->y, player_ui.pane_list.pane->w, player_ui.pane_list.pane->h);
	
	volatile uint32_t *timer_hw = (volatile uint32_t *) PERIPHERAL_TIMER_BASE;
	
	timer_hw[0] = 0xFFFF0001UL;
	timer_hw[4] = 0x3;
	timer_hw[5] = 0;
	timer_hw[6] = 500;
	
	
	interrupt_register(3, timer_callback);
	interrupt_perihperal_enable(PERIPHERAL_ID_TIMER, 3);
	
	interrupt_register(5, audio_callback);
	interrupt_perihperal_enable(PERIPHERAL_ID_AUDIO, 5);
	interrupt_global_enable();
	
	sound_start();
	
	while(!done) {
		player_ui_volumes_bargraphs();
		muil_events(&player_ui.pane_list, true);
	}
	
	timer_hw[4] = 0x0;
	
	sound_stop();
	interrupt_global_disable();
	rm_free(rm);
	free(mod_buffer);
}
