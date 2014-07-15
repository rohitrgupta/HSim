#ifndef __POOL_H__
#define __POOL_H__

#include "Mutex.h"
#include <queue>
#include <vector>
#include <unistd.h>
#include <stdio.h>

#define POOL_MESSAGE 1

#define POWER_EVENT 100001
#define TS_EVENT 100002


#define print_pool(fmt, ...) \
            do { if (POOL_MESSAGE) {fprintf(stdout, fmt, __VA_ARGS__); }} while (0)

#define POOL_STAT print_pool("[Pool %d] count %d \n",(int)pool_id,data.size());		    
		    
struct powerEvent
{
    int id;
    int ts;
    float power;
    int eventType;
    int nextTs;
};


// following class is required for queue comparison
class CompareTs {
public:
    bool operator()(powerEvent& p1, powerEvent& p2)
    {
       if (p1.ts > p2.ts) return true;
       else if (p1.ts == p2.ts && p1.eventType > p2.eventType) return true;
       return false;
    }
};

		    

template <class T,class compareT> class Pool
{
private:
//    std::queue<T> data;
    std::priority_queue<T, std::vector<T>, compareT> data;

    Mutex mtx;
    size_t max_size;
    size_t spin_time;
public:
    int pool_id; 
    int wait_count,cont_wait_count;
    int e_count,cont_e_count;
    Pool(size_t max_size = 50, size_t spin_time = 50,int dest = -1)
        : max_size(max_size), spin_time(spin_time) ,pool_id(dest), wait_count(0),e_count(0)
    {print_pool("[Pool] Created %d \n",(int)pool_id);}
/*    Pool(size_t max_size = 50, size_t spin_time = 50,int dest = -1){
        max_size = max_size;
	spin_time =spin_time;
	pool_id = dest;
	wait_count = 0;
	e_count = 0;
	print_pool("[Pool] Created %d \n",(int)pool_id);
    }
*/    
    ~Pool()
    {
        mtx.lock();
        while (data.size()) data.pop();
        mtx.unlock();
    }
    size_t size()
    {
        mtx.lock();
        size_t result = data.size();
        mtx.unlock();
        if (result == 0){
            e_count ++;
            cont_e_count ++;
            if(e_count%10000 == 1){
                print_pool("[Pool %d] empty: count %d,%d \n",(int)pool_id,e_count,cont_e_count);
            }
        }
        return result;
    }
    void push(T item)
    {
        //printf("pushing: size = %d ",data.size());

        mtx.lock();
        while (data.size() >= max_size)
        {
            mtx.unlock();
            //if (pool_id > 300 && pool_id != 702 ){
                wait_count ++;
                cont_wait_count ++;
                if(wait_count%10000 == 1){
                    print_pool("[Pool %d] Full: count %d,%d \n",(int)pool_id,wait_count,cont_wait_count);
                }
            //}
            usleep(spin_time);
            mtx.lock();
        }
        cont_wait_count = 0;
        //printf("pushing \n");
        data.push(item);
        mtx.unlock();
	print_pool("[Pool %d] count %d \n",(int)pool_id,data.size());
    }

    T pop()
    {
        mtx.lock();
        while (data.size() <= 0)
        {
            mtx.unlock();
            e_count ++;
            cont_e_count ++;
            if(e_count%10000 == 1){
                print_pool("[Pool %d] empty: count %d,%d \n",(int)pool_id,e_count,cont_e_count);
            }
            usleep(spin_time);
            mtx.lock();
        }
        cont_e_count = 0;
        T item = data.top();
        data.pop();
        mtx.unlock();
	print_pool("[Pool %d] count %d \n",(int)pool_id,data.size());
        return item;
    }

    T pop(int eventType, int ts)
    {
        mtx.lock();
	int dataAvailable = 0; 
        while (dataAvailable <= 0)
        {
            if (data.size() > 0){
		T item = data.top();
		if (item.eventType <= eventType && item.ts == ts){
			dataAvailable =1;
			continue;
		}
	    }

            mtx.unlock();
            e_count ++;
            cont_e_count ++;
            if(e_count%10000 == 1){
                print_pool("[Pool %d] partial empty: count %d,%d \n",(int)pool_id,e_count,cont_e_count);
            }
            usleep(spin_time);
            mtx.lock();
        }
        cont_e_count = 0;
        T item = data.top();
        data.pop();
        mtx.unlock();
	print_pool("[Pool %d] count %d \n",(int)pool_id,data.size());
        return item;
    }

    std::vector<T> popAll()
    {
        mtx.lock();
        std::vector<T> items;
        while (data.size() > 0 )
        {

            T item = data.top();
            data.pop();
            items.push_back(item);
        }
        mtx.unlock();
	print_pool("[Pool %d] count %d \n",(int)pool_id,data.size());
        return items;
    }
    
    T front()
    {
        mtx.lock();
        while (data.size() <= 0)
        {
            mtx.unlock();
            e_count ++;
            cont_e_count ++;
            if(e_count%10000 == 1){
                print_pool("[Pool %d] empty count %d,%d \n",(int)pool_id,e_count,cont_e_count);
            }
            usleep(spin_time);
            mtx.lock();
        }
        cont_e_count = 0;
        T item = data.top();

        mtx.unlock();
        return item;
    }

};

#endif
