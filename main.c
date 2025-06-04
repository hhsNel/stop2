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
	init_menu(scr_w, scr_h);
	render_elements();
	render_screen();
	flush_buffer();
	return 0;
}

