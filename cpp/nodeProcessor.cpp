/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#include "nodeProcessor.h"

nodeProcessor::nodeProcessor(){
}

int nodeProcessor::addOutLink(int parent,POWER_POOL * outPool){
	print_debug(2,"Adding parent link %d %d at node %d\n",parent,outPool->pool_id,nodeId);
	parentId = parent;
	if (outLink == NULL){
		outLink = outPool;
		outLinks[parentId] = outPool;
		return 0;
	}
	else{
		return -2;
	}
	return -1;
}

int nodeProcessor::addChildLink(int child, POWER_POOL * childPool){
	print_debug(2,"Adding child *link* %d at node %d \n",child,nodeId);

	if (childLinks.count(child) == 0){
		childLinks[child] = childPool;
		return 0;
	}
	else{
		print_error("Already exist%d\n",100003);			
		return -2;
	}
	return -1;
}

int nodeProcessor::addChild(int child){
	print_debug(2,"Adding child %d at node %d \n",child,nodeId);
	if (childs.count(child) == 0){  // the target is unknown 
		smData sm;
		childs[child] = 1;
		sm.id = child;
		sm.power = 0;
		sm.allocation = 0;
		powerData[child] = sm;
		return 0;
	}
	else{
		print_error("Already exist %d\n",100002);			
		return -2;
	}
	return -1;
}
void nodeProcessor::init()
{
    //initialize if required
    start();
}

void nodeProcessor::run()
{
	powerEvent t;
	int confirmCount=0;
	print_debug(3,"processcessor run: %d\n",nodeId);
	while(1)
	{
		/* Alternate algorithm for synchronisation
		 * processer will keep count of childs childs.count()
		 * it will keep count of TS_EVENT received for given lastTs int confirmCount
		 * 
		 * 
		 */
		
		//print_debug(3,"node %d read queue %d\n",nodeId,inLink->pool_id);
		if (confirmCount == 0){
			t = inLink->pop();
		}
		else{
			t = inLink->pop(TS_EVENT,lastTs);
		}
		print_debug(3,"node %d:received data sm %d ts %d power %f event %d\n",nodeId,t.id,t.ts,t.power,t.eventType);
		if (t.eventType == POWER_EVENT && powerData.count(t.id) > 0 ){
			powerData[t.id].power = t.power;
			lastTs = t.ts;
		}
		print_debug(5,"comparing %d %d\n",t.eventType,TS_EVENT);
		if ( t.eventType == TS_EVENT){
			confirmCount ++;
			print_debug(5,"confirm count %d %d\n",confirmCount ,childs.size());
			if (confirmCount >= childs.size()){
				confirmCount = 0;
				if (outLink != NULL ){
					powerEvent p;
					p.id = nodeId;
					p.eventType = POWER_EVENT;
					p.ts = lastTs;
					p.power = 0; // computation required
					for (std::map<int,smData>::iterator it = powerData.begin() ; it != powerData.end(); ++it){
						p.power += it->second.power;
					}
					if (nodeId != 0 ){
						print_debug(4,"node %d:sending data sm %d ts %d power %f to %d\n",nodeId,p.id,p.ts,p.power,outLink->pool_id);
						outLink->push(p);
						p.eventType = TS_EVENT;
						outLink->push(p);
					}
					else{
						print_debug(4,"node %d:ts %d total power %f \n",nodeId,p.ts,p.power);
						
					}
				}
				
				
			}
		}
	}
}
