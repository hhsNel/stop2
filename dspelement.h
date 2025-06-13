#ifndef DSPELEMENT_H
#define DSPELEMENT_H

#include "cchar.h"

#define EL_PLAINTEXT 0
#define EL_MENU_BUTTON 1
#define EL_GRAPH_OVER_TIME 2
#define EL_DIVIDER 3
#define EL_VALUE 4
#define EL_H_CAP 5

struct argument {
	int i;
	void *v;
};

struct dspelement {
	int type;
	struct argument arg;
	void *input;
	struct color foreground;
	unsigned int x, y, w, h;
	void *data;
};

#endif

