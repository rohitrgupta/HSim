//import org.freeplane.plugin.script.proxy.ScriptUtils
import java.sql.*
import org.sqlite.SQLiteDataSource
import groovy.sql.Sql

def static addPowerUp( c ) {
	def updated = 1
	while (updated == 1){
		updated = 0
		c.findAllDepthFirst().each { n ->
			if (!n.isLeaf()) { // leaf power should not be computed
				def sum = 0 
				def exist = 0
				exist = n.children.sum(0){ it['pDirty'].to.num0 }	
				if (exist > 0){
					updated = 1
					sum = n.children.sum(0){ it['P'].to.num0 }	
					n['P'] = sum
					n['pDirty'] = 1
					n.children.each { ch ->
						ch['pDirty'] = 0
					}
				}
			}
		}
	}
}

def static initPowerProcessing( c ) {
	c.findAllDepthFirst().each { n ->
	    def sum
	    if (n.isLeaf()) {
	        n['pDirty'] = 1
	    } else {
	        n['pDirty'] = 0
	    }
	}
}

def static loadInitFile( c ,fileName){ 
//def static loadInitFile( c ){ 	
	def dataList = []
//	def fileName = 'testdata.txt'
	def SMData = [:]

	File theInfoFile = new File(fileName)

	if (!theInfoFile.exists()) {
		println "File does not exist"
	} else {
		// Step through each line in the file
		theInfoFile.eachLine { line ->
			// If the line isn't blank
			if (line.trim()) {
				// Split into a key and value
				def(key, value) = line.split(':').collect {
					it.trim()
				}
			// and store them in the SMData Map
			SMData."$key" = value
			}	
		}
	}
	c.findAll().each {n -> 
		def sum, alloc
		if (n.isLeaf()) {
			if (SMData.containsKey(n.text)) {
				sum = SMData."$n.text"
				alloc = 0
			} else {
				sum = 0
				alloc = 0
			}
		} else {
			if (SMData.containsKey(n.text)) {
				alloc = SMData."$n.text"
			} else {
				alloc = 0
			}
		}
//		n['P'] = format(sum, '0.00')
//		n['A'] = format(alloc, '0.00')
		n['P'] = sum
		n['A'] = alloc
	//n['index'] = index++
	}
}

//////////////////////////////////////////////////////////////////
////////////////// Final version /////////////////////////////////
//////////////////////////////////////////////////////////////////


//def makeChildNode(node, id, name) {
//    def child = node.createChild()
//    child.text = name
//    child['node'] = id
//}

def static makeAllChildNode(c,node){
	def mapFile = c.getSelected().map.file
	def path = mapFile.parent
	def fname =  mapFile.name.lastIndexOf('.').with {it != -1 ? mapFile.name[0..<it] : mapFile.name}

	SQLiteDataSource ds = new SQLiteDataSource();
	ds.setUrl("jdbc:sqlite:" + path + "//" + fname + ".sqlite")

	def sql = new Sql(ds.getConnection())
	def query = "select * from net where netid = 1 and parent = " + node['node'] 
	//println query
	sql.eachRow(query) { 
	  //println("id=${it.node}, name= ${it.parent}")
//	  makeChildNode(node,it.node,it.name)
    def child = node.createChild()
    child.text = it.name
    child.style.setMaxNodeWidth(50)
    child['node'] = it.node

	}
	node.children.each { n ->
		makeAllChildNode(c,n)
	}
}

def static loadNetwork( c ) {
	def r = c.getSelected().map.root
	makeAllChildNode(c,r)
}


// clears all attibutes of all nodes
def static clear( c ) {
	c.findAllDepthFirst().each { n ->
        n.attributes.clear()
	}
}

// clears spacifide attibute of all nodes
def static clear( c,attrib ) {
	c.findAllDepthFirst().each { n ->
        n[attrib] = null
	}
}

// initialize data for root node
def static setRoot( c ) {
    def r = c.getSelected().map.root
   	r['NetID'] = 1
   	r['RunID'] = 1
   	r['ProfID'] = 1
   	r['Strat'] = 1
}

// This function will generate unique ID for each node.
// The root node is always assigne  ID 0
def static genID( c ) {
    def id = 0
	c.findAllDepthFirst().each { n ->
	    if (n.isRoot()) {
	    	n['node'] = 0
	    } else {
	    	id = id + 1
	    	n['node'] = id
	    }
	}
}

// This function will generate Name of the Node from its ID
def static genName( c ) {
	c.findAllDepthFirst().each { n ->
	    if (n.isRoot()) {
	    	n.text = 'Grid'
	    } else if (!n.children) {
	    	n.text = 'SM' + n['node']
	    }
	    else {
	    	n.text = 'D' + n['node']
	    }
	}
}

def static saveNetwork( c ) {

	def mapFile = c.getSelected().map.file
	def path = mapFile.parent
	def fname =  mapFile.name.lastIndexOf('.').with {it != -1 ? mapFile.name[0..<it] : mapFile.name}
	
	SQLiteDataSource ds = new SQLiteDataSource();
	ds.setUrl("jdbc:sqlite:" + path + "//" + fname + ".sqlite")

	def sql = new Sql(ds.getConnection())


	def net = c.getSelected().map.root['NetID'] 

	sql.execute("BEGIN TRANSACTION")
	sql.execute("DELETE from net where netid = " + net)
	def network = sql.dataSet("net")

	c.findAllDepthFirst().each { n ->
		if (n.isRoot()) {
			//network.add(netid:net,node:n['node'],parent:0)
		}
		else{
			//network.add(netid:net,node:n['node'],parent:n.parent['node'])
			sql.execute("INSERT INTO net values (" + net + "," + n['node'] + ',' +  n.parent['node'] + ",'" + n.text + "')" )
		}
	}
	sql.execute("COMMIT")
}

def static initDB( c ) {

	def mapFile = c.getSelected().map.file
	def path = mapFile.parent
	def fname =  mapFile.name.lastIndexOf('.').with {it != -1 ? mapFile.name[0..<it] : mapFile.name}
	
	SQLiteDataSource ds = new SQLiteDataSource();
	ds.setUrl("jdbc:sqlite:" + path + "//" + fname + ".sqlite")
	def sql = new Sql(ds.getConnection())

	sql.execute("CREATE TABLE net (netid int, node int, parent int, name text);")
	sql.execute("CREATE TABLE prof (profid int, node int, power float);")
	sql.execute("CREATE TABLE sys (runid int, seq int);")
	sql.execute("CREATE TABLE run (runid int, netid int, profid int,strat text);")
	sql.execute("CREATE TABLE log (runid int, desc text);")

}
