#ifndef PACKET_H
#define PACKET_H
#include <iostream>
#include <vector>
class Packet{
	struct DVinfo
	{
		int dest;
		int cost;
		DVinfo(int d , int c ){ dest=d; cost=c; }
	};
public:
	Packet();
	~Packet();
	enum pType{ data , DV};
	int source;
	int destination;
	double arrivalTime;
	double sentTime;
	std::vector<DVinfo> routingInfo;
	void fillDvInfo(int c, int d);
	
private:	
	
};


#endif