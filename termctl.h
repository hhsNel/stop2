#ifndef TERMCTL_H
#define TERMCTL_H

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "cchar.h"

#define ANSI_USR_F "\033[3%um"
#define ANSI_256_F "\033[38;5;%um"
#define ANSI_RGB_F "\033[38;2;%u;%u;%um"
#define ANSI_USR_B "\033[4%um"
#define ANSI_256_B "\033[48;5;%um"
#define ANSI_RGB_B "\033[48;2;%u;%u;%um"

static struct termios term;
static struct cchar **prev_screen;
static struct cchar **current_screen;
static unsigned int scr_w = 0, scr_h = 0;
static char output_buffer[OUT_BUFF_SIZE];
unsigned int out_size;

void hide_cursor();
void show_cursor();
void disable_wrapping();
void enable_wrapping();
void init_term();
void enter_immediate();
void enter_normal();
void resize_screen(unsigned int w, unsigned int h);
int check_resolution();
void move_cursor_to(unsigned int ln, unsigned int ix);
int color_equal(struct color c1, struct color c2);
void output_char(char c);
void output_color(struct color c, char *usrf, char *c256f, char *rgbf);
void render_screen_line(unsigned int ln);
void render_screen();
void render_char(unsigned int x, unsigned int y);
int line_diff(unsigned int ln);
void copy_to_previous_buff();
void flush_buffer();
void render_diff();
void set_cell(unsigned int x, unsigned int y, struct cchar c);
void reset_region(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

void hide_cursor() {
	strcpy(output_buffer + out_size, "\033[?25l");
	out_size += 6;
}

void show_cursor() {
	strcpy(output_buffer + out_size, "\033[?25h");
	out_size += 6;
}

void disable_wrapping() {
	strcpy(output_buffer + out_size, "\033[=7l");
	out_size += 5;
}

void enable_wrapping() {
	strcpy(output_buffer + out_size, "\033[=7h");
	out_size += 5;
}

void init_term() {
	unsigned int i, j;
	struct winsize w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	tcgetattr(STDIN_FILENO, &term);

	prev_screen = (struct cchar **)malloc(w.ws_col * sizeof(struct cchar *));
	current_screen = (struct cchar **)malloc(w.ws_col * sizeof(struct cchar *));
	for(i = 0; i < w.ws_col; ++i) {
		prev_screen[i] = (struct cchar *)malloc(w.ws_col * sizeof(struct cchar));
		current_screen[i] = (struct cchar *)malloc(w.ws_col * sizeof(struct cchar));
		for(j = 0; j < w.ws_row; ++j) {
			current_screen[i][j].c = BG_CHAR;
			current_screen[i][j].foreground = def_foreground;
			current_screen[i][j].background = def_background;
		}
	}
	scr_w = w.ws_col;
	scr_h = w.ws_row;
}

void enter_immediate() {
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	term.c_cc[VTIME] = 0;
	term.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void enter_normal() {
	term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void resize_screen(unsigned int w, unsigned int h) {
	unsigned int i, j;

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
		for(j = 0; j < h; ++j) {
			current_screen[i][j].c = BG_CHAR;
			current_screen[i][j].foreground = def_foreground;
			current_screen[i][j].background = def_background;
		}
	}
	scr_w = w;
	scr_h = h;
}

int check_resolution() {
	struct winsize w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	if(w.ws_col != scr_w || w.ws_row != scr_h) {
		resize_screen(w.ws_col, w.ws_row);
		render_screen();
		return 1;
	}
	return 0;
}

void move_cursor_to(unsigned int ln, unsigned int ix) {
	out_size += sprintf(output_buffer + out_size, "\033[%u;%uH", ln + 1, ix + 1);
}

int color_equal(struct color c1, struct color c2) {
	if(c1.color_mode != c2.color_mode) return 0;
	switch(c1.color_mode) {
		case CLRM_USR:
		case CLRM_256:
			if(c1.clr != c2.clr) return 0;
			break;
		case CLRM_RGB:
			if(c1.r != c2.r) return 0;
			if(c1.g != c2.g) return 0;
			if(c1.b != c2.b) return 0;
			break;
	}
	return 1;
}

void output_color(struct color c, char *usrf, char *c256f, char *rgbf) {
	switch(c.color_mode) {
		case CLRM_USR:
			out_size += sprintf(output_buffer + out_size, usrf, c.clr);
			break;
		case CLRM_256:
			out_size += sprintf(output_buffer + out_size, c256f, c.clr);
			break;
		case CLRM_RGB:
			out_size += sprintf(output_buffer + out_size, rgbf, c.r, c.g, c.b);
			break;
	}
}

void output_char(char c) {
	output_buffer[out_size] = c;
	++out_size;
	output_buffer[out_size] = '\0';
}

void render_screen_line(unsigned int ln) {
	unsigned int i;
	struct color prev_fg, prev_bg;

	move_cursor_to(ln, 0);
	prev_fg = current_screen[0][ln].foreground;
	prev_bg = current_screen[0][ln].background;
	output_color(prev_fg, ANSI_USR_F, ANSI_256_F, ANSI_RGB_F);
	output_color(prev_bg, ANSI_USR_B, ANSI_256_B, ANSI_RGB_B);
	output_char(current_screen[0][ln].c);
	for(i = 1; i < scr_w; ++i) {
		if(!color_equal(current_screen[i][ln].foreground, prev_fg)) {
			prev_fg = current_screen[i][ln].foreground;
			output_color(prev_fg, ANSI_USR_F, ANSI_256_F, ANSI_RGB_F);
		}
		if(!color_equal(current_screen[i][ln].background, prev_bg)) {
			prev_bg = current_screen[i][ln].background;
			output_color(prev_bg, ANSI_USR_B, ANSI_256_B, ANSI_RGB_B);
		}
		output_char(current_screen[i][ln].c);
	}
}

void render_screen() {
	unsigned int i;

	strcpy(output_buffer, "\033[2J");
	out_size = 4;

	for(i = 0; i < scr_h; ++i) {
		render_screen_line(i);
	}
	copy_to_previous_buff();
}

void render_char(unsigned int x, unsigned int y) {
	move_cursor_to(x, y);
	output_color(current_screen[x][y].foreground, ANSI_USR_F, ANSI_256_F, ANSI_RGB_F);
	output_color(current_screen[x][y].background, ANSI_USR_B, ANSI_256_B, ANSI_RGB_B);
	output_char(current_screen[x][y].c);
}

int line_diff(unsigned int ln) {
	unsigned int i, diff = -1;	/* return -1 when no changes found */

	for(i = 0; i < scr_w; ++i) {
		if(current_screen[i][ln].c != prev_screen[i][ln].c ||
			!color_equal(current_screen[i][ln].foreground, prev_screen[i][ln].foreground) ||
			!color_equal(current_screen[i][ln].background, prev_screen[i][ln].background)) {
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
	write(STDOUT_FILENO, output_buffer, out_size);
	fflush(stdout);
	output_buffer[0] = '\0';
	out_size = 0;
}

void render_diff() {
	unsigned int i, diff;

	for(i = 0; i < scr_h; ++i) {
		diff = line_diff(i);

		if(diff == -2) {
			render_screen_line(i);
		} else {
			if(diff <= -2) {
				render_char(diff, i);
			}
		}
	}
	copy_to_previous_buff();
}

void set_cell(unsigned int x, unsigned int y, struct cchar c) {
	current_screen[x][y] = c;
}

void reset_region(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
	unsigned int x, y;

	for(x = x1; x < x2; ++x) {
		for(y = y1; y < y2; ++y) {
			current_screen[x][y].c = BG_CHAR;
			current_screen[x][y].foreground = def_foreground;
			current_screen[x][y].background = def_background;
		}
	}
}

#endif

