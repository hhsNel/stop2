#ifndef MENU_H
#define MENU_H

#include "dspelement.h"
#include "elements.h"

static struct dspelement *current_menu = main_menu;
static unsigned int current_length = sizeof(main_menu)/sizeof(*main_menu);
static unsigned int selected_element = 0;

#define SIMPLE_ITERATE(I,OPER,FUNC) \
	I = 0; \
	while(I < current_length) { \
		OPER FUNC(current_menu, &i); \
	}

void increment_selection();
void decrement_selection();
void load_menu(struct dspelement *menu, unsigned int length);
void reset_selection();
unsigned int total_offset();
unsigned int total_count();
void init_menu(unsigned int width, unsigned int height);
void unload_menu();
void handle_rendering(struct dspelement *menu, unsigned int *i);
void render_elements();
int menu_handle_input(char c);
int select_next();
int select_prev();
int go_down();
int go_up();
int interact();

void increment_selection() {
	++selected_element;
	selected_element %= current_length;
}

void decrement_selection() {
	selected_element += current_length - 1;
	selected_element %= current_length;
}

void load_menu(struct dspelement *menu, unsigned int length) {
	current_menu = menu;
	current_length = length;
}

void reset_selection() {
	selected_element = 0;
	if(!select_next()) {
		selected_element = -1;
	}
}

unsigned int total_offset() {
	unsigned int i, offset = 0;

	SIMPLE_ITERATE(i, offset += , vertical_offset);
	return offset;
}

unsigned int total_count() {
	unsigned int i, count = 0;

	SIMPLE_ITERATE(i, count +=, vertical_count);
	return count;
}

void init_menu(unsigned int width, unsigned int height) {
	unsigned int base_h, remainder, ct, i, h, cy = 0;

	height -= total_offset();
	ct = total_count();

	base_h = height / ct;
	remainder = height % ct;

	i = 0;
	while(i < current_length) {
		h = base_h + (i < remainder ? 1 : 0);
		cy += init_element(current_menu, &i, 0, cy, width, h);
	}
}

void unload_menu() {
	unsigned int i;

	SIMPLE_ITERATE(i, , delete_element);
}

void handle_rendering(struct dspelement *menu, unsigned int *i) {
	struct color orig_fg, orig_bg;

	if(*i == selected_element) {
		orig_fg = menu[*i].foreground;
		orig_bg = menu[*i].background;
		if(selected_foreground.color_mode != CLRM_UNSET) {
			menu[*i].foreground = selected_foreground;
		}
		if(selected_background.color_mode != CLRM_UNSET) {
			menu[*i].background = selected_background;
		}
		render_element(menu, i);
		menu[selected_element].foreground = orig_fg;
		menu[selected_element].background = orig_bg;
	} else {
		render_element(menu, i);
	}
}

void render_elements() {
	unsigned int i;

	SIMPLE_ITERATE(i, , handle_rendering);
}

int menu_handle_input(char c) {
	int i;

	for(i = 0; i < sizeof(keybinds)/sizeof(*keybinds); ++i) {
		if(c == keybinds[i].trigger) {
			return keybinds[i].action();
		}
	}

	return 1;
}

int select_next() {
	unsigned int start = selected_element;

	do {
		increment_selection();
		if(is_selectable(current_menu[selected_element])) {
			return 1;
		}
	} while(selected_element != start);

	return 0;
}

int select_prev() {
	unsigned int start = selected_element;
	
	do {
		decrement_selection();
		if(is_selectable(current_menu[selected_element])) {
			return 1;
		}
	} while(selected_element != start);

	return 0;
}

int go_down() {
	if(selected_element != -1) {
		select_next();
	}
	return 1;
}

int go_up() {
	if(selected_element != -1) {
		select_prev();
	}
	return 1;
}

int interact() {
	if(selected_element != -1) {
		interact_element(current_menu[selected_element]);
	}
	return 1;
}

#endif

