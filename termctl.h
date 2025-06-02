#ifndef TERMCTL_H
#define TERMCTL_H

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "cchar.h"

static struct termios term;
static struct cchar **prev_screen;
static struct cchar **current_screen;
static unsigned int scr_w = 0, scr_h = 0;
static char output_buffer[OUT_BUFF_SIZE];

void init_term();
void enter_immediate();
void enter_normal();
void resize_screen(unsigned int w, unsigned int h);
void check_resolution();
void move_cursor_to(unsigned int ln, unsigned int ix);
void render_screen_line(unsigned int ln);
void render_screen();
void render_char(unsigned int x, unsigned int y);
int line_diff(unsigned int ln);
void copy_to_previous_buff();
void flush_buffer();
void render_diff();
void set_cell(unsigned int x, unsigned int y, struct cchar c);

void init_term() {
	unsigned int i;
	struct winsize w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	tcgetattr(STDIN_FILENO, &term);

	prev_screen = (struct cchar **)malloc(w.ws_row * sizeof(struct cchar *));
	current_screen = (struct cchar **)malloc(w.ws_row * sizeof(struct cchar *));
	for(i = 0; i < w.ws_row; ++i) {
		prev_screen[i] = (struct cchar *)malloc(w.ws_col * sizeof(struct cchar));
		current_screen[i] = (struct cchar *)malloc(w.ws_col * sizeof(struct cchar));
	}
}

void enter_immediate() {
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void enter_normal() {
	term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void resize_screen(unsigned int w, unsigned int h) {
	unsigned int i;

	for(i = 0; i < scr_w; ++i) {
		free(prev_screen[i]);
		free(current_screen[i]);
	}
	free(prev_screen);
	free(current_screen);

	prev_screen = (struct cchar **)malloc(w * sizeof(struct cchar *));
	current_screen = (struct cchar **)malloc(w * sizeof(struct cchar *));
	for(i = 0; i < w; ++i) {
		prev_screen[i] = (struct cchar *)malloc(h * sizeof(struct cchar));
		current_screen[i] = (struct cchar *)malloc(h * sizeof(struct cchar));
	}
	scr_w = w;
	scr_h = h;
}

void check_resolution() {
	struct winsize w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	if(w.ws_row != scr_w || w.ws_col != scr_h) {
		resize_screen(w.ws_row, w.ws_col);
		render_screen();
	}
}

void move_cursor_to(unsigned int ln, unsigned int ix) {
	sprintf(output_buffer, "\033[%u;%uH", ln, ix);
}

void render_screen_line(unsigned int ln) {
	unsigned int i;
	unsigned char color_mode = -1;
	unsigned short int clr;
	unsigned short int r;
	unsigned short int g;
	unsigned short int b;

	move_cursor_to(ln, 0);
	for(i = 0; i < scr_w; ++i) {
		if(current_screen[i][ln].color_mode != color_mode) {
			color_mode = current_screen[i][ln].color_mode;
			switch(color_mode) {
				case CLRM_RGB:
					r = current_screen[i][ln].r;
					g = current_screen[i][ln].g;
					b = current_screen[i][ln].b;
					sprintf(output_buffer, "\033[38;2;%hu;%hu;%hum%c", r, g, b, current_screen[i][ln].c);
					break;
				case CLRM_256:
					clr = current_screen[i][ln].clr;
					sprintf(output_buffer, "\033[38;5;%hum%c", clr, current_screen[i][ln].c);
					break;
				case CLRM_USR:
					clr = current_screen[i][ln].clr;
					sprintf(output_buffer, "\033[3%hum%c", clr, current_screen[i][ln].c);
			}
		} else {
			switch(color_mode) {
				case CLRM_RGB:
					if(current_screen[i][ln].r != r || current_screen[i][ln].g != g || current_screen[i][ln].b != b) {
						r = current_screen[i][ln].r;
						g = current_screen[i][ln].g;
						b = current_screen[i][ln].b;
						sprintf(output_buffer, "\033[38;2;%hu;%hu;%hum%c", r, g, b, current_screen[i][ln].c);
					} else {
						sprintf(output_buffer, "%c", current_screen[i][ln].c);
					}
					break;
				case CLRM_256:
					if(current_screen[i][ln].clr != clr) {
						clr = current_screen[i][ln].clr;
						sprintf(output_buffer, "\033[38;5;%hum%c", clr, current_screen[i][ln].c);
					} else {
						sprintf(output_buffer, "%c", current_screen[i][ln].c);
					}
					break;
				case CLRM_USR:
					if(current_screen[i][ln].clr != clr) {
						clr = current_screen[i][ln].clr;
						sprintf(output_buffer, "\033[3%hum%c", clr, current_screen[i][ln].c);
					} else {
						sprintf(output_buffer, "%c", current_screen[i][ln].c);
					}
			}
		}
	}
}

void render_screen() {
	unsigned int i;

	for(i = 0; i < scr_h; ++i) {
		render_screen_line(i);
	}
	copy_to_previous_buff();
}

void render_char(unsigned int x, unsigned int y) {
	move_cursor_to(x, y);
	sprintf(output_buffer, "\033[38;2;%hu;%hu;%hum%c", 
		current_screen[x][y].r,
		current_screen[x][y].g,
		current_screen[x][y].b,
		current_screen[x][y].c);
}

int line_diff(unsigned int ln) {
	unsigned int i, diff = -1;	/* return -1 when one change found */

	for(i = 0; i < scr_w; ++i) {
		if(prev_screen[i][ln].c != current_screen[i][ln].c ||
			prev_screen[i][ln].color_mode != current_screen[i][ln].color_mode ||
			(prev_screen[i][ln].color_mode == CLRM_256 || prev_screen[i][ln].color_mode == CLRM_USR) && prev_screen[i][ln].clr !=
				current_screen[i][ln].clr ||
			prev_screen[i][ln].color_mode == CLRM_RGB && (prev_screen[i][ln].r != current_screen[i][ln].r || prev_screen[i][ln].g
				!= current_screen[i][ln].g || prev_screen[i][ln].b != current_screen[i][ln].b)) {
			if(diff == -1) {
				diff = i;
			} else {
				return -2;	/* return -2 when two+ changes found */
			}
		}
	}
	return diff;
}

void copy_to_previous_buff() {
	unsigned int i, j;

	for(i = 0; i < scr_w; ++i) {
		for(j = 0; j < scr_h; ++j) {
			prev_screen[i][j] = current_screen[i][j];
		}
	}
}

void flush_buffer() {
	write(STDOUT_FILENO, output_buffer, strlen(output_buffer));
	output_buffer[0] = '\0';
}

void render_diff() {
	unsigned int i, diff;

	for(i = 0; i < scr_h; ++i) {
		diff = line_diff(i);

		if(diff == -2) {
			render_screen_line(i);
		} else {
			if(diff >= 0) {
				render_char(diff, i);
			}
		}
	}
	copy_to_previous_buff();
}

void set_cell(unsigned int x, unsigned int y, struct cchar c) {
	current_screen[x][y] = c;
}

#endif

