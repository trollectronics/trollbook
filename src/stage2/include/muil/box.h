/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#ifndef MUIL_BOX_H
#define MUIL_BOX_H

typedef struct MuilWidgetList {
	MuilWidget *widget;
	int expand;
	struct MuilWidgetList *next;
} MuilWidgetList;

#endif
