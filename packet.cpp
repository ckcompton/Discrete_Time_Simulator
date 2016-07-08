#include "packet.h"
using namespace std;

Packet::Packet(){
	
}
Packet::~Packet(){}	
void Packet::fillDvInfo(int c, int d){
	if(destination!=c){
	routingInfo.push_back(DVinfo(d,c));
	}
	
	
}