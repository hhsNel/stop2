#include <stdlib.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>

#include "cchar.h"
#include "dspelement.h"
#include "funcs.h"
#include "key.h"
#include "keybinds.h"
#include "config.h"
#include "termctl.h"
#include "elements.h"
#include "menu.h"

int handle_input();
int quit();

int handle_input() {
	char c;
	struct pollfd input = {fd: STDIN_FILENO, events: POLLIN};

	if(poll(&input, 1, POLL_TIMEOUT) > 0 && input.revents & POLLIN) {
		c = getc(stdin);

		return menu_handle_input(c);
	}
	return 1;
}

int quit() {
	return 0;
}

int main(int argc, char **argv) {
	init_term();
	enter_immediate();
	check_resolution();
	disable_wrapping();
	hide_cursor();
	init_menu(scr_w, scr_h);
	reset_selection();
	update_sysinfo();
	render_elements();
	render_screen();
	flush_buffer();

	while(handle_input()) {
		update_sysinfo();
		render_elements();
		render_diff();
		flush_buffer();

		usleep(50000);
	}

	enter_normal();
	enable_wrapping();
	show_cursor();
	flush_buffer();

	return 0;
}

