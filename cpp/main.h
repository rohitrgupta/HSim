#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <vector>
#include <map>
#include <sqlite3.h>
#include <stdlib.h>

#define DEBUG_LEVEL 10
#define POWER_POOL Pool<powerEvent,CompareTs>

#define print_debug(level,fmt, ...) \
            do { if (level <= DEBUG_LEVEL) {fprintf(stdout, "[Debug %d]%s | line %d | ",level,__FILE__,__LINE__); fprintf(stdout, fmt, __VA_ARGS__); }} while (0)

#define print_error(fmt, ...) \
            do { fprintf(stderr, "[Error]|%s|%d|",__FILE__,__LINE__); fprintf(stderr, fmt, __VA_ARGS__); } while (0)

		    
struct node
{
    int nodeID;
    int parentID;
};

struct powerEvent
{
    int id;
    int ts;
    float power;
    bool flgTsEnd;
    int nextTs;
};

// following class is required for queue comparison
class CompareTs {
public:
    bool operator()(powerEvent& p1, powerEvent& p2)
    {
       if (p1.ts > p2.ts) return true;
       return false;
    }
};

void createNetwork();

std::vector<node> readNetwork();

#endif // MAIN_H