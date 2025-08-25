#ifndef ARGSHELPER_H
#define ARGSHELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void update_min_count_value(const char *instr, int *x);
void update_threshold_count_value(const char *instr, float *x);
void parseArgs(int argc, char **argv, int *min_count, float* threshold);

#endif // ARGSHELPER_H
