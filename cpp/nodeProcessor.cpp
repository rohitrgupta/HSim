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
	print_debug(2,"Adding parent link %d at node %d\n",parent,nodeID);
	parentID = parent;
	if (outLink != NULL){
		outLink = outPool;
		return 0;
	}
	else{
		return -2;
	}
	return -1;
}

int nodeProcessor::addChildLink(int child, POWER_POOL * childPool){
	print_debug(2,"Adding child *link* %d at node %d \n",child,nodeID);

	if (childLinks.count(child) == 0){
		childLinks[child] = * childPool;
		return 0;
	}
	else{
		print_error("Already exist%d\n",100003);			
		return -2;
	}
	return -1;
}

int nodeProcessor::addChild(int child){
	print_debug(2,"Adding child %d at node %d \n",child,nodeID);

	if (childs.count(child) == 0){  // the target is unknown 
		childs[child] = 1;
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
	while(1)
	{
	}
}