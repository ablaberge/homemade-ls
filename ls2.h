#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define TRUE 1
#define FALSE 0
#define INDENT "    "

int parseMode(int argc);
void mode1(char *path, int depth);
int mode2(char *path, stack_t *s, int depth, char *pattern);


#endif
