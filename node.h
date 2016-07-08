#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>
#include "network.h"
class Network;
class Packet; 
class Node{
	
	
	struct DV{
		int cost;
		int destination;
		int nextHop;
		DV(int c,int d, int n){ destination=d; cost=c; nextHop=n;}
		
	};
		struct neighbor{
		int neighid;
		double delay;
		int defaultCost;
		neighbor(int c, double d, int def){neighid=c;delay=d;defaultCost=def;}
		
	};	
public:
	Node();
	Node(int id);
	~Node();
	int nodeId;
	std::vector<DV> dvTable; //routing table
	std::vector<neighbor> neighborList;
	void addRoute(int src, int dest,int c);
	void updateRoute(int dest,int nextHop, int c);
	void addNeighbor(int id, double d, int c);
	int returnCost(int a, int b, Network net);
	void printNeighborList();
	int returnId(int a, int b, Network net);
	bool routeExists(int a,int b,Network net);
	void printRoutingTable();
	bool getsPacket(Packet pac,Network &net);

private:	
	
};


#endif