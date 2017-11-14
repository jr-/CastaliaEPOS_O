/***********************************************************************************************
 *  Copyright (c) Federal University of Para, brazil - 2012                                    *
 *  Developed at the Research Group on Computer Network and Multimedia Communication (GERCOM)  *
 *  All rights reserved                                                                        *
 *                                                                                             *
 *  Permission to use, copy, modify, and distribute this protocol and its documentation for    *
 *  any purpose, without fee, and without written agreement is hereby granted, provided that   *
 *  the above copyright notice, and the author appear in all copies of this protocol.          *
 *                                                                                             *
 *  Author(s): Kassio Machado <kassio.comp@gmail.com> ,                                        *
 *             Thiago Oliveira <thiagofdso.ufpa@gmail.com>                                     *
 *  Reference Paper: Design of a routing protocol using remaining energy and link quality indicator (REL)
 **********************************************************************************************/

#ifndef _RELROUTING_H_
#define _RELROUTING_H_

#include <list>
#include <map>
#include <iterable_queue.h>
#include "VirtualRouting.h"
#include "RELRoutingPacket_m.h"
#include "RELRoutingRrepPacket_m.h"
#include "RELRoutingDataPacket_m.h"
#include "RELTable_rt.h"
#include "PacketId_m.h"

using namespace std;

enum TimerIndex{
	REL_RREQ_BROADCAST_DROP_TIMER = 0,
	REL_RREQ_EXPIRATION_TIMER = 1,
	REL_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER = 2,
	REL_RREQ_RATE_LIMIT_TIMER = 3,
	REL_HELLO_MESSAGE_REFRESH_TIMER = 4,
	REL_HELLO_MESSAGE_EXPIRE_TIMER = 5,
	REL_RERR_RATE_LIMIT_TIMER = 6,
	REL_RREP_ACK_WAIT_TIMER = 7,
	REL_RREQ_BLACKLIST_REMOVE_TIMER = 8,
	REL_RADV_SEND_TIMER = 9,
};

enum PacketTypes {
	REL_UNKNOWN_PACKET_TYPE = 0,
	REL_RREQ_PACKET = 1,
	REL_RREP_PACKET = 2,
	REL_RERR_PACKET = 3,
	REL_RREP_ACK_PACKET = 4,
	REL_HELLO_MESSAGE_PACKET = 5,
	REL_DATA_PACKET = 6,
	REL_RADV_PACKET = 7,

};
struct RoutingTableEntryExpireTimerSet{
	double lifetime;
	string destination;
	bool canceled;
};

struct RoutingTableEntryExpireTimerSet2{
	double lifetime;
	string destination;
	string nextHop;
	bool canceled;
};

struct RoutingTableEntryLifetimeCompare{
	bool operator()(const RoutingTableEntryExpireTimerSet &a,
					const RoutingTableEntryExpireTimerSet &b){
		return a.lifetime > b.lifetime ;
	}
};

struct RoutingTableEntryLifetimeCompare2{
	bool operator()(const RoutingTableEntryExpireTimerSet2 &a,
					const RoutingTableEntryExpireTimerSet2 &b){
		return a.lifetime > b.lifetime ;
	}
};
struct RreqExpireTimerSet{
	bool canceled;
	int rreqID;
	string originator;
	string dest;
	double_t lifetime;
};

struct RreqExpireLifetimeCompare{
	bool operator()(const RreqExpireTimerSet &a,
					const RreqExpireTimerSet &b){
		return a.lifetime > b.lifetime ;
	}
};

class RELRouting: public VirtualRouting {

 private:
	unsigned long currSeqNum; //sequence number of current node
	RELTable* rt; //The routing table;
	int currRreqID; //next RreqID to be used in the RREQ packet

	map<string, int> rreqRetryCount;

	iterable_priority_queue <RoutingTableEntryExpireTimerSet2,
					vector<RoutingTableEntryExpireTimerSet2>,
					RoutingTableEntryLifetimeCompare2 > rtExpireSeq;

	//routing table entry lifetime update count
	map<string, map<string, int> > rteluc;

	iterable_priority_queue <RreqExpireTimerSet,
					vector<RreqExpireTimerSet>,
					RreqExpireLifetimeCompare> rreqBroadcast;
	iterable_priority_queue <RreqExpireTimerSet,
					vector<RreqExpireTimerSet>,
					RreqExpireLifetimeCompare> rreqRequest;
	iterable_queue <RELRREQPacket* > rreqBuffer;
	queue <RELRERRPacket* > rerrBuffer;

	//for hello message
	priority_queue <RoutingTableEntryExpireTimerSet,
					vector<RoutingTableEntryExpireTimerSet>,
					RoutingTableEntryLifetimeCompare > hmExpireSeq;
	map<string, int> hmeluc;

	//for unidirectional route discovery
	map<string, bool> rrepAcked;
	list<string> rreqBlacklist;
	iterable_priority_queue <RoutingTableEntryExpireTimerSet,
					vector<RoutingTableEntryExpireTimerSet>,
					RoutingTableEntryLifetimeCompare > rrepAckExpire;
	iterable_priority_queue <RoutingTableEntryExpireTimerSet,
					vector<RoutingTableEntryExpireTimerSet>,
					RoutingTableEntryLifetimeCompare > rreqBlacklistTimeout;


					
	//for lqi vector
	map<string,double> lqiMean;
	map<string,list<double>* > lqiVector;
	
	//neib energy
	map<string,int> neibEnergy;
	unsigned int lqiVectorSize;
	//parametres
	double activeRouteTimeout; //in s
	int allowedHelloLoss;  
	double helloInterval; //in s
	int localAddTTL;
	int netDiameter;
	double nodeTraversalTime; //in s
	int rerrRatelimit;
	int rreqRetries;
	int rreqRatelimit;
	int timeoutBuffer;
	int ttlStart;
	int ttlIncrement;
	int ttlThreshould;
	int energyThreshould;
	int currEnergy;
	double lqiThreshould;
	int hopCountThreshould;
	void setRreqBoardcastDropTimer(const char* dest, const char* source, int id);
	void setRreqExpirationTimer(const char* dest);
	void setRrepAckTimer(const char* neib);
	void setRreqBlacklistTimer(const char* neib);
	ResourceManager * resourceMng;
	//returns true if there is an on going rreq request for the dest
	bool isRreqRequstActive(const char* dest);

	bool isInRreqBlacklist(const char* dest);

	bool checkExpireEntry(const iterable_priority_queue <RreqExpireTimerSet, vector<RreqExpireTimerSet>, RreqExpireLifetimeCompare>* ,
							const char* orig, int idx);
	bool checkExpireEntry(const iterable_priority_queue <RreqExpireTimerSet, vector<RreqExpireTimerSet>, RreqExpireLifetimeCompare>* ,
							const char* dest);
	void cancelExpireEntry(iterable_priority_queue <RreqExpireTimerSet, vector<RreqExpireTimerSet>, RreqExpireLifetimeCompare>* ,
							const char* dest);

	//update the routing table and set the timer
	void updateRTEntry(const char* destinationAddr,unsigned long destinationSeqNum,
						bool isDestinationValid,RoutingFlag state,int hopCount, int weakLinks,int energy,
						const char* nextHopAddr,std::list<std::string>* precursor);
	void updateRTEntry(const char* destinationAddr,unsigned long destinationSeqNum,
						bool isDestinationValid,RoutingFlag state,int hopCount, int weakLinks,int energy,
						const char* nextHopAddr,std::list<std::string>* precursor,
						double lifetime);
	double getRemainingLifetimeTime(const char * dest,const char* nextHop);
	//update a lifetime of a VALID routing table entry (to no less then simTime() + time)
	void updateLifetimeEntry(const char * dest,const char * neib, double time);
	void resetLifetimeEntry(const char * dest);
	

 protected:
	void startup();
	void finish();
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
	void processRELRreqPackage(RELRREQPacket *,int srcMacAddress, double rssi, double lqi);
	void processRELRrepPackage(RELRREPPacket *,int srcMacAddress, double rssi, double lqi);
	void processRELRerrPackage(RELRERRPacket *,int srcMacAddress, double rssi, double lqi);
	void processRELRadvPackage(RELRADVPacket *,int srcMacAddress, double rssi, double lqi);
	void processDataPackage(RELDataPacket*);
	void processHelloMsg(RELHelloMessage*);
	void processPackagesInBuffer(const char *, bool drop);
	void processrreqBuffer();
	void timerFiredCallback(int index);
	void sendRreqRequest(int hopCount, int rreqID, const char* sourceAodv, const char* destAodv,
						unsigned long sSeq, unsigned long dSeq,int energy);
	void sendRerrMessage(list<string>* affDest, list<string>* affPrecur);
	void sendSingleRerrMessage(list<string>* affDest, list<string>* affPrecur, const char* dest);
	void sendHelloMessage();
	void sendRrepPacket(int hopCount, const char* orig, const char* destAodv, unsigned long dSeq, double lifetime, bool forwarding,int energy);
	void sendRrepAckPacket(const char* neib);
	void sendRadvPacket();
	void lqiUpdate(string neib, double lqi);
	bool updateSelfPercentualEnergy();
	void updateNeibPercentualEnergy(string neib,int energy);
	int increaseLinkCost(int lqi,int linkCost);
	int getMaxNeibEnergy();
	int getMinNeibEnergy();	
};

#endif
