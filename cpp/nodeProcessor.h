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

#ifndef NODEPROCESSOR_H
#define NODEPROCESSOR_H
#include "main.h"
#include "priorityPool.h"
#include "Thread.h"

class nodeProcessor:public Thread
{
public:

//	~nodeProcessor();
	nodeProcessor();

	nodeProcessor(int id,POWER_POOL * inPool)
        : inLink(inPool), nodeId(id)  {}
        int addOutLink(int parent,POWER_POOL * outPool);
        int addChildLink(POWER_POOL * childPool);
	int addChildLink(int child,POWER_POOL * outPool);
	int addChild(int child);
	virtual void run();
	void init();
	
	POWER_POOL * inLink;
	POWER_POOL * outLink;
	int nodeId,parentId,lastTs;
	std::map<int,POWER_POOL *> childLinks; // changed to pointer to avoid object cloning
	std::map<int,int > childs;

};

#endif // NODEPROCESSOR_H
