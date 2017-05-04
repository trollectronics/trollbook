#ifndef _MENU_H_
#define _MENU_H_

#include <stdbool.h>

typedef struct MenuItem MenuItem;
struct MenuItem {
	const char *text;
	void (*func)(void *arg);
	void *arg;
};

typedef struct Menu Menu;
struct Menu {
	void (*header)(void *);
	void *arg;
	bool has_back;
	int selected;
	int items;
	MenuItem item[];
};

void menu_execute(void *menu);

#endif
