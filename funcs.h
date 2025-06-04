#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>

void vcpu_load(unsigned int x, unsigned int y, unsigned int w, unsigned int h, struct cchar **buffer);
void vram_max(unsigned int x, unsigned int y, unsigned int w, unsigned int h, struct cchar **buffer);
void vram_total(unsigned int x, unsigned int y, unsigned int w, unsigned int h, struct cchar **buffer);

void vcpu_load(unsigned int x, unsigned int y, unsigned int w, unsigned int h, struct cchar **buffer) {
	printf("NOT IMPLEMENTED\n");
}

void vram_max(unsigned int x, unsigned int y, unsigned int w, unsigned int h, struct cchar **buffer) {
	printf("NOT IMPLEMENTED\n");
}

void vram_total(unsigned int x, unsigned int y, unsigned int w, unsigned int h, struct cchar **buffer) {
	printf("NOT IMPLEMENTED\n");
}

#endif

