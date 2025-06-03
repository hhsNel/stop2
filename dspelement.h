#ifndef DSPELEMENT_H
#define DSPELEMENT_H

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
	void (*input)(unsigned int x, unsigned int y, unsigned int w, unsigned int h, struct cchar **buffer);
	unsigned int x, y, w, h;
	void *data;
};

#endif

