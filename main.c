#include <stdlib.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>

#include "cchar.h"
#include "dspelement.h"
#include "funcs.h"
#include "config.h"
#include "termctl.h"
#include "elements.h"
#include "menu.h"

int handle_input();

int handle_input() {
	char c;
	struct pollfd input = {fd: STDIN_FILENO, events: POLLIN};

	if(poll(&input, 1, POLL_TIMEOUT) > 0 && input.revents & POLLIN) {
		c = getc(stdin);

		return menu_handle_input(c);
	}
	return 1;
}

int main(int argc, char **argv) {
	init_term();
	enter_immediate();
	check_resolution();
	disable_wrapping();
	init_menu(scr_w, scr_h);
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

	return 0;
}

