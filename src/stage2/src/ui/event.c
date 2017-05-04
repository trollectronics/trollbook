/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


//#include <string.h>

#include <muil/muil.h>

//higher value for lower speed, yeah it's stupid but whatever
#define KEYBOARD_REPEAT_SPEED 3
#define KEYBOARD_REPEAT_DELAY 20

#define SETMOD(m) e_k.modifiers=(key_action==DARNIT_KEYACTION_PRESS)?e_k.modifiers|MUIL_EVENT_KEYBOARD_MOD_##m :e_k.modifiers&~MUIL_EVENT_KEYBOARD_MOD_##m

int muil_event_global_removed = 0;

void muil_events(struct MuilPaneList *panes, int render) {
	
	//override for dialogues such as message boxes.
	if(muil_panelist_dialogue.pane) {
		muil_panelist_render(panes);
		panes = &muil_panelist_dialogue;
	}
	#if 0

	MuilEvent e;
	MuilEventMouse e_m;
	MuilEventButtons e_b;
	MuilEventUI e_u;
	static MuilEventKeyboard e_k = {0, 0, 0}, e_k_repeat = {0, 0, 0};
	static int repeat = 0;

	DARNIT_KEYS buttons;
	buttons = d_keys_get();
	memcpy(&e_b, &buttons, sizeof(MuilEventButtons));

	DARNIT_MOUSE mouse;
	mouse = d_mouse_get();

	e_m.x = mouse.x;
	e_m.y = mouse.y;
	e_m.buttons = (mouse.lmb) | (mouse.rmb << 2);
	e_m.wheel = mouse.wheel;

	int key_action = 0;
	DARNIT_KEY_RAW rawkey = d_key_raw_pop();
	key_action = rawkey.action;
	e_k.keysym = rawkey.keysym;

	switch(e_k.keysym) {
		case KEY(LCTRL):
			SETMOD(LCTRL);
			break;
		case KEY(RCTRL):
			SETMOD(RCTRL);
			break;
		case KEY(LSHIFT):
			SETMOD(LSHIFT);
			break;
		case KEY(RSHIFT):
			SETMOD(RSHIFT);
			break;
		case KEY(LALT):
			SETMOD(LALT);
			break;
		case KEY(RALT):
			SETMOD(RALT);
			break;
		case KEY(LSUPER):
			SETMOD(LSUPER);
			break;
		case KEY(RSUPER):
			SETMOD(RSUPER);
			break;
	}

	e_k.character = rawkey.unicode;


	e.keyboard = &e_k;
	if(muil_selected_widget) {
		//Keyboard events for selected widget
		if(e_k_repeat.character || e_k_repeat.keysym == 8) {
			if(repeat > KEYBOARD_REPEAT_DELAY && !((repeat - KEYBOARD_REPEAT_DELAY) % KEYBOARD_REPEAT_SPEED)) {
				MuilEvent e_repeat = {.keyboard = &e_k_repeat};
				muil_selected_widget->event_handler->send(muil_selected_widget, MUIL_EVENT_TYPE_KEYBOARD_PRESS, &e_repeat);
			}
			repeat++;
		}
		if(key_action == DARNIT_KEYACTION_PRESS) {
			if(e_k.character || e_k.keysym == 8) {
				e_k_repeat = e_k;
				repeat = 0;
			}
			muil_selected_widget->event_handler->send(muil_selected_widget, MUIL_EVENT_TYPE_KEYBOARD_PRESS, &e);
		} else if(key_action == DARNIT_KEYACTION_RELEASE) {
			if(e_k.keysym == e_k_repeat.keysym) {
				e_k_repeat.character = 0;
				e_k_repeat.keysym = 0;
				e_k_repeat.modifiers = 0;
				repeat = 0;
			}
			muil_selected_widget->event_handler->send(muil_selected_widget, MUIL_EVENT_TYPE_KEYBOARD_RELEASE, &e);
		}
	} else {
		//Global keyboard events
		if(key_action == DARNIT_KEYACTION_PRESS)
			muil_event_global_send(MUIL_EVENT_TYPE_KEYBOARD_PRESS, &e);
		else if(key_action == DARNIT_KEYACTION_RELEASE)
			muil_event_global_send(MUIL_EVENT_TYPE_KEYBOARD_RELEASE, &e);
	}

	//Global mouse events
	e.mouse = &e_m;
	if((muil_e_m_prev.buttons & e_m.buttons) < e_m.buttons)
		muil_event_global_send(MUIL_EVENT_TYPE_MOUSE_PRESS, &e);
	if((muil_e_m_prev.buttons & e_m.buttons) <muil_e_m_prev.buttons)
		muil_event_global_send(MUIL_EVENT_TYPE_MOUSE_RELEASE, &e);
	if(e_m.buttons)
		muil_event_global_send(MUIL_EVENT_TYPE_MOUSE_DOWN, &e);
	muil_event_global_send(MUIL_EVENT_TYPE_MOUSE_ENTER, &e);

	//Mouse events for widgets
	#endif
	struct MuilPaneList *p;
	for(p = panes; p; p = p->next) {
		#if 0
		if(p->pane->root_widget->event_handler) {
			MuilWidget *w = p->pane->root_widget;

			e.ui = &e_u;
			e.mouse = &e_m; //ui events get sent the mouse value to be able to track mouse movement for child widgets
			w->event_handler->send(w, MUIL_EVENT_TYPE_UI_EVENT, &e);

			if(PINR(e_m.x, e_m.y, w->x, w->y, w->w, w->h)) {
				if(!PINR(muil_e_m_prev.x,muil_e_m_prev.y, w->x, w->y, w->w, w->h))
					w->event_handler->send(w, MUIL_EVENT_TYPE_MOUSE_ENTER, &e);
				if((muil_e_m_prev.buttons & e_m.buttons) < e_m.buttons)
					w->event_handler->send(w, MUIL_EVENT_TYPE_MOUSE_PRESS, &e);
				if((muil_e_m_prev.buttons & e_m.buttons) <muil_e_m_prev.buttons)
					w->event_handler->send(w, MUIL_EVENT_TYPE_MOUSE_RELEASE, &e);
				if(e_m.buttons)
					w->event_handler->send(w, MUIL_EVENT_TYPE_MOUSE_DOWN, &e);
				if(e_m.wheel)
					w->event_handler->send(w, MUIL_EVENT_TYPE_MOUSE_SCROLL, &e);

			} else if(PINR(muil_e_m_prev.x,muil_e_m_prev.y, w->x, w->y, w->w, w->h))
				w->event_handler->send(w, MUIL_EVENT_TYPE_MOUSE_LEAVE, &e);
		}
		#endif
		if(render && p->pane)
			muil_pane_render(p->pane);
	}

	#if 0
	e.buttons = &e_b;
	muil_event_global_send(MUIL_EVENT_TYPE_BUTTONS, &e);
	e.mouse = &e_m;
	muil_event_global_send(MUIL_EVENT_TYPE_UI, &e);

	muil_e_m_prev = e_m;
	#endif
}

void muil_event_add(MuilWidget *widget, void (*handler)(MuilWidget *, unsigned int, MuilEvent *), unsigned int mask) {
	struct MuilEventHandlerList **h, *hh;
	for(h = &(widget->event_handler->handlers); *h; h = &((*h)->next));
	if((*h = malloc(sizeof(struct MuilEventHandlerList))) == NULL)
		return;
	hh = *h;
	hh->handler = handler;
	hh->mask = mask;
	hh->next = NULL;
}

void muil_event_remove(MuilWidget *widget, void (*handler)(MuilWidget *, unsigned int, MuilEvent *), unsigned int mask) {
	struct MuilEventHandlerList **h, *h_next;
	for(h = &(widget->event_handler->handlers); *h; h = &((*h)->next)) {
		if((*h)->handler == handler && ((*h)->mask & mask)) {
			h_next = (*h)->next;
			free(*h);
			*h = h_next;
			break;
		}
	}
}

void muil_event_send(MuilWidget *widget , unsigned int type, MuilEvent *e) {
	struct MuilEventHandlerList *h;
	for(h = widget->event_handler->handlers; h; h = h->next)
		if((type & h->mask) == type)
			h->handler(widget, type & h->mask, e);
}

void muil_event_global_add(void (*handler)(MuilWidget *, unsigned int, MuilEvent *), unsigned int mask) {
	struct MuilEventHandlerList **h, *hh;
	for(h = &(muil_event_global_handlers); *h; h = &((*h)->next));
	if((*h = malloc(sizeof(struct MuilEventHandlerList))) == NULL)
		return;
	hh = *h;
	hh->handler = handler;
	hh->mask = mask;
	hh->next = NULL;
}

void muil_event_global_remove(void (*handler)(MuilWidget *, unsigned int, MuilEvent *), unsigned int mask) {
	struct MuilEventHandlerList **h, *h_next;
	for(h = &(muil_event_global_handlers); *h; h = &((*h)->next)) {
		if((*h)->handler == handler && ((*h)->mask & mask)) {
			h_next = (*h)->next;
			free(*h);
			*h = h_next;
			muil_event_global_removed = 1;
			break;
		}
	}
}

void muil_event_global_send(unsigned int type, MuilEvent *e) {
	muil_event_global_removed = 0;
	struct MuilEventHandlerList *h;
	for(h =muil_event_global_handlers; h; h = h->next)
		if((type & h->mask) == type) {
			h->handler(NULL, type & h->mask, e);
			if(muil_event_global_removed)
				break;
		}
}
