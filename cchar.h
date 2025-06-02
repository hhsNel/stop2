#ifndef CCHAR_H
#define CCHAR_H

#define CLRM_RGB 0
#define CLRM_256 1
#define CLRM_USR 2

struct cchar {
	char c;
	unsigned char color_mode;
	unsigned short int clr;
	unsigned short int r;
	unsigned short int g;
	unsigned short int b;
};

#endif

