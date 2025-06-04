#ifndef MENU_H
#define MENU_H

static struct dspelement *current_menu = main_menu;
static unsigned int current_length = sizeof(main_menu)/sizeof(*main_menu);

#define SIMPLE_ITERATE(I,OPER,FUNC) \
	I = 0; \
	while(I < current_length) { \
		OPER FUNC(current_menu, &i); \
	}

void load_menu(struct dspelement *menu, unsigned int length);
unsigned int total_offset();
unsigned int total_count();
void init_menu(unsigned int width, unsigned int height);
void unload_menu();
void render_elements();

void load_menu(struct dspelement *menu, unsigned int length) {
	current_menu = menu;
	current_length = length;
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
		init_element(current_menu, &i, 0, cy, width, h);
		cy += h;
	}
}

void unload_menu() {
	unsigned int i;

	SIMPLE_ITERATE(i, , delete_element);
}

void render_elements() {
	unsigned int i;

	SIMPLE_ITERATE(i, , render_element);
}

#endif

