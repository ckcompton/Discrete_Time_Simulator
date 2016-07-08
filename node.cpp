#include "node.h"
#include "packet.h"
using namespace std;
Node::Node(){

}
Node::Node(int id){
	nodeId=id;
}
Node::~Node(){}	
void Node::addRoute(int src, int dest,int c){
	dvTable.push_back(DV(src,dest,c));
	}
void Node::addNeighbor(int id, double d, int c){
	neighborList.push_back(neighbor(id,d,c));
}
void Node::printNeighborList(){
	cout<<"---------------------------------"<<endl;
	cout<<"NeighborList for Node"<<nodeId<<endl;
	for(int i=0; i<neighborList.size(); i++){
	cout<<neighborList[i].neighid<<endl;
	}
	cout<<"---------------------------------"<<endl;
}
void Node::printRoutingTable(){
	cout<<"---------------------------------"<<endl;
	cout<<"Routing Table for Node"<<nodeId<<endl;
	for(int i=0; i<dvTable.size(); i++){
	cout<<dvTable[i].destination<<","<<dvTable[i].cost<<","<<dvTable[i].nextHop<<endl;
	}
	cout<<"---------------------------------"<<endl;
}
 bool Node::getsPacket(Packet pac, Network &net){
	
	bool updatedRoutingTable=false;
	//cout<<pac.source<<"---->"<<pac.destination<<endl;
	
	for(int i=0; i<pac.routingInfo.size(); i++)
	{	
		//cout<<"i: "<<i<<endl;
		//for(int j=0; j<net.nodes[pac.destination].dvTable.size(); j++)
	//	{	//cout<<"j: "<<j<<endl;
			if(routeExists(pac.routingInfo[i].dest,pac.destination,net)) 
					{
						//find better cost
						//cout<<"if: "<<pac.routingInfo[i].dest<<" "<<pac.destination<<endl;
						//cout<<"possible need to update route: "<<endl;
						int tempCost=returnCost(pac.source,pac.destination,net);
						int tempIndex=returnId(pac.routingInfo[i].dest,pac.destination,net);
						//cout<<"Temp cost: "<<tempCost<<" Temp index "<<tempIndex<<endl;
						//cout<<pac.routingInfo[i].cost+tempCost<<" < "<<net.nodes[pac.destination].dvTable[tempIndex].cost<<endl;
						if(pac.routingInfo[i].cost+tempCost<net.nodes[pac.destination].dvTable[tempIndex].cost)
						{
							//cout<<"better solution found"<<endl;
							net.nodes[pac.destination].updateRoute(net.nodes[pac.destination].dvTable[tempIndex].destination,pac.source,tempCost+pac.routingInfo[i].cost);
							updatedRoutingTable=true;
							return updatedRoutingTable;
						}
						

					}
					else
					{
						//cout<<"if: "<<pac.routingInfo[i].dest<<" "<<pac.destination<<endl;
						//cout<<"Creating route: "<<endl;
						int tempCost=returnCost(pac.source,pac.destination,net);
						//cout<<pac.routingInfo[i].dest<<","<<pac.routingInfo[i].cost+tempCost<<","<<pac.source<<endl;
						net.nodes[pac.destination].addRoute(pac.routingInfo[i].cost+tempCost,pac.routingInfo[i].dest,pac.source);
						updatedRoutingTable=true;
						return updatedRoutingTable;
					}
			
				
			
	//	}
		//return updatedRoutingTable;		
		
	}
	
	
	
	
/*	bool updatedRoutingTable=false;
	for(int i=0; i<pac.routingInfo.size(); i++)  //check each destination for better route
	{
	cout<<"------------------"<<endl;
	cout<<"PacketSource: "<<pac.source<<endl;
	cout<<"Packet destination: "<<pac.destination<<endl;
	cout<<"Destination node routingTable size: "<<net.nodes[pac.destination].dvTable.size()<<endl;
	cout<<"packet routingTable size: "<<pac.routingInfo.size()<<endl;

	for(int m=0; m<pac.routingInfo.size(); m++)
				{
				cout<<"Destination: "<<pac.routingInfo[m].dest<<endl;
				cout<<"Cost: "<<pac.routingInfo[m].cost<<endl;
				cout<<endl;
				}
	//cout<<"net.nodes[pac.routingInfo[i].dest].dvTable[i].destination "<<net.nodes[pac.routingInfo[i].dest].dvTable[i].destination<<endl;
		cout<<"dest nodes routing table size: "<<net.nodes[pac.destination].dvTable.size()<<endl;
		for(int j=0; j<pac.routingInfo.size(); j++) //2
		{
			cout<<"\nChecking Route "<<j<<endl;
			cout<<"Packet:"<<pac.routingInfo[j].dest<<","<<pac.routingInfo[j].cost<<endl;
			//cout<<"net.nodes[pac.destination].dvTable.size() "<<net.nodes[pac.destination].dvTable.size()<<endl;
			//cout<<endl;		

			for(int k=0; k<net.nodes[pac.destination].dvTable.size(); k++)//3
			{
			
			
			
			int costToSource=0;
			cout<<"Node:  "<<net.nodes[pac.destination].dvTable[k].destination<<","<<net.nodes[pac.destination].dvTable[k].cost<<endl;
			cout<<pac.routingInfo[j].dest<<"=="<<net.nodes[pac.destination].dvTable[k].destination<<endl;
			cout<<"k: "<<k<<" j: "<<j<<endl;
			cout<<"net.nodes[pac.destination].dvTable[k].destination "<<net.nodes[pac.destination].dvTable[k].destination<<endl; 
				
				if(pac.routingInfo[j].dest != net.nodes[pac.destination].dvTable[k].destination) //if packet's routing entry is equal to destination's routing entry
				{ //both have routes to same node
					cout<<"made 1"<<endl;
					//find out the cost to that route
					for(int m=0; m<net.nodes[pac.source].dvTable.size(); m++)
					{
						if(net.nodes[pac.destination].dvTable[m].destination == pac.source)
						{
							costToSource = net.nodes[pac.destination].dvTable[m].cost;
							cout<<"Cost to source: "<<costToSource<<endl;
						}
					}
					cout<<pac.routingInfo[j].cost+costToSource<<" < "<<net.nodes[pac.destination].dvTable[k].cost<<endl;
					//check if cost is less
					//if(pac.routingInfo[j].cost + costToSource < net.nodes[pac.destination].dvTable[k].cost)
					//{	
					for(int a=0; a<net.nodes[pac.destination].dvTable.size(); a++)
					{
							if(pac.routingInfo[j].dest != net.nodes[pac.destination].dvTable[a].destination)
							{
								cout<<"Adding route "<<endl;
								cout<<"first if"<<endl;
								cout<<"net.nodes[pac.destination].dvTable.push_back(DV("<<pac.routingInfo[j].cost+costToSource<<","<<pac.routingInfo[j].dest<<","<<pac.source<<endl;
								net.nodes[pac.destination].addRoute(pac.routingInfo[j].cost+costToSource,pac.routingInfo[j].dest,pac.source);
								updatedRoutingTable=true;
							}
							else{
								if(pac.routingInfo[j].cost+costToSource < net.nodes[pac.destination].dvTable[a].cost)
								cout<<"Found a better route"<<endl;
								net.nodes[pac.destination].updateRoute(pac.routingInfo[j].dest,pac.source,pac.routingInfo[j].cost+costToSource);
								updatedRoutingTable=true;
							}
					}		
					
					
					//}
					
				} 
				//pac.routingInfo[j].dest != net.nodes[pac.destination].dvTable[k].destination
					//cout<<pac.routingInfo[j].dest<<"=="<<net.nodes[pac.destination].dvTable[k].destination<<endl;
					else
					{
						for(int b=0; b<pac.routingInfo.size(); b++)
						{	cout<<"k: "<<k<<" b: "<<b<<" j: "<<j<<endl;
							 cout<<pac.routingInfo[k].dest<<"=="<<net.nodes[pac.destination].dvTable[b].destination<<endl;
								if(pac.routingInfo[k].dest == net.nodes[pac.destination].dvTable[b].destination )
							{
									cout<<pac.routingInfo[k].dest<<"=="<<net.nodes[pac.destination].dvTable[b].destination<<endl;
							updatedRoutingTable=true;
							cout<<"adding route"<<endl;
								for(int m=0; m<net.nodes[pac.source].dvTable.size(); m++)
								{
										if(net.nodes[pac.destination].dvTable[m].destination == pac.source)
									{
										costToSource = net.nodes[pac.destination].dvTable[m].cost;
										cout<<"Cost to source: "<<costToSource<<endl;
									}
								}
							//net.nodes[pac.destination].dvTable.push_back(DV(pac.routingInfo[j].cost,pac.source,pac.routingInfo[j].dest));
							//cout<<"second if"<<endl;
							cout<<"net.nodes[pac.destination].dvTable.updating(DV("<<pac.routingInfo[j].cost+costToSource<<","<<pac.routingInfo[j].dest<<","<<pac.source<<endl;
							if(pac.routingInfo[j].cost+costToSource < net.nodes[pac.destination].dvTable[b].cost){
							net.nodes[pac.destination].updateRoute(pac.routingInfo[j].dest,pac.source,pac.routingInfo[j].cost+costToSource);
							}
							//net.nodes[pac.destination].printRoutingTable();
							//return 0;
							}
						}
					}
			}
		}
		return updatedRoutingTable;
		
		
		
	}
	*/
} 

void Node::updateRoute(int dest,int nextHop,int c){
	
	for(int i=0; i<dvTable.size(); i++){
		
		if(dvTable[i].destination == dest){
			
			dvTable[i].nextHop=nextHop;
			dvTable[i].cost=c;
			
		}
		
	}
	
	
}
bool Node::routeExists(int a,int b,Network net){ //if it exists then dont add it
	
	for(int i=0; i<net.nodes[b].dvTable.size(); i++)
	{
		if(a==net.nodes[b].dvTable[i].destination)
			return true;
		
	
	}
	return false;
	
}
int Node::returnCost(int a,int b,Network net){
	
	for(int i=0; i<net.nodes[b].dvTable.size(); i++)
	{
		if(a==net.nodes[b].dvTable[i].destination)
			return net.nodes[b].dvTable[i].cost;
		
	
	}
	
	
	return 0;
	
}
int Node::returnId(int a,int b,Network net){ 
	
	for(int i=0; i<net.nodes[b].dvTable.size(); i++)
	{
		if(a==net.nodes[b].dvTable[i].destination)
			return i;
		
	
	}
	
	
	return 0;
	
}