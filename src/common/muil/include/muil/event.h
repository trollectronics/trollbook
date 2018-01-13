/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_EVENT_H
#define MUIL_EVENT_H

#include <input.h>

struct MuilPaneList;

#define KEY(k) INPUT_KEY_##k

//~ #define KEYSYM_RETURN KEY(RETURN)
//~ #define KEYSYM_SPACE ' '
//~ #define KEYSYM_KP_ENTER KEY(RETURN)

//Check if point (px, py) is inside the bounds of a rectangle
#define PINR(px, py, x, y, w, h) ((px)>(x)&&(py)>(y)&&(px)<((x)+(w))&&(py)<((y)+(h)))

#define MUIL_EVENT_TYPE_KEYBOARD		0x1FF
#define MUIL_EVENT_TYPE_KEYBOARD_PRESS	0x101
#define MUIL_EVENT_TYPE_KEYBOARD_RELEASE	0x102
#define MUIL_EVENT_TYPE_MOUSE		0x2FF
#define MUIL_EVENT_TYPE_MOUSE_BUTTON	0x20C
#define MUIL_EVENT_TYPE_MOUSE_ENTER	0x201
#define MUIL_EVENT_TYPE_MOUSE_LEAVE	0x202
#define MUIL_EVENT_TYPE_MOUSE_RELEASE		0x204
#define MUIL_EVENT_TYPE_MOUSE_PRESS	0x208
#define MUIL_EVENT_TYPE_MOUSE_DOWN	0x210
#define MUIL_EVENT_TYPE_MOUSE_SCROLL	0x220
#define MUIL_EVENT_TYPE_JOYSTICK		0x4FF
#define MUIL_EVENT_TYPE_BUTTONS		0x801
#define MUIL_EVENT_TYPE_UI		0x10FF
#define MUIL_EVENT_TYPE_UI_EVENT		0x1001
#define MUIL_EVENT_TYPE_UI_WIDGET_ACTIVATE	0x1002

#define MUIL_EVENT_KEYBOARD_MOD_CTRL	0x03
#define MUIL_EVENT_KEYBOARD_MOD_LCTRL	0x01
#define MUIL_EVENT_KEYBOARD_MOD_RCTRL	0x02
#define MUIL_EVENT_KEYBOARD_MOD_SHIFT	0x0C
#define MUIL_EVENT_KEYBOARD_MOD_LSHIFT	0x04
#define MUIL_EVENT_KEYBOARD_MOD_RSHIFT	0x08
#define MUIL_EVENT_KEYBOARD_MOD_ALT	0x30
#define MUIL_EVENT_KEYBOARD_MOD_LALT	0x10
#define MUIL_EVENT_KEYBOARD_MOD_RALT	0x20
#define MUIL_EVENT_KEYBOARD_MOD_SUPER	0xC0
#define MUIL_EVENT_KEYBOARD_MOD_LSUPER	0x40
#define MUIL_EVENT_KEYBOARD_MOD_RSUPER	0x80

typedef struct {
	int character;
	unsigned char modifiers;
	int keysym;
} MuilEventKeyboard;

#define MUIL_EVENT_MOUSE_BUTTON_LEFT	0x01
#define MUIL_EVENT_MOUSE_BUTTON_MIDDLE	0x02
#define MUIL_EVENT_MOUSE_BUTTON_RIGHT	0x04

typedef struct {
	unsigned int x;
	unsigned int y;
	unsigned char buttons;
	signed int wheel;
} MuilEventMouse;

typedef struct {
	
} MuilEventJoystick;


typedef struct {
	
} MuilEventUI;

typedef union {
	MuilEventKeyboard *keyboard;
	MuilEventMouse *mouse;
	MuilEventJoystick *joystick;
	MuilEventUI *ui;
} MuilEvent;

struct MuilEventHandlerList {
	void (*handler)(MuilWidget *, unsigned int, MuilEvent *);
	unsigned int mask;
	struct MuilEventHandlerList *next;
};

typedef struct MuilEventHandler {
	void (*add)(MuilWidget *, void (*handler)(MuilWidget *, unsigned int, MuilEvent *), unsigned int);
	void (*remove)(MuilWidget *, void (*handler)(MuilWidget *, unsigned int, MuilEvent *), unsigned int);
	void (*send)(MuilWidget *, unsigned int, MuilEvent *);
	struct MuilEventHandlerList *handlers;
} MuilEventHandler;

MuilEventMouse muil_e_m_prev;

struct MuilEventHandlerList *muil_event_global_handlers;

void muil_events(struct MuilPaneList *panes, int render);
void muil_event_add(MuilWidget *widget, void (*handler)(MuilWidget *, unsigned int, MuilEvent *), unsigned int mask);
void muil_event_remove(MuilWidget *widget, void (*handler)(MuilWidget *, unsigned int, MuilEvent *), unsigned int mask);
void muil_event_send(MuilWidget *widget , unsigned int type, MuilEvent *e);

void muil_event_global_add(void (*handler)(MuilWidget *, unsigned int, MuilEvent *), unsigned int mask);
void muil_event_global_remove(void (*handler)(MuilWidget *, unsigned int, MuilEvent *), unsigned int mask);
void muil_event_global_send(unsigned int type, MuilEvent *e);

#endif
