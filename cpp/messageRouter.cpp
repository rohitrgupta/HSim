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

#include "messageRouter.h"

messageRouter::messageRouter(){

}

int messageRouter::addTarget(int sm,int node){
// This function should be called only if link is added	
	print_debug(2,"Adding Smart meter %d for parent %d\n",sm,node);
	if (targets.count(sm) == 0){  // the target is unknown 
		if (targetLinks.count(node) != 0){ // the link is already set
			targets[sm] = node;
			return 0;
		}
		else{
			print_error("Link should be set first for node %d\n",node);
			return -3;
		}
	}
	else{
		print_error("SM Already exist %d\n",sm);			
		return -2;
	}
	return -1;
}

int messageRouter::addTargetLink(int node,POWER_POOL * outPool){
	print_debug(2,"Adding link for parent %d \n",node);
	if (targetLinks.count(node) == 0){
		targetLinks[node] = * outPool;
		return 0;
	}
	else{
		print_error("Link Already exist for node %d\n",node);			
		return -2;
	}
	return -1;
}

void messageRouter::init()
{
    //initialize if required
    start();
}

void messageRouter::run()
{
	while(1)
	{
	}
}
