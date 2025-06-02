#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "termctl.h"
#include "dspelement.h"

unsigned int vertical_offset(struct dspelement *menu, unsigned int *i);
unsigned int vertical_offset_divider(struct dspelement *menu, unsigned int *i);
unsigned int vertical_count(struct dspelement *menu, unsigned int *i);
unsigned int vertical_count_divider(struct dspelement *i, unsigned int *i);
void init_element(struct dspelement *menu, unsigned int *i, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void init_element_divider(struct dspelement *menu, unsigned int *i, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void render_element(struct dspelement *menu, unsigned int *i);
void render_element_divider(struct dspelement *menu, unsigned int *i);

unsigned int vertical_offset(struct dspelement *menu, unsigned int *i) {
	switch(menu[*i].type) {
		case EL_DIVIDER:
			return vertical_offset_divider(menu, i);
		case EL_H_CAP:
			*i += 2;
			return menu[*i].arg.i;
		default:
			++(*i);
			return 0;
	}
}

unsigned int vertical_offset_divider(struct dspelement *menu, unsigned int *i) {
	unsigned int max = 0, this = *i, c;

	++(*i);
	while(*i < this + menu[this].arg.i) {
		c = vertical_offset(menu, i);
		if(c > max) max = c;
	}
	return max;
}

unsigned int vertical_count(struct dspelement *menu, unsigned int *i) {
	switch(menu[*i].type) {
		case EL_DIVIDER:
			return vertical_offset_divider(menu, i);
		case EL_H_CAP:
			*i += 2;
			return 0;
		default:
			++(*i);
			return 1;
	}
}

unsigned int vertical_count_divider(struct dspelement *menu, unsigned int *i) {
	unsigned int max = 0, this = *i, c;

	++(*i);
	while(*i < this + menu[this].arg.i) {
		c = vertical_count(menu, i);
		if(c > max) max = c;
	}
	return max;
}

void init_element(struct dspelement *menu, unsigned int *i, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	unsigned int j;

	switch(menu[*i].type) {
		case EL_DIVIDER:
			init_element_divider(menu, i);
			return;
		default:
			menu[*i].x = x;
			menu[*i].y = y;
			menu[*i].w = w;
			menu[*i].h = menu[*i].type == EL_H_CAP ? menu[*i].arg.i : h;
			menu[*i].data = NULL;
			++(*i);
			if(menu[*i].type == EL_H_CAP) {
				init_element(menu, i, x, y, w, menu[*i - 1].arg.i);
			}
			return;
	}
}

void init_element_divider(struct dspelement *menu, unsigned int *i, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	unsigned int j, this = *i, base_w = w / menu[this].arg.i, remainder = w % menu[this].arg.i, cx = x;
	
	menu[this].x = x;
	menu[this].y = y;
	menu[this].w = w;
	menu[this].h = h;
	menu[this].data = NULL;

	++(*i);
	while(*i < this + menu[this].arg.i) {
		init_element(menu, i, cx, y, base_w + (*i < this + remainder ? 1 : 0), h);
		cx += base_w + (*i < this + remainder ? 1 : 0);
	}
}

#endif

