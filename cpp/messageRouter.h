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

#ifndef MESSAGEROUTER_H
#define MESSAGEROUTER_H
#include "main.h"
#include "common.h"
#include "priorityPool.h"
#include "Thread.h"

class messageRouter:public Thread
{
public:
	messageRouter(int id,POWER_POOL * inPool)
        : inLink(inPool), routerId(id),lastTs(0)  {}
	int addTarget(int sm,int node);
	int addTargetLink(int node,POWER_POOL * outPool);
	virtual void run();
	void init();

	int routerId,lastTs;
	POWER_POOL * inLink;
	std::map<int,POWER_POOL *> targetLinks; // changed to pointer to avoid object cloning
	std::map<int,int > targets;

};

#endif // MESSAGEROUTER_H
