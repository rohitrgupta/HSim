#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <vector>
#include <map>
#include <sqlite3.h>
#include <stdlib.h>


#define POWER_POOL Pool<powerEvent,CompareTs>

//#define print_debug(level,fmt, ...) \
//            do { if (level <= DEBUG_LEVEL) {fprintf(stdout, "\x1B[32m[Debug %d]\x1B[0m%s | line %d | ",level,__FILE__,__LINE__); fprintf(stdout, fmt, __VA_ARGS__); }} while (0)
// if (isatty (1))
	
//#define print_debug(level,fmt, ...) \
//            do { if (level <= DEBUG_LEVEL) {fprintf(stdout, "[Debug %d]%s | line %d | ",level,__FILE__,__LINE__); fprintf(stdout, fmt, __VA_ARGS__); }} while (0)

#define print_error(fmt, ...) \
            do { fprintf(stderr, "[Error]%s|%d|",__FILE__,__LINE__); fprintf(stderr, fmt, __VA_ARGS__); } while (0)


struct node
{
    int nodeID;
    int parentID;
};

struct smData
{
    int id;
    float power;
    float allocation;
    // more to come
};

void createNetwork();

std::vector<node> readNetwork();

#endif // MAIN_H