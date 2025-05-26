#ifndef TERMCTL_H
#define TERMCTL_H

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>

#define CLRM_RGB 0
#define CLRM_256 1
#define CLRM_USR 2

struct cchar {
	char c;
	unsigned char color_mode;
	union {
		unsigned short int clr;
		struct {
			unsigned short int r;
			unsigned short int g;
			unsigned short int r;
		};
	};
};

static struct termios term;
static struct cchar **prev_screen;
static struct cchar **current_screen;
char *output_buffer[OUT_BUFF_SIZE];

void init_term();
void enter_immediate();
void enter_normal();
void resize_screen(unsigned int w, unsigned int h);
void render_screen_line(unsigned int ln);
void render_screen();
void render_char(unsigned int x, unsigned int y);
unsigned int line_diff(unsigned int ln);
void flush_buffer();
void render_diff();
void set_cell(unsigned int x, unsigned int y, cchar c);

void init_term() {
	tcgetattr(STDIN_FILENO, &term);
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

#endif

