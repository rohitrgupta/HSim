#include "Reader1.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/time.h>

Reader1::Reader1()
{
    //ctor
    lastSlot = 0;
    lastTs = 0;

}

Reader1::~Reader1()
{
    //dtor
}

void Reader1::init()
{
    //initialize if required
    start();
}

void Reader1::run()
{
    //if (!routerLink ||!tickPool2 ) return;
	print_debug(3,"Reader run: %d\n",readerId);
	runFromFile();
}

int Reader1::send(std::string token){

    char *p,*q;
    char *s = (char*)token.c_str();
    FILE * pFile;

    p=s;
    q=p;
    int colNo =0,ts;
    float pow;
    char cc,plug_id;
	print_debug(3,"Data: %s\n",s);

    while (*q!= '#'){
        if(*q==','){
            *q = '\0';
            if(colNo == 0){
                ts = atoi(p);
            }
            if(colNo == 1){
                plug_id = atoi(p);
            }
            if(colNo == 2){
                pow = atof(p);
            }
            p=q+1;
            colNo = colNo+1;
        }
        q++;
    }
    if (colNo < 3){
	    print_error("data format error %d\n",10004);
        return -1;
    }
    powerEvent t1;
    //tick2 t2;
    //printf("%d\n",lastTs);
    t1.eventType = POWER_EVENT;
    t1.power = pow;
    t1.id =plug_id;
    t1.ts =ts;

//    if (ts > lastTs){
//        gettimeofday(&now, NULL);
//    }
    if (plug_id <= 100000 ){
        if (routerLink){
            routerLink->push(t1);
    }
        /*
        if (sendList != NULL){
            std::vector<Pool<tick2>*>::iterator it;

            for (it = sendList->begin();it != sendList->end(); ++it){
                print_debug("Sending delete record %d it %d\n",t2.ts,(*it)->pool_id);
                if (*it!= NULL){
                    (*it)->push(t2);
                }
            }

        }
        */
    }

/*
    if (ts != lastTs ){
        //printf("%d~",ts);
        if (lastTs != 0){
            pFile = fopen("log/receive_time.txt","a");
            fprintf (pFile, "%d,%d,%d,%d\n",ts-1,TS_TO_GSLOT(ts -1),now.tv_sec,now.tv_usec);
            fclose(pFile);
//            usleep(2000 * (ts - lastTs));
            if (TS_TO_SLOT(ts) != TS_TO_SLOT(ts-1)){
                print_debug("Data start for new slot %d \n",TS_TO_SLOT(ts));
            }
        }
        lastTs = ts;
    }

    int slot  = TS_TO_SLOT(ts);
    if (lastSlot != slot) {
        printf("Slot Changed to %d\n",slot);
        lastSlot = slot;
    }
   */
    //logData(t2.plug,t2.ts,t2.power);

}

void Reader1::runFromFile()
{
    char* data = 0;
    int i = 0;
    int size = 0;
    int sz = 1024*16;
    //std::ifstream is("p1.csv", std::ifstream::binary);
    std::ifstream is("data.txt", std::ifstream::binary);


    data = new char[sz+1];
    int read_sz = 0;
    int read_filesize = 0;


    std::string s ;
    std::string delimiter = "\n";

    size_t pos = 0;
    std::string token;
    if (is)
    {

        while (std::getline(is, s))
        {
		// Process str
		print_debug(11,"send called: %d\n",0);
		send(s.append(",#"));
        }
        /*

        is.seekg(0,std::ios::end);
        read_filesize= is.tellg();
        is.seekg(0,std::ios::beg);

        read_sz = is.tellg();
        while (!is.eof())
        {
            read_sz = is.tellg();
            int old_read_sz = read_sz;
            is.read(data, sz);
            read_sz = is.tellg();
            if (read_sz < 0){
                read_sz  = read_filesize;
            }
            print_debug(" data size so far: %d length %d \n",read_sz,read_sz-old_read_sz );
            data[read_sz-old_read_sz] = '\0';

            print_debug("%s\n-------------------------\n",data);
            s.append(data);
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                print_debug("split:%s\n", (char*)token.c_str());
                s.erase(0, pos + delimiter.length());
                send(token.append(",#"));
            }

        }
        */
        is.close();
        print_debug(3,"Send end signal to all threads: %d\n",read_sz);;
    }
}
/*
int Reader1::logData(plugId plg, TS ts,INDEX_WATT power)
{

    char sql[1000];
    if (sqlPool1 != NULL){
        sprintf(sql,"INSERT INTO power "  \
                "VALUES (%d, %d, %d, %d, %d);",plg.house,plg.household,plg.plug,ts,power);
        // Execute SQL statement
        print_message(" Pushing: %s\n",sql);
    //   string s = string(sql);
        sqlPool1->push(sql);
    }
    return 0;
}
*/
