#ifndef KEY_H
#define KEY_H

struct key {
	char trigger;
	int (*action)();
};

#endif

