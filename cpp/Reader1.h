#ifndef READER1_H
#define READER1_H
#include "Thread.h"
#include "priorityPool.h"
#include "main.h"
#include "common.h"
#include <string>
using namespace std;

class Reader1 : public Thread
{
    public:
        Reader1();
        virtual ~Reader1();
        virtual void run();
        void runFromFile();
        void init();
        int send(std::string token);
        //int logData(plugId plg, TS ts,INDEX_WATT power);
        Reader1(int id,POWER_POOL * routerPool)
            : routerLink(routerPool), readerId(id)  {}
    protected:
    private:
        POWER_POOL * routerLink;
        int readerId;
        int lastTs;
        int lastSlot;
};

#endif // Reader1_H
