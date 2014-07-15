#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdarg.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define DEBUG_LEVEL 100

//char col1[10],col2[10];
#define print_debug(level,fmt, ...) \
            do { if (level <= DEBUG_LEVEL) {fprintf(stdout, "[Debug %d] %s | line %d | ",level,__FILE__,__LINE__); fprintf(stdout, fmt, __VA_ARGS__); }} while (0)

//int print_debug(int level,const char * fmt, ...);

#endif // COMMON_h