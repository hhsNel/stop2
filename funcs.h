#ifndef FUNCS_H
#define FUNCS_H

#include "dspelement.h"

#include <stdint.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

typedef int64_t (*value_function)(struct argument arg, void **data);
static struct sysinfo info;

void update_sysinfo();
int64_t vcpu_load(struct argument arg, void **data);
int64_t vram_max(struct argument arg, void **data);
int64_t vswap_max(struct argument arg, void **data);
int64_t vram_total(struct argument arg, void **data);
int64_t vswap_total(struct argument arg, void **data);

void update_sysinfo() {
	sysinfo(&info);
}

int64_t vcpu_load(struct argument arg, void **data) {
	return 4;
}

int64_t vram_max(struct argument arg, void **data) {
	unsigned long long mem = info.totalram;

	return mem * info.mem_unit;
}

int64_t vswap_max(struct argument arg, void **data) {
	unsigned long long swap = info.totalswap;

	return swap * info.mem_unit;
}

int64_t vram_total(struct argument arg, void **data) {
	unsigned long long mem = info.totalram - info.freeram;

	return mem * info.mem_unit;
}

int64_t vswap_total(struct argument arg, void **data) {
	long long mem = info.totalswap - info.freeswap;

	return mem * info.mem_unit;
}

#endif

