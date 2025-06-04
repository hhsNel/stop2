#ifndef ELEMENTDISPLAY_H
#define ELEMENTDISPLAY_H

#include "dspelement.h"
#include "termctl.h"

void write_text_in_bounds(char *str, struct cchar ex, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void display_plaintext(struct dspelement el);
void display_fallback(struct dspelement el);

void write_text_in_bounds(char *str, struct cchar ex, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	unsigned int cx, cy;

	for(cy = y; cy < y + h; ++cy) {
		for(cx = x; cx < x + w; ++cx) {
			if(! *str) return;
			ex.c = *str;
			set_cell(cx, cy, ex);
			++str;
		}
	}
}

void display_plaintext(struct dspelement el) {
	struct cchar ex;
	
	ex.color_mode = CLRM_USR;
	ex.clr = 7;
	write_text_in_bounds(el.arg.v, ex, el.x, el.y, el.w, el.h);
}

void display_fallback(struct dspelement el) {
	struct cchar ex;
	
	ex.color_mode = CLRM_USR;
	ex.clr = 7;
	write_text_in_bounds("FALLBACK", ex, el.x, el.y, el.w, el.h);
}

#endif

