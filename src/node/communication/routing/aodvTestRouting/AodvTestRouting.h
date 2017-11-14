/*
 * @Author: Mathieu MICHEL (http://w3.umons.ac.be/staff/Michel.Mathieu/)
 *
 * Copyright (C) 2013  Mathieu MICHEL
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef _AODVTESTROUTING_H_
#define _AODVTESTROUTING_H_

#include "VirtualRouting.h"
#include "AodvTestRoutingControlPacket_m.h"
#include "AodvTestRoutingDataPacket_m.h"
#include "AodvRoutingTable_rt.h"
#include "AodvRREQTable.h"
#include <list>
#include <map>
#include <iterable_queue.h>
#include <math.h>
#include <limits>



// types of packets
enum AODVPacketTypes
{
   	AODV_RREQ = 1,
	AODV_RREP = 2,
	AODV_RERR = 3,
	AODV_DATA = 4,
	AODV_HELLO = 5,
	AODV_RREP_ACK = 6,
	AODV_UNKNOWN = 0,

};

enum TimerIndex
{
	AODV_RREQ_BROADCAST_DROP_TIMER = 0,
	AODV_RREQ_EXPIRATION_TIMER = 1,
	AODV_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER = 2,
	AODV_RREQ_RATE_LIMIT_TIMER = 3,
	AODV_HELLO_MESSAGE_REFRESH_TIMER = 4,
	AODV_HELLO_MESSAGE_EXPIRE_TIMER = 5,
	AODV_RERR_RATE_LIMIT_TIMER = 6,
	AODV_RREP_ACK_WAIT_TIMER = 7,
	AODV_RREQ_BLACKLIST_REMOVE_TIMER = 8,
};

struct aodvTimer
{
    string destination;
    double lifetime;
};

struct rreqBroadcastedTimer
{
    int id;
    string origin;
    double lifetime;
};

struct compareRrepAckTimer
{
	bool operator()(const aodvTimer &a, const aodvTimer &b)
	{
		return a.lifetime > b.lifetime ;
	}
};

struct compareRreqBroadcastedTimer
{
    bool operator()(const rreqBroadcastedTimer &a, const rreqBroadcastedTimer &b)
    {
        return a.lifetime > b.lifetime ;
    }
};

class AodvTestRouting: public VirtualRouting 
{
	private:
	unsigned long currSN;
	int rreqID;
	AodvRoutingTable* rtable;// the routing table
	AodvRREQTable* rreqTable;// the RREQ table
	iterable_priority_queue <RouteTimer,vector<rreqBroadcastedTimer>,compareRreqBroadcastedTimer > rreqBroadTable;// list of rreq broadcasted


	double activeRouteTimeout; //in s
	int allowedHelloLoss;
	double helloInterval; //in s
	int localAddTTL;
	int netDiameter;
	double nodeTraversalTime; //in s
	double netTraversalTime;
	double pathDiscoveryTime;
	double frameSize;
	double rreqExpTime;
	double rreqExpTimeB;
	int rerrRatelimit;
	int rreqRetries;
	int rreqRatelimit;
	int timeoutBuffer;
	int ttlStart;
	int ttlIncrement;
	int ttlThreshould;

	bool shortestDelay;

    double latencyMax;
    double latencyMin;
    int latencyBuckets;

    int atmode;

	iterable_queue <PacketRREQ* > rreqBuffer;
	queue <PacketRERR* > rerrBuffer;

	map<std::string, int> rreqRetryCount;
	map<int, double> rreqSendTimes;//store the time at which a specific rreq (authenticated by an id) has been sent
	map<std::string, int> hmUpdateCount;
	map<std::string, int> routeUpdateCount;
    map<std::string, bool> rrepAcked;
    map<int,bool> rreqAnswered;



    //value used for the rreq response time calculation
    double respTimeMax;
    double respTimeMin;
    double respTimeTotal;
    double respTimeAverage;
    int rrepRxCount;//number of RREP received, for which the node is the RREQ originator

    //timers
    iterable_priority_queue <aodvTimer,vector<aodvTimer>,compareRrepAckTimer> rrepAck;
    iterable_priority_queue <aodvTimer,vector<aodvTimer>,compareRrepAckTimer> rreqBlacklist;
    priority_queue <aodvTimer,vector<aodvTimer>,compareRrepAckTimer> helloTimer;




	void updateLifetimeRoute(std::string destination, double time);
	void resetLifetimeRoute(const std::string dstIP);
	double getLifetimeRoute(const std::string dstIP);
	void updateRreqTable(std::string destination, int id);
	void updateRreqBroadcastedList(std::string destination, std::string source, int id);
	bool checkRREQProcessed(std::string dst);
	bool checkRREQBuffered(std::string orig, int idx);
	//check if the RREQ as already been forwarded (section 6.5 RFC3650)
	bool checkRREQBroadcasted(std::string orig, int idx);
	void updateRoute(const std::string dstIP,unsigned long dstSN,bool state,RoutingFlag flag,int hopCount,const std::string nextHopAddr,std::list<std::string>* precursor, double aTime);

	void setRrepAckTimer(const char* neib);
	void setRreqBlacklistTimer(const char* neib);



	protected:
	
	//call back methods
	void startup();
	void finish();
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
	void timerFiredCallback(int index);
	void receivePktDATA(PacketDATA* pkt);
	void receivePktRREQ(PacketRREQ* pkt,int srcMacAddress, double rssi, double lqi);
	void receivePktRREP(PacketRREP* pkt,int srcMacAddress, double rssi, double lqi);
	void receivePktRERR(PacketRERR* pkt,int srcMacAddress, double rssi, double lqi);
	void receivePktHELLO(PacketHELLO* pkt);

	//rreqSrc and rreqDst  are the value for all the AODV protocol : originator + finalDST
	void sendPktRREQ(int hopCount, int rreqID, std::string srcIP, std::string dstIP, unsigned long srcSN, unsigned long dstSN);
	void sendPktRREP(int hopCount, std::string rreqSrc, std::string rreqDst, unsigned long dstSN, double lifetime, bool forwarding, int rreqID);
	void sendPktHELLO();
	//affDst : list of broken dst - affPre : list of affected precursors
	void sendPktRERR(list<std::string>* affDst, list<std::string>* affPre);
	void sendPktRREPack(const char* neib);

	void processBufferedDATA(std::string dstIP, bool drop);
	void processBufferedRREQ();
	bool isBlacklisted(const char* neib);
	void setBlacklistTimer(const char* neib);
};

#endif
