#include <list>
#include <fstream>
#include "network.h"
#include "packet.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
bool debug=false;
using namespace std;
double currentTime=0;
double lastUpdateTime=0;
int numberOfPackets=0;
Network::Network(){
}
Network::~Network(){}
void Network::initial(string fileName)
{

	//cout<<"Filename: "<<fileName<<endl;
	fstream fin;
	fin.open(fileName, std::fstream::in);
	
	int src;
	int dest;
	int cost;
	double delay;
	int numberOfNodes=0;

	if(!fin.is_open()) //check to see if file is open
		perror("file is not open"); 
	while(fin.good()) //read lines till the end of the file	
		{
			fin>>src;
			fin>>dest;
			fin>>cost;
			fin>>delay;
			if(src>numberOfNodes)
				numberOfNodes=src;
			if(dest>numberOfNodes)
				numberOfNodes=dest;
		}
		fin.seekg(0, fin.beg); //reset the file cursor to the beginning of the file
		for(int i=0; i<numberOfNodes+1; i++)
		nodes.push_back(Node()); 
	cout<<"number of Nodes: "<<numberOfNodes+1<<endl;
	while(fin.good()) //read lines till the end of the file	
		{
			fin>>src;
			fin>>dest;
			fin>>cost;
			fin>>delay;
			
			if(debug)
			{
				cout<<"\nSrc:"<<src<<endl;
				cout<<"Dest:"<<dest<<endl;
				cout<<"Cost:"<<cost<<endl;
				cout<<"Delay:"<<delay<<endl;
			}

			nodes[src].nodeId = src; //wont matter if it is already created
			nodes[src].addRoute(cost,dest,dest); //cost,destination,nexthop
			nodes[src].addNeighbor(dest,delay,cost);
			nodes[dest].nodeId = dest; //wont matter if it is already created
			nodes[dest].addRoute(cost,src,src); //cost,destination,nexthop
			nodes[dest].addNeighbor(src,delay,cost);
		
		}	
		//cout<<"Data filled"<<endl;



}
void Network::simulation(int runTime){

 		
		int nextUpdate=1;
		int k =0;
		int nodeIndex;
		
		
		/*
		for(int i=0;i<nodes.size();i++)
		{
		//nodes[i].printRoutingTable();
		//nodes[i].printNeighborList();
		}
		*/
		
		
		periodicUpdate();
 		cout<<"Initial event size: "<<events.size()<<endl;

		while(!events.empty() && currentTime<runTime)
		{
			
			
			if(events.front().arrivalTime>nextUpdate)
			{	
				currentTime=nextUpdate;
				cout<<"Periodic update at time "<<currentTime<<endl;
				periodicUpdate();
				nextUpdate++;
			}
			
			currentTime=events.front().arrivalTime;
			nodeIndex=events.front().destination;
			
			
			if(nodes[events.front().destination].getsPacket(events.front(),*this))
			{
				//sendDVpacket
				//cout<<"RoutingTable updated"<<endl;
				sendDvPacket(nodeIndex);
				lastUpdateTime=events.front().arrivalTime;
				
			}
			events.pop_front(); //take a packet off the list of events
		} 
		
		//Output information
		cout<<"current time: "<<currentTime<<endl;
		cout<<"last update time: "<<lastUpdateTime<<endl;
		cout<<"number of packets sent: "<<numberOfPackets<<endl;
		cout<<"last node to converge: "<<nodeIndex<<endl;
		
		//Output routing table and neighborlist
		for(int i=0;i<nodes.size();i++)
		{
		nodes[i].printRoutingTable();
		nodes[i].printNeighborList();
		}
}
void Network::periodicUpdate(){


	for(int i=0; i<nodes.size(); i++)
	{
		sendDvPacket(i);
		
	}

}

void Network::sendDvPacket(int id){
	int i=id;
	int tempTime;
	int tempDest;
	int packetSource;
	int packetDestination;
	double arrivalTime;
	double tempDelay;
	
	for(int a=0; a<nodes[i].neighborList.size(); a++)
	{
		Packet tempPacket;
		//cout<<"\nnewpacket"<<endl;
 		for( int j=0; j<nodes[i].dvTable.size(); j++) //size of routing table
		{	
			
			tempPacket.source=i; //source = nodeId
			tempPacket.destination=nodes[i].neighborList[a].neighid; //destination of packet = neighbor
			
			
			if(debug)
			{
			cout<<"Packet Source: "<<tempPacket.source<<endl; 
			cout<<"Packet Destination: "<<tempPacket.destination<<endl;//destination of packet = neighbor id
			cout<<"routing	nfo.size(): "<<nodes[i].dvTable.size()<<endl; //size of neighbors
			} 
			
			

				if( tempPacket.destination != nodes[i].dvTable[j].destination ) //split horizon
				{
				
				tempPacket.fillDvInfo(nodes[i].dvTable[j].cost,nodes[i].dvTable[j].destination); //insert into routing table 
				
					if(debug)
					cout<<"Filling info cost: "<<nodes[i].dvTable[j].cost<<" Destination: "<<nodes[i].dvTable[j].destination<<endl;
				
				
				}
				
			for(int b=0; b<nodes[i].neighborList.size();b++)
			{
				if(tempPacket.destination==nodes[i].neighborList[b].neighid)
					arrivalTime=nodes[i].neighborList[b].delay; //gets the delay from the nieghborslist
			}

			tempPacket.arrivalTime=currentTime+arrivalTime;
					
					if(debug)
					{
						cout<<"ArrivalTime: "<<arrivalTime<<endl;
						cout<<"Packet routing Table size: "<<tempPacket.routingInfo.size()<<endl;
						cout<<"Outputting packet routing Info information: "<<endl;
						for(int m=0; m<tempPacket.routingInfo.size(); m++)
						{
						cout<<"Destination: "<<tempPacket.routingInfo[m].dest<<endl;
						cout<<"Cost: "<<tempPacket.routingInfo[m].cost<<endl;
						cout<<endl;
						}
					}
			

				
		}
		
		if(debug)
		cout<<"pushing packet"<<endl;
	
	
		numberOfPackets++;
		events.push_back(tempPacket); //put the packet on the list of events	
	}
	
	
events.sort([](const Packet &a,const Packet &b) {return(a.arrivalTime < b.arrivalTime);});
 

}

int main(int argc, char **argv){
	
	Network network;
	cout<<"-----------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
	network.initial(argv[1]);
	network.simulation(atoi(argv[2]));
	cout<<"DONE"<<endl;
	return 0;
}