#include "main.h"
#include "priorityPool.h"
#include "nodeProcessor.h"
#include "messageRouter.h"
#include "Reader1.h"

int main(int argc, char **argv)
{
	printf("HSim \n");
	createNetwork();
	return 0;
}

void createNetwork(){
//	change	status	desc
//	1	done	This function will load network from database
//	2	done	create the pipes
//	3	done	create threads
//	4	not	Create reader thread  
//	5	not	update thread with pipes
//	6	not	start threads and wait for them to finish
  
//	[1]
	std::vector<node> net;
	std::map<int,int> higherNodes;
	
	net = readNetwork();

//	[2][3]
//	itrate over each node in the "net" and create a pool for parent 
//	put these links into the links map
//	create processor thread for each parent using these links as "inLink"
	
	POWER_POOL *routerLink;
	routerLink = new  POWER_POOL(50,50,9999);
	
	messageRouter r1(9999,routerLink);
	Reader1 fr1(9998,routerLink);
	
	POWER_POOL *link1;
	std::map<int,POWER_POOL *> links; // changed to pointer to avoid object cloning
	nodeProcessor * processor;
	std::map<int,nodeProcessor> processors;
	
	// create the parent processors and the in links
	for (std::vector<node>::iterator it = net.begin() ; it != net.end(); ++it){
		if (links.count(it->parentID) == 0){
			print_debug(2,"Making link for %d\n",it->parentID);
			link1 = new POWER_POOL(50,50,it->parentID);
			links[it->parentID] = link1;
			processor = new nodeProcessor(it->parentID,link1);
			processors[it->parentID] = *processor;
			higherNodes[it->parentID] = 1;
			print_debug(2,"link for %d is %d\n",it->parentID,links[it->parentID]->pool_id);
		}
	}
	

	for (std::vector<node>::iterator it = net.begin() ; it != net.end(); ++it){
		// add child to parent this should be done for all entries in the map
		processors[it->parentID].addChild(it->nodeID);
		
		// Setup the out link for all the nodes
		
		// if node is a parent and not root its output link will exist
		// roots entry do not exist in the net
		if (links.count(it->nodeID) != 0){ // this is just a check 
			print_debug(2,"link exist %d parent %d linkid %d \n",it->nodeID,it->parentID,links[it->parentID]->pool_id);

			processors[it->nodeID].addOutLink(it->parentID,links[it->parentID]);
			// also add reverse path
			processors[it->parentID].addChildLink(it->nodeID,links[it->nodeID]);
			
		}
		// If node is not a parent add entry and parent's link to router 
		else{
			print_debug(2,"link absent %d \n",it->nodeID);
			// parent link might already be added
			r1.addTargetLink(it->parentID,links[it->parentID]);
			r1.addTarget(it->nodeID,it->parentID);
		}
		
		
	}	
	// call init for all threads
	print_debug(1,"-----------------Initialization complete------------------ %d \n",0);
	fr1.init();
	r1.init();

	//std::map<int,nodeProcessor> processors;
	//std::map<int,nodeProcessor>::iterator it;
	for (std::map<int,nodeProcessor>::iterator it = processors.begin() ; it != processors.end(); ++it){
		print_debug(1,"calling init for %d \n",it->first);
		it->second.init();
	}
	

	// call join for all threads
	fr1.join();
	r1.join();

	//std::map<int,nodeProcessor> processors;
	//std::map<int,nodeProcessor>::iterator it;
	for (std::map<int,nodeProcessor>::iterator it = processors.begin() ; it != processors.end(); ++it){
		it->second.join();
	}
	
}

static int nodeReader(void *data, int argc, char **argv, char **azColName){
	int i;
	node n;
	std::vector<node> * net;
	print_debug(1,"selected data col %d\n",argc);
	if (data != NULL)
	{
		net = (std::vector<node> *)data;
		//print_debug(" pool pointer %d: ", data);
	}
	else{
		print_error("Eoorr no net to load %d\n",90001);
	}
	for(i=0; i<argc; i++)
	{
		switch(i) {
		case 0 : 
			n.nodeID = atoi(argv[i]);
			break;
		case 1 :
			n.parentID = atoi(argv[i]);
			break;
		}
        print_debug(1,"%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	net->push_back(n);
	return 0;
}

std::vector<node> readNetwork(){
	std::vector<node> net;
	sqlite3 *db;
	int rc;
	char sql[5000];
	char *zErrMsg = 0;

	rc = sqlite3_open_v2("sim.sqlite", &db,SQLITE_OPEN_READWRITE , 0);
	if( rc )
	{
		print_error("Can't open database: %d\n", rc);
	}
	else
	{
		print_debug(1,"#############Opened database successfully %d\n",rc);
	}
	
	sprintf(sql,"SELECT node,parent from net where netid  = 1");
	print_debug(1, "%s\n",sql);

	rc = sqlite3_exec(db, sql, nodeReader, (void*)&net, &zErrMsg);
	print_debug(1,"r1: rc = %d\n",rc);
	if( rc != SQLITE_OK )
	{
                print_error("error  -- -- -- %d %s\n",rc,sql);
    //            print_error("SQL error: %s\n", zErrMsg);
    //            sqlite3_free(zErrMsg);
        }

	return net;
};
