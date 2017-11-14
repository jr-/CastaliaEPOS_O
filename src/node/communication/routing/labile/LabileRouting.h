/***********************************************************************************************
 *  Copyright (c) Federal University of Para, brazil - 2012                                    *
 *  Developed at the Research Group on Computer Network and Multimedia Communication (GERCOM)  *
 *  All rights reserved                                                                        *
 *                                                                                             *
 *  Permission to use, copy, modify, and distribute this protocol and its documentation for    *
 *  any purpose, without fee, and without written agreement is hereby granted, provided that   *
 *  the above copyright notice, and the author appear in all copies of this protocol.          *
 *                                                                                             *
 *  Author(s): M. Butt et al.                                                                  *
 *  Reference Paper: link quality-based lexical routing metric for reactive routing protocols  *
 *  in ieee 802.15. 4 networks                                                                 *
 *  Implementarion: Thiago Fernandes <thiago.oliveira@itec.ufpa.br> <thiagofdso.ufpa@gmail.com>* 
 **********************************************************************************************/

#ifndef _LABILEROUTING_H_
#define _LABILEROUTING_H_

#include <list>
#include <map>
#include <iterable_queue.h>
#include "VirtualRouting.h"
#include "LabileRoutingPacket_m.h"
#include "LabileRoutingRrepPacket_m.h"
#include "LabileRoutingDataPacket_m.h"
#include "LabileTable_rt.h"
#include "PacketId_m.h"

using namespace std;

enum TimerIndex{
	LABILE_RREQ_BROADCAST_DROP_TIMER = 0,
	LABILE_RREQ_EXPIRATION_TIMER = 1,
	LABILE_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER = 2,
	LABILE_RREQ_RATE_LIMIT_TIMER = 3,
	LABILE_HELLO_MESSAGE_REFRESH_TIMER = 4,
	LABILE_HELLO_MESSAGE_EXPIRE_TIMER = 5,
	LABILE_RERR_RATE_LIMIT_TIMER = 6,
	LABILE_RREP_ACK_WAIT_TIMER = 7,
	LABILE_RREQ_BLACKLIST_REMOVE_TIMER = 8,
};

enum PacketTypes {
	LABILE_UNKNOWN_PACKET_TYPE = 0,
	LABILE_RREQ_PACKET = 1,
	LABILE_RREP_PACKET = 2,
	LABILE_RERR_PACKET = 3,
	LABILE_RREP_ACK_PACKET = 4,
	LABILE_HELLO_MESSAGE_PACKET = 5,
	LABILE_DATA_PACKET = 6,

};

struct RoutingTableEntryExpireTimerSet{
	double lifetime;
	string destination;
	bool canceled;
};

struct RoutingTableEntryLifetimeCompare{
	bool operator()(const RoutingTableEntryExpireTimerSet &a,
					const RoutingTableEntryExpireTimerSet &b){
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

class LabileRouting: public VirtualRouting {

 private:
	unsigned long currSeqNum; //sequence number of current node
	LabileTable* rt; //The routing table;
	int currRreqID; //next RreqID to be used in the RREQ packet

	map<string, int> rreqRetryCount;

	iterable_priority_queue <RoutingTableEntryExpireTimerSet,
					vector<RoutingTableEntryExpireTimerSet>,
					RoutingTableEntryLifetimeCompare > rtExpireSeq;
	//routing table entry lifetime update count
	map<string, int> rteluc;

	iterable_priority_queue <RreqExpireTimerSet,
					vector<RreqExpireTimerSet>,
					RreqExpireLifetimeCompare> rreqBroadcast;
	iterable_priority_queue <RreqExpireTimerSet,
					vector<RreqExpireTimerSet>,
					RreqExpireLifetimeCompare> rreqRequest;
	iterable_queue <LabileRREQPacket* > rreqBuffer;
	queue <LabileRERRPacket* > rerrBuffer;

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
	double lqiThreshould;
	int hopCountThreshould;
	void setRreqBoardcastDropTimer(const char* dest, const char* source, int id);
	void setRreqExpirationTimer(const char* dest);
	void setRrepAckTimer(const char* neib);
	void setRreqBlacklistTimer(const char* neib);

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
						bool isDestinationValid,RoutingFlag state,int hopCount, int weakLinks,
						const char* nextHopAddr,std::list<std::string>* precursor);
	void updateRTEntry(const char* destinationAddr,unsigned long destinationSeqNum,
						bool isDestinationValid,RoutingFlag state,int hopCount, int weakLinks,
						const char* nextHopAddr,std::list<std::string>* precursor,
						double lifetime);
	double getRemainingLifetimeTime(const char * dest);
	//update a lifetime of a VALID routing table entry (to no less then simTime() + time)
	void updateLifetimeEntry(const char * dest, double time);
	void resetLifetimeEntry(const char * dest);
	

 protected:
	void startup();
	void finish();
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
	void processLabileRreqPackage(LabileRREQPacket *,int srcMacAddress, double rssi, double lqi);
	void processLabileRrepPackage(LabileRREPPacket *,int srcMacAddress, double rssi, double lqi);
	void processLabileRerrPackage(LabileRERRPacket *,int srcMacAddress, double rssi, double lqi);
	void processDataPackage(LabileDataPacket*);
	void processHelloMsg(LabileHelloMessage*);
	void processPackagesInBuffer(const char *, bool drop);
	void processrreqBuffer();
	void timerFiredCallback(int index);
	void sendRreqRequest(int hopCount, int rreqID, const char* sourceAodv, const char* destAodv,
						unsigned long sSeq, unsigned long dSeq);
	void sendRerrMessage(list<string>* affDest, list<string>* affPrecur);
	void sendSingleRerrMessage(list<string>* affDest, list<string>* affPrecur, const char* dest);
	void sendHelloMessage();
	void sendRrepPacket(int hopCount, const char* orig, const char* destAodv, unsigned long dSeq, double lifetime, bool forwarding);
	void sendRrepAckPacket(const char* neib);
};

#endif
