#ifndef _FILEBROWSE_H
#define _FILEBROWSE_H

#include <stdbool.h>
#include "menu.h"

extern Menu menu_dir;

void execute_elf_path(const char *_path, bool debug);

#endif
