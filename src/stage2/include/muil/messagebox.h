/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_MESSAGEBOX_H
void muil_messagebox(DrawFont *font, const char *text, const char *button_text);
void muil_messagebox_button_click(MuilWidget *widget, unsigned int type, MuilEvent *e);
#endif
