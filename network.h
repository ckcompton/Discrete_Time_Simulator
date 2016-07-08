#ifndef NETWORK_H
#define NETWORK_H
#include "node.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "packet.h"
class Node;

class Network{
	
public:
	Network();
	~Network();
	void initial(std::string fileName);
	void simulation(int runTime);
	void sendDvPacket(int id);
	void periodicUpdate();
	std::vector<Node> nodes;
	std::list<Packet> events;
	double time;
	
	
	private:	
	
};


#endif