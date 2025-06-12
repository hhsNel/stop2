#include <stdlib.h>

#include "cchar.h"
#include "dspelement.h"
#include "funcs.h"
#include "config.h"
#include "termctl.h"
#include "elements.h"
#include "menu.h"

int main(int argc, char **argv) {
	init_term();
	check_resolution();
	disable_wrapping();
	init_menu(scr_w, scr_h);
	update_sysinfo();
	render_elements();
	render_screen();
	flush_buffer();

	sleep(5);
	return 0;
}

