#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "termctl.h"
#include "dspelement.h"
#include "elementdisplay.h"

unsigned int vertical_offset(struct dspelement *menu, unsigned int *i);
unsigned int vertical_offset_divider(struct dspelement *menu, unsigned int *i);
unsigned int vertical_count(struct dspelement *menu, unsigned int *i);
unsigned int vertical_count_divider(struct dspelement *menu, unsigned int *i);
unsigned int init_element(struct dspelement *menu, unsigned int *i, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
unsigned int init_element_divider(struct dspelement *menu, unsigned int *i, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void delete_element(struct dspelement *menu, unsigned int *i);
void render_element(struct dspelement *menu, unsigned int *i);
void render_element_divider(struct dspelement *menu, unsigned int *i);
int is_selectable(struct dspelement el);
void interact_element(struct dspelement el);

void load_menu(struct dspelement *menu, unsigned int length);
void reset_selection();
void unload_menu();

unsigned int vertical_offset(struct dspelement *menu, unsigned int *i) {
	switch(menu[*i].type) {
		case EL_DIVIDER:
			return vertical_offset_divider(menu, i);
		case EL_H_CAP:
			*i += 2;
			return menu[*i - 2].arg.i;
		default:
			++(*i);
			return 0;
	}
}

unsigned int vertical_offset_divider(struct dspelement *menu, unsigned int *i) {
	unsigned int max = 0, this = *i, c;

	++(*i);
	while(*i <= this + menu[this].arg.i) {
		c = vertical_offset(menu, i);
		if(c > max) max = c;
	}
	return max;
}

unsigned int vertical_count(struct dspelement *menu, unsigned int *i) {
	switch(menu[*i].type) {
		case EL_DIVIDER:
			return vertical_count_divider(menu, i);
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
	while(*i <= this + menu[this].arg.i) {
		c = vertical_count(menu, i);
		if(c > max) max = c;
	}
	return max;
}

unsigned int init_element(struct dspelement *menu, unsigned int *i, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	unsigned int cache;

	switch(menu[*i].type) {
		case EL_DIVIDER:
			return init_element_divider(menu, i, x, y, w, h);
		default:
			menu[*i].x = x;
			menu[*i].y = y;
			menu[*i].w = w;
			menu[*i].h = menu[*i].type == EL_H_CAP ? menu[*i].arg.i : h;
			menu[*i].data = NULL;
			if(menu[*i].type == EL_H_CAP) {
				cache = menu[*i].arg.i;
				++(*i);
				init_element(menu, i, x, y, w, menu[*i - 1].arg.i);
				return cache;
			}
			++(*i);
			return h;
	}
}

unsigned int init_element_divider(struct dspelement *menu, unsigned int *i, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	unsigned int this = *i, base_w = w / menu[this].arg.i, remainder = w % menu[this].arg.i, cx = x, max = 0, cache;
	
	menu[this].x = x;
	menu[this].y = y;
	menu[this].w = w;
	menu[this].h = h;
	menu[this].data = NULL;

	++(*i);
	while(*i <= this + menu[this].arg.i) {
		cache = init_element(menu, i, cx, y, base_w + (*i < this + remainder ? 1 : 0), h);
		if(cache > max) max = cache;
		cx += base_w + (*i < this + remainder ? 1 : 0);
	}
	return max;
}

void delete_element(struct dspelement *menu, unsigned int *i) {
	if(menu[*i].data) {
		free(menu[*i].data);
	}
	menu[*i].x = menu[*i].y = menu[*i].w = menu[*i].h = 0;
	++(*i);
}

void render_element(struct dspelement *menu, unsigned int *i) {
	switch(menu[*i].type) {
		case EL_PLAINTEXT:
			display_plaintext(menu[*i]);
			break;
		case EL_VALUE:
			display_value(menu[*i]);
			break;
		case EL_DIVIDER:
			render_element_divider(menu, i);
			return;
		default:
			display_fallback(menu[*i]);
			break;
	}
	++(*i);
}

void render_element_divider(struct dspelement *menu, unsigned int *i) {
	unsigned int this = *i;

	++(*i);
	while(*i <= this + menu[this].arg.i) {
		render_element(menu, i);
	}
}

int is_selectable(struct dspelement el) {
	switch(el.type) {
		case EL_MENU_BUTTON:
			return 1;
		default:
			return 0;
	}
}

void interact_element(struct dspelement el) {
	switch(el.type) {
		case EL_MENU_BUTTON:
			unload_menu();
			load_menu(el.input, el.arg.i);
			reset_selection();
			break;
	}
}

#endif

