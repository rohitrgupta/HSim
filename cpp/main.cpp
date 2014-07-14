#include "main.h"
#include "priorityPool.h"
#include "nodeProcessor.h"
#include "messageRouter.h"

int main(int argc, char **argv)
{
	printf("HSim \n");
	createNetwork();
	return 0;
}

void createNetwork(){
//	1	This function will load network from database
//	2	create the pipes
//	3	create threads
//	4	Create reader thread  
//	5	update thread with pipes
//	6	start threads and wait for them to finish
  
//	[1]
	std::vector<node> net;
	std::map<int,int> higherNodes;
	
	net = readNetwork();

//	[2][3]
//	itrate over each node in the "net" and create a pool for parent 
//	put these links into the links map
//	create processor thread for each parent using these links as "inLink"
	messageRouter r1;
	POWER_POOL *link1;
	std::map<int,POWER_POOL > links;
	nodeProcessor * processor;
	std::map<int,nodeProcessor> processors;
	
	// create the parent processors and the in links
	for (std::vector<node>::iterator it = net.begin() ; it != net.end(); ++it){
		if (links.count(it->parentID) == 0){
			print_debug(2,"Making link for %d\n",it->parentID);
			link1 = new POWER_POOL(50,50,it->parentID);
			links[it->parentID] = *link1;
			processor = new nodeProcessor(it->parentID,link1);
			processors[it->parentID] = *processor;
			higherNodes[it->parentID] = 1;
		}
		
	}

	for (std::vector<node>::iterator it = net.begin() ; it != net.end(); ++it){
		// add child to parent this should be done for all entries in the map
		processors[it->parentID].addChild(it->nodeID);
		
		// Setup the out link for all the nodes
		
		// if node is a parent and not root its output link will exist
		// roots entry do not exist in the net
		if (links.count(it->nodeID) != 0){
			print_debug(2,"link exist %d \n",it->nodeID);

			processors[it->nodeID].addOutLink(it->parentID,&links[it->parentID]);
			// also add reverse path
			processors[it->parentID].addChildLink(it->nodeID,&links[it->nodeID]);
			
		}
		// If node is not a parent add entry and parent's link to router 
		else{
			print_debug(2,"link absent %d \n",it->nodeID);
			// parent link might already be added
			r1.addTargetLink(it->parentID,&links[it->parentID]);
			r1.addTarget(it->nodeID,it->parentID);
		}
		
		
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
