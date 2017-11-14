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

#include "RELRouting.h"
#include <csimulation.h>

Define_Module(RELRouting);

void RELRouting::startup()
{
	//parametres
	activeRouteTimeout = (double)par("activeRouteTimeout") / 1000.0;
	allowedHelloLoss = par("allowedHelloLoss");
	helloInterval = (double)par("helloInterval") / 1000.0;
	localAddTTL = par("localAddTTL");
	netDiameter = par("netDiameter");
	nodeTraversalTime = (double)par("nodeTraversalTime") / 1000.0;
	rerrRatelimit = par("rerrRatelimit");
	rreqRetries = par("rreqRetries");
	rreqRatelimit = par("rreqRatelimit");
	timeoutBuffer = par("timeoutBuffer");
	ttlStart = par("ttlStart");
	ttlIncrement = par("ttlIncrement");
	ttlThreshould = par("ttlThreshould");
	energyThreshould = par("energyThreshould");
	lqiThreshould = par("lqiThreshould");
	lqiVectorSize = par("lqiVectorSize");
	hopCountThreshould = par("hopCountThreshould");
	
	currSeqNum = 1; 
	currRreqID = 0; 
		
	rt = new RELTable();
	resourceMng = check_and_cast <ResourceManager*>(getParentModule()->getParentModule()->getSubmodule("ResourceManager"));
	currEnergy = 100;
	setTimer(REL_RADV_SEND_TIMER, 1);
}

void RELRouting::finish(){
	VirtualRouting::finish();
	RELRREQPacket* rreqpkt;
	RELRERRPacket* rerrpkt;
	// clear the buffer
	while (!rreqBuffer.empty()) {
		rreqpkt = rreqBuffer.front();
		rreqBuffer.pop();
		cancelAndDelete(rreqpkt);
	}
	while (!rerrBuffer.empty()) {
		rerrpkt = rerrBuffer.front();
		rerrBuffer.pop();
		cancelAndDelete(rerrpkt);
	}
	delete rt;
}

void RELRouting::setRreqBoardcastDropTimer(const char* dest, const char* source, int id){
	if(checkExpireEntry(&rreqBroadcast, source, id)){
		return;
	}
	RreqExpireTimerSet rets;
	rets.canceled = false;
	rets.dest = string(dest);
	rets.lifetime = simTime().dbl() + 2 * nodeTraversalTime * 1000; //PATH_DISCOVERY_TIME = 2 * NODE_TRAVERSAL_TIME
	rets.originator = string(source);
	rets.rreqID = id;
	//if the first to expire changes
	if(!rreqBroadcast.empty() && rets.lifetime < rreqBroadcast.top().lifetime){
		
		cancelTimer(REL_RREQ_BROADCAST_DROP_TIMER);
	}
	rreqBroadcast.push(rets);
	if(getTimer(REL_RREQ_BROADCAST_DROP_TIMER).dbl()<=0){
		setTimer(REL_RREQ_BROADCAST_DROP_TIMER, rreqBroadcast.top().lifetime - simTime().dbl());
	}

}

bool RELRouting::isInRreqBlacklist(const char* dest){
    for(list<string>::const_iterator i=rreqBlacklist.begin();i!=rreqBlacklist.end();++i){
        string s = *i;
        if(s.compare(dest)==0){
            return true;
        }
    }
    return false;
}

bool RELRouting::checkExpireEntry(const iterable_priority_queue <RreqExpireTimerSet, vector<RreqExpireTimerSet>, RreqExpireLifetimeCompare>* exq,
								const char* dest){
    for(const auto & i : *exq)
        if(i.dest.compare(dest) == 0)
            return true;
    return false;
//    if(!(exq->empty())){
//         auto tmp = *exq;
//         while(!tmp.empty()) {
//             if(tmp.top().dest.compare(dest) == 0)
//                 return true;
//             tmp.pop();
//         }
//     }
//     return false;
//	const RreqExpireTimerSet* q0;
//	if(!(exq->empty())){
//		q0 = &(exq->top());
//		for(unsigned int i = 0; i < exq->size(); i++){
//			if(q0[i].dest.compare(dest)==0){
//				return true;
//			}
//		}
//	}
//	return false;
}

void RELRouting::cancelExpireEntry(iterable_priority_queue <RreqExpireTimerSet, vector<RreqExpireTimerSet>, RreqExpireLifetimeCompare>* exq,
								const char* dest){
    for(auto&& i : *exq) {
        if(i.dest.compare(dest) == 0) {
            i.canceled = true;
            return;
        }
    }
//    priority_queue <RreqExpireTimerSet, vector<RreqExpireTimerSet>, RreqExpireLifetimeCompare> tmp;
//    while(!exq->empty()) {
//        auto q0 = exq->top();
//        if(q0.dest.compare(dest)==0) {
//            q0.canceled = true;
//            tmp.push(q0);
//            exq->pop();
//            break;
//        }
//        tmp.push(q0);
//        exq->pop();
//    }
//    exq->swap(tmp);

//	RreqExpireTimerSet* q0;
//		if(!(exq->empty())){
//			q0 = (RreqExpireTimerSet*)&(exq->top());
//			for(unsigned int i = 0; i < exq->size(); i++){
//				if(q0[i].dest.compare(dest)==0){
//					q0[i].canceled = true;
//					return;
//				}
//			}
//		}
//		return;
}

void RELRouting::setRrepAckTimer(const char* neib){
    for(const auto & i : rrepAckExpire) {
        if(i.destination.compare(neib) == 0)
            return;
    }
//    //check if is already waiting for the neighbor to ack
//         if(!(rrepAckExpire.empty())){
//             auto tmp = rrepAckExpire;
//             while(!tmp.empty()) {
//                     if(tmp.top().destination.compare(neib)==0)
//                         return;
//                 tmp.pop();
//             }
//         }
//	//check if is already waiting for the neighbor to ack
//	const RoutingTableEntryExpireTimerSet* q0;
//	if(!(rrepAckExpire.empty())){
//		q0 = &(rrepAckExpire.top());
//		for(unsigned int i = 0; i < rrepAckExpire.size(); i++){
//			if(q0[i].destination.compare(neib)==0){
//				return;
//			}
//		}
//	}
	rrepAcked[string(neib)]=false;
	RoutingTableEntryExpireTimerSet ratts;
	ratts.destination = string(neib);
	ratts.lifetime = simTime().dbl() + 2 * nodeTraversalTime;
	rrepAckExpire.push(ratts);
	if(getTimer(REL_RREP_ACK_WAIT_TIMER).dbl()<=0){
		setTimer(REL_RREP_ACK_WAIT_TIMER, rrepAckExpire.top().lifetime - simTime().dbl());
	}
}

void RELRouting::setRreqBlacklistTimer(const char* neib){
    for(const auto & i : rreqBlacklistTimeout)
        if(i.destination.compare(neib) == 0)
            return;
//    if(!(rreqBlacklistTimeout.empty())){
//        auto tmp = rreqBlacklistTimeout;
//        while(!tmp.empty()) {
//                if(tmp.top().destination.compare(neib)==0)
//                    return;
//            tmp.pop();
//        }
//    }
//	//check if is already in the blacklist
//	const RoutingTableEntryExpireTimerSet* q0;
//	if(!(rreqBlacklistTimeout.empty())){
//		q0 = &(rreqBlacklistTimeout.top());
//		for(unsigned int i = 0; i < rreqBlacklistTimeout.size(); i++){
//			if(q0[i].destination.compare(neib)==0){
//				return;
//			}
//		}
//	}
	RoutingTableEntryExpireTimerSet rbetts;
	rbetts.destination = string(neib);
	rbetts.lifetime = simTime().dbl() + 2 * rreqRetries * nodeTraversalTime * netDiameter;
	rreqBlacklistTimeout.push(rbetts);
	if(getTimer(REL_RREQ_BLACKLIST_REMOVE_TIMER).dbl()<=0){
		setTimer(REL_RREQ_BLACKLIST_REMOVE_TIMER, rreqBlacklistTimeout.top().lifetime - simTime().dbl());
	}
}

void RELRouting::setRreqExpirationTimer(const char* dest){
	if(checkExpireEntry(&rreqRequest, dest)){
		return;
	}
	double expTime = 2 * netDiameter * nodeTraversalTime;
	for (int i=1;i<rreqRetryCount[string(dest)];i++){
		expTime *= 2;
	}
	RreqExpireTimerSet rets;
	rets.canceled = false;
	rets.dest = string(dest);
	rets.lifetime = simTime().dbl() + activeRouteTimeout/10;
	rets.originator = string(SELF_NETWORK_ADDRESS);
	rets.rreqID = currRreqID;
	
	if(!rreqRequest.empty() && rets.lifetime < rreqRequest.top().lifetime){
		cancelTimer(REL_RREQ_EXPIRATION_TIMER);
	}
	rreqRequest.push(rets);
	if(getTimer(REL_RREQ_EXPIRATION_TIMER).dbl()<=0){
		setTimer(REL_RREQ_EXPIRATION_TIMER,rreqRequest.top().lifetime - simTime().dbl());
	}
}

bool RELRouting::isRreqRequstActive(const char* dest){
    for(const auto & i : rreqRequest)
        if(i.dest.compare(dest) == 0)
            return true;
    for(const auto i : rreqBuffer)
        if(string(i->getDestinationREL()).compare(dest)==0)
            return true;
//    {//on going
//        auto tmp = rreqRequest;
//        while(!tmp.empty()) {
//            if(tmp.top().dest.compare(dest)==0)
//                return true;
//            tmp.pop();
//        }
//    }
//    {//buffered
//        auto tmp = rreqBuffer;
//        while(!tmp.empty()) {
//            if(string(tmp.front()->getDestinationREL()).compare(dest)==0)
//                return true;
//            tmp.pop();
//        }
//    }

    return false;
//	const RreqExpireTimerSet* q0;
//	RELRREQPacket** q1;
//	//on going
//	if(!(rreqRequest.empty())){
//		q0 = &(rreqRequest.top());
//		for(unsigned int i = 0; i < rreqRequest.size(); i++){
//			if(q0[i].dest.compare(dest)==0){
//				return true;
//			}
//		}
//	}
//	//buffered
//	if(!rreqBuffer.empty()){
//		q1 = &(rreqBuffer.front());
//		for(unsigned int i = 0; i < rreqBuffer.size(); i++){
//			if(string(q1[i]->getDestinationREL()).compare(dest)==0){
//				return true;
//			}
//		}
//	}
//	return false;
}

bool RELRouting::checkExpireEntry(const iterable_priority_queue <RreqExpireTimerSet, vector<RreqExpireTimerSet>, RreqExpireLifetimeCompare>* exq,
		const char* orig, int idx){
    for(const auto & i : *exq) {
        if(i.rreqID==idx && i.originator.compare(orig)==0)
            return true;
    }
    return false;
//    if(!(exq->empty())){
//         auto tmp = *exq;
//         while(!tmp.empty()) {
//             if(tmp.top().rreqID==idx && tmp.top().originator.compare(orig)==0)
//                 return true;
//             tmp.pop();
//         }
//     }
//     return false;
//	const RreqExpireTimerSet* q0;
//	if(!(exq->empty())){
//		q0 = &(exq->top());
//		for(unsigned int i = 0; i < exq->size(); i++){
//			if(q0[i].rreqID==idx && q0[i].originator.compare(orig)==0){
//				return true;
//			}
//		}
//	}
//	return false;
}

double RELRouting::getRemainingLifetimeTime(const char * dest,const char* nextHop){
    double x = 0;
    for(const auto & t : rtExpireSeq) {
        if(!t.canceled && t.destination.compare(dest) == 0 && t.nextHop.compare(nextHop)==0)
            if(t.lifetime > x)
                x = t.lifetime;
    }
    return x;

//    if(!rtExpireSeq.empty()){
//        auto tmp = rtExpireSeq;
//        while(!tmp.empty()) {
//            if (!tmp.top().canceled && tmp.top().destination.compare(dest)==0 && tmp.top().nextHop.compare(nextHop)==0)
//                if (tmp.top().lifetime > x)
//                    x = tmp.top().lifetime;
//            tmp.pop();
//        }
//    }
//        return x;
//	const RoutingTableEntryExpireTimerSet2* q0;
//	double x = 0;
//	if(!rtExpireSeq.empty()){
//		q0 = &rtExpireSeq.top();
//		for (int i = rtExpireSeq.size() - 1 ; i >=0; i--){
//			if (!q0[i].canceled && q0[i].destination.compare(dest)==0 && q0[i].nextHop.compare(nextHop)==0){
//				if (q0[i].lifetime > x){
//					x = q0[i].lifetime;
//				}
//			}
//		}
//	}
//	return x;
}

void RELRouting::updateLifetimeEntry(const char * dest,const char * neib, double time){
	if(rt->getState(dest,neib)!=VALID){
		return;
	}
	RoutingTableEntryExpireTimerSet2 rteet;
	
	rteluc[string(dest)][string(neib)]++;
	rteet.destination = string(dest);
	rteet.nextHop = string(neib);
	rteet.lifetime = simTime().dbl() + time;
	rteet.canceled = false;
	rtExpireSeq.push(rteet);
}

void RELRouting::resetLifetimeEntry(const char * dest){
    for(auto&& i : rtExpireSeq) {
        if(i.destination.compare(dest) == 0) {
            i.canceled = true;
            return;
        }
    }
    rteluc[string(dest)][string(dest)]=0;
//
//	RoutingTableEntryExpireTimerSet2* q0;
//	if(!(rtExpireSeq.empty())){
//		q0 = (RoutingTableEntryExpireTimerSet2*)&(rtExpireSeq.top());
//		for(unsigned int i = 0; i < rtExpireSeq.size(); i++){
//			if(q0[i].destination.compare(dest)==0){
//				q0[i].canceled = true;
//				return;
//			}
//		}
//	}
//	rteluc[string(dest)][string(dest)]=0;
//	return;
}

void RELRouting::updateRTEntry(const char* destinationAddr,unsigned long destinationSeqNum,
					bool isDestinationValid,RoutingFlag state,int hopCount, int linkCost,int energy,
					const char* nextHopAddr,std::list<std::string>* precursor){

	//refer to RFC3561 chapter 6.2
	RoutingTableEntryExpireTimerSet2 rteet;
	double oldLifetime = getRemainingLifetimeTime(destinationAddr,nextHopAddr);
	rteluc[string(destinationAddr)][string(nextHopAddr)]++;
	rteet.destination = string(destinationAddr);
	rteet.nextHop = string(nextHopAddr);
	rteet.lifetime = oldLifetime<=0 ? simTime().dbl() + activeRouteTimeout : oldLifetime + activeRouteTimeout;
	rteet.canceled = false;

	if(!rtExpireSeq.empty() && rteet.lifetime < rtExpireSeq.top().lifetime){
		cancelTimer(REL_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER);
	}
	rtExpireSeq.push(rteet);
	if(getTimer(REL_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER).dbl()<=0){
		setTimer(REL_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER, rtExpireSeq.top().lifetime - simTime().dbl());
	}
	rt->updateEntry(destinationAddr,  destinationSeqNum, isDestinationValid, state, hopCount,linkCost,energy, nextHopAddr, precursor);
}

void RELRouting::updateRTEntry(const char* destinationAddr,unsigned long destinationSeqNum,
					bool isDestinationValid,RoutingFlag state,int hopCount, int linkCost,int energy,
					const char* nextHopAddr,std::list<std::string>* precursor,
					double lifetime){
	RoutingTableEntryExpireTimerSet2 rteet;
	double oldLifetime = getRemainingLifetimeTime(destinationAddr,nextHopAddr);
	rteluc[string(destinationAddr)][string(nextHopAddr)]++;
	rteet.destination = string(destinationAddr);
	rteet.nextHop = string(nextHopAddr);
	if(lifetime>0)
	  rteet.lifetime = oldLifetime<=0 ? simTime().dbl() + lifetime : oldLifetime + lifetime;
	else
	  rteet.lifetime = oldLifetime<=0 ? simTime().dbl() + activeRouteTimeout : oldLifetime + activeRouteTimeout;
	rteet.canceled = false;
	while(!rtExpireSeq.empty() && rteet.lifetime < rtExpireSeq.top().lifetime){
		cancelTimer(REL_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER);
	}

	rtExpireSeq.push(rteet);
	if(getTimer(REL_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER).dbl()<=0){
		setTimer(REL_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER, rtExpireSeq.top().lifetime - simTime().dbl());
	}
	rt->updateEntry(destinationAddr,  destinationSeqNum, isDestinationValid, state, hopCount,linkCost,energy,nextHopAddr, precursor);
}

void RELRouting::sendRreqRequest(int hopCount, int rreqID, const char* sourceAodv, const char* destAodv,
								unsigned long sSeq, unsigned long dSeq,int energy){
	RELRREQPacket *rreqPacket =
	    new RELRREQPacket("REL routing RREQ packet", NETWORK_LAYER_PACKET);
	rreqPacket->setHopCount(hopCount);
	rreqPacket->setRreqID(rreqID);
	rreqPacket->setDestinationREL(destAodv);
	rreqPacket->setDestinationSeqNum(dSeq);
	rreqPacket->setSourceREL(sourceAodv);
	rreqPacket->setSourceSeqNum(sSeq);
	rreqPacket->setSource(SELF_NETWORK_ADDRESS);
	rreqPacket->setDestination(destAodv);
	rreqPacket->setEnergy(energy);
	rreqPacket->setId(REL_RREQ_PACKET);
	//initial value to number of weak links
	if(string(rreqPacket->getSourceREL()).compare(string(SELF_NETWORK_ADDRESS)))
		rreqPacket->setLinkCost(0);
	if (getTimer(REL_RREQ_RATE_LIMIT_TIMER).dbl() <= 0){
		++rreqRetryCount[string(destAodv)];
		setRreqBoardcastDropTimer(destAodv, sourceAodv, rreqID);
		if(string(sourceAodv).compare(SELF_NETWORK_ADDRESS)==0){
			setRreqExpirationTimer(destAodv);
		}
		setTimer(REL_RREQ_RATE_LIMIT_TIMER, (double)1/rreqRatelimit);
		toMacLayer(rreqPacket, BROADCAST_MAC_ADDRESS);
	} else {
		rreqBuffer.push(rreqPacket);
	}

}


void RELRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{
	//the application module can only send data packets
	RELDataPacket *dataPacket =
	    new RELDataPacket("REL routing data packet", NETWORK_LAYER_PACKET);
	dataPacket->setSource(SELF_NETWORK_ADDRESS);
	dataPacket->setDestinationREL(destination);
	dataPacket->setDestination(destination);
	dataPacket->setId(REL_DATA_PACKET);
	encapsulatePacket(dataPacket, pkt);
	if(string(destination).compare(BROADCAST_NETWORK_ADDRESS)==0){
		//do not route broadcast package
	        toMacLayer(dataPacket, BROADCAST_MAC_ADDRESS);
		return;
	}
	
	//refer to RFC3561 chapter 6.3
	//if in the routing table there is already an valid entry for the destination, send the package
	if(rt->isRouteValid(destination)){
		updateLifetimeEntry(destination, rt->getNextHop(destination,hopCountThreshould),activeRouteTimeout);
		dataPacket->setDestination(rt->getNextHop(destination,hopCountThreshould));
		toMacLayer(dataPacket, resolveNetworkAddress(rt->getNextHop(destination,hopCountThreshould)));
		return;
	} else if(!isRreqRequstActive(destination)){
		currSeqNum++;
		currRreqID++;
		sendRreqRequest(0, currRreqID,SELF_NETWORK_ADDRESS, destination,
						currSeqNum, rt->getDestSeqNum(destination),currEnergy);
	}
	//buffer the packet
	bufferPacket(dataPacket);
}

void RELRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
	switch((dynamic_cast <PacketId*>(pkt))->getId()){
		case REL_DATA_PACKET:
		{
			RELDataPacket *dataPacket = dynamic_cast <RELDataPacket*>(pkt);
			lqiUpdate(string(dataPacket->getSource()),lqi);
			
			if(string(dataPacket->getDestinationREL()).compare(BROADCAST_NETWORK_ADDRESS)==0){
				toApplicationLayer(dataPacket->decapsulate());
				return;
			}
			if(string(dataPacket->getDestination()).compare(SELF_NETWORK_ADDRESS)!=0){
				return;
			}
			processDataPackage(dataPacket);
		}
			break;
		case REL_RREQ_PACKET:
		{
			RELRREQPacket *rreqPacket = dynamic_cast <RELRREQPacket*>(pkt);
			lqiUpdate(string(rreqPacket->getSource()),lqi);
			processRELRreqPackage(rreqPacket, srcMacAddress, rssi, lqi);
		}
			break;
		case REL_RREP_PACKET:
		{
			RELRREPPacket *rrepPacket = dynamic_cast <RELRREPPacket*>(pkt);
			if(string(rrepPacket->getDestination()).compare(SELF_NETWORK_ADDRESS)!=0){
				return;
			}
			lqiUpdate(string(rrepPacket->getSource()),lqi);
			processRELRrepPackage(rrepPacket, srcMacAddress, rssi, lqi);
		}
			break;
		case REL_RREP_ACK_PACKET:
		{
			RELRREPAckPacket* rrepaPacket = dynamic_cast <RELRREPAckPacket*>(pkt);
			lqiUpdate(string(rrepaPacket->getSource()),lqi);
			rrepAcked[string(rrepaPacket->getSource())] = true;
		}
			break;
		case REL_HELLO_MESSAGE_PACKET:
		{
			RELHelloMessage* helloMsg = dynamic_cast <RELHelloMessage*>(pkt);
			processHelloMsg(helloMsg);
		}
			break;
		case REL_RERR_PACKET:
//		case 3:
		{
			RELRERRPacket *rerrPacket = dynamic_cast <RELRERRPacket*>(pkt);
			lqiUpdate(string(rerrPacket->getSource()),lqi);
			processRELRerrPackage(rerrPacket, srcMacAddress, rssi, lqi);
		}
			break;
//		case 7:
		case REL_RADV_PACKET:
		{
			RELRADVPacket* radvPacket = dynamic_cast <RELRADVPacket*>(pkt);	
		  	lqiUpdate(string(radvPacket->getSource()),lqi);
			processRELRadvPackage(radvPacket, srcMacAddress, rssi, lqi);
		}
			break;
		default:
			return;
	}
	return;

}
void RELRouting::processRELRadvPackage(RELRADVPacket *relRADV,int srcMacAddress, double rssi, double lqi){
    	    updateNeibPercentualEnergy(relRADV->getOriginator(),relRADV->getEnergy());
	    updateLifetimeEntry(relRADV->getOriginator(),rt->getNextHop(relRADV->getOriginator(),hopCountThreshould),activeRouteTimeout);
	    rt->updateEntryEnergy(relRADV->getOriginator(), relRADV->getEnergy());
}

void RELRouting::processDataPackage(RELDataPacket* pkt){
	RELDataPacket *ndPacket = pkt->dup();
	if(string(pkt->getDestinationREL()).compare(SELF_NETWORK_ADDRESS)==0){
		toApplicationLayer(pkt->decapsulate());
		cancelAndDelete(ndPacket);
		return;
	} else if(rt->isRouteValid(pkt->getDestinationREL())&&rt->getNextHop(pkt->getDestinationREL(),hopCountThreshould)){
		updateLifetimeEntry(pkt->getDestinationREL(),rt->getNextHop(pkt->getDestinationREL(),hopCountThreshould), activeRouteTimeout);
		ndPacket->setDestination(rt->getNextHop(pkt->getDestinationREL(),hopCountThreshould));
		toMacLayer(ndPacket, resolveNetworkAddress(rt->getNextHop(pkt->getDestinationREL(),hopCountThreshould)));
		//cancelAndDelete(pkt);
		return;
	} else if(!isRreqRequstActive(pkt->getDestinationREL())){
		//if in current time there are no rreq requests on going for the destination
		//send the RREQ to find a route
		currSeqNum++;
		currRreqID++;
		sendRreqRequest(0, currRreqID,SELF_NETWORK_ADDRESS, pkt->getDestinationREL(),
								currSeqNum, rt->getDestSeqNum(pkt->getDestinationREL()),currEnergy);
		//buffer the packet
		bufferPacket(ndPacket);
		//cancelAndDelete(pkt);
	} else {
	    //cancelAndDelete(pkt);
	    cancelAndDelete(ndPacket);
	}
}

void RELRouting::processRELRreqPackage(RELRREQPacket * rreqPackage,int srcMacAddress, double rssi, double lqi){
	//update WL of RREQ
	if(lqiMean.find(rreqPackage->getSource())!=lqiMean.end())
	    rreqPackage->setLinkCost(increaseLinkCost(lqiMean[rreqPackage->getSource()],rreqPackage->getLinkCost()));
	else
	    rreqPackage->setLinkCost(increaseLinkCost(lqi,rreqPackage->getLinkCost()));
	
	if(isInRreqBlacklist(rreqPackage->getSource())){
		return;
	}
	//updates a route to the previous hop without a valid seq number
	updateRTEntry(rreqPackage->getSource(), 0, false, VALID, 1, rreqPackage->getLinkCost(),rreqPackage->getEnergy(),rreqPackage->getSource(),NULL);

	//check if this node has already processed for this rreq, if yes drop packet
	if(checkExpireEntry(&rreqBroadcast,rreqPackage->getSourceREL(), rreqPackage->getRreqID())){
		return;
	}
	//update route for the originator
	updateRTEntry(rreqPackage->getSourceREL(), rreqPackage->getSourceSeqNum(), true, VALID, rreqPackage->getHopCount() + 1,rreqPackage->getLinkCost(),
		      rreqPackage->getEnergy(),rreqPackage->getSource(),NULL);

	if(!((string(rreqPackage->getDestinationREL()).compare(SELF_NETWORK_ADDRESS)==0))
			/*&&(!rt->getNextHop(rreqPackage->getDestinationREL()))*/){ // PRG: this line was commented but without commenting it's faster
		//forwarding the rreq
		//refer to RFC3561 chapter 6.5
		
		sendRreqRequest(rreqPackage->getHopCount() + 1,
						rreqPackage->getRreqID(),
						rreqPackage->getSourceREL(),
						rreqPackage->getDestinationREL(),
						rreqPackage->getSourceSeqNum(),
						rt->getDestSeqNum(rreqPackage->getDestinationREL()) > rreqPackage->getDestinationSeqNum() ?
							rt->getDestSeqNum(rreqPackage->getDestinationREL()) : rreqPackage->getDestinationSeqNum(),
							currEnergy
 			      );
	}

	else if(rt->getNextHop(rreqPackage->getDestinationREL(),hopCountThreshould)){
		//current node has an active route to the destination
		//refer to RFC3561 chapter 6.6.2

		setRreqBoardcastDropTimer(rreqPackage->getDestinationREL(), rreqPackage->getSourceREL(), rreqPackage->getRreqID());
		sendRrepPacket(rt->getHopCount(rreqPackage->getDestinationREL()),
						rreqPackage->getSourceREL(),
						rreqPackage->getDestinationREL(),
						rt->getDestSeqNum(rreqPackage->getDestinationREL()),
						getRemainingLifetimeTime(rreqPackage->getDestinationREL(),rt->getNextHop(rreqPackage->getDestinationREL(),
				                hopCountThreshould))-simTime().dbl(),
						false,currEnergy);
	} else {
		//current node is the destination
		//refer to RFC3561 chapter 6.6.1
		if(rreqPackage->getDestinationSeqNum()==currSeqNum){
			currSeqNum++;
		}
		setRreqBoardcastDropTimer(rreqPackage->getDestinationREL(), rreqPackage->getSourceREL(), rreqPackage->getRreqID());
		//MY_ROUTE_TIMOUT = 2 * ACTIVE_ROUTE_TIMEOUT
		sendRrepPacket(0, rreqPackage->getSourceREL(), SELF_NETWORK_ADDRESS,
						currSeqNum, 2 * activeRouteTimeout,  false,currEnergy);
	}
}

void RELRouting::processRELRrepPackage(RELRREPPacket * rrepPackage,int srcMacAddress, double rssi, double lqi){

	//update field WL of RREP
	if(lqiMean.find(rrepPackage->getSource())!=lqiMean.end())
	    rrepPackage->setLinkCost(increaseLinkCost(lqiMean[rrepPackage->getSource()],rrepPackage->getLinkCost()));
	else
	    rrepPackage->setLinkCost(increaseLinkCost(lqi,rrepPackage->getLinkCost()));
	
	sendRrepAckPacket(rrepPackage->getSource());
	/* TODO: temporary disabled for debugging*/
	if(getTimer(REL_HELLO_MESSAGE_REFRESH_TIMER).dbl()<=0){
		sendHelloMessage();
	}


	//updates a route to the previous hop without a valid seq number
	updateRTEntry(rrepPackage->getSource(), 0, false, VALID, 1,rrepPackage->getLinkCost(),rrepPackage->getEnergy(), rrepPackage->getSource(),NULL);
	//update route for the destination
	
	updateRTEntry(rrepPackage->getDestinationREL(), rrepPackage->getDestinationSeqNum(),
				true, VALID, rrepPackage->getHopCount() + 1,rrepPackage->getLinkCost(),rrepPackage->getEnergy(), rrepPackage->getSource(),
				NULL,rrepPackage->getLifetime());
	if (!(string(rrepPackage->getOriginator()).compare(SELF_NETWORK_ADDRESS)==0)){
		//forward the rrep
		sendRrepPacket(rrepPackage->getHopCount() + 1,
						rrepPackage->getOriginator(),
						rrepPackage->getDestinationREL(),
						rrepPackage->getDestinationSeqNum(),
						rrepPackage->getLifetime(),
						true,currEnergy);
	} else {
		//routing table already updated for destination
		//send buffered packages for destination
		rreqRetryCount[string(rrepPackage->getDestinationREL())] = 0;
		processPackagesInBuffer(rrepPackage->getDestinationREL(), false);
		cancelExpireEntry(&rreqRequest,rrepPackage->getDestinationREL());
	}
}

void RELRouting::processPackagesInBuffer(const char * dest, bool drop){
	queue< cPacket* > bufferTemp;
	RELDataPacket* currPkt;
	while (!TXBuffer.empty()) {
		currPkt = dynamic_cast <RELDataPacket*>(TXBuffer.front());
		if(currPkt && string(dest).compare(currPkt->getDestinationREL())==0){
			if(!drop && rt->getNextHop(dest,hopCountThreshould)){
				currPkt->setDestination(rt->getNextHop(dest,hopCountThreshould));
				toMacLayer(currPkt, resolveNetworkAddress(rt->getNextHop(dest,hopCountThreshould)));
			} else
                cancelAndDelete(currPkt);
		}
		else{
			bufferTemp.push(TXBuffer.front());
		}
		TXBuffer.pop();
		updateLifetimeEntry(dest,rt->getNextHop(dest,hopCountThreshould) ,activeRouteTimeout);
	}
	while (!bufferTemp.empty()) {
		TXBuffer.push(bufferTemp.front());
		bufferTemp.pop();
	}

}

void RELRouting::timerFiredCallback(int index){
	switch(index){
	case REL_RREQ_BROADCAST_DROP_TIMER:
		rreqBroadcast.pop();
		if(!rreqBroadcast.empty()){
			setTimer(REL_RREQ_BROADCAST_DROP_TIMER, rreqBroadcast.top().lifetime - simTime().dbl());
		}
		break;

	case REL_RREQ_EXPIRATION_TIMER:
	{
		if (rreqRequest.top().canceled){ 
			rreqRequest.pop();
			if(!rreqRequest.empty()){
				setTimer(REL_RREQ_EXPIRATION_TIMER, rreqRequest.top().lifetime - simTime().dbl());
			}
			break;
		}
		string dest1 = string(rreqRequest.top().dest);
		rreqRequest.pop();
		if(!rreqRequest.empty()){
			setTimer(REL_RREQ_EXPIRATION_TIMER, rreqRequest.top().lifetime - simTime().dbl());
		}
		if(rreqRetryCount[dest1] < rreqRetries){
			//send another rreq
			currRreqID++;
			sendRreqRequest(0, currRreqID, SELF_NETWORK_ADDRESS, dest1.c_str(),
							currSeqNum, rt->getDestSeqNum(dest1.c_str()),currEnergy);
		} else {
			rreqRetryCount[dest1] = 0;
			processPackagesInBuffer(dest1.c_str(),true);
		}
	}
		break;

	case REL_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER:
	{
		//se rota nao tiver sido cancelada

		if(!rtExpireSeq.top().canceled){
			string dest2 = string(rtExpireSeq.top().destination);
			string nextHop2 = string(rtExpireSeq.top().nextHop);	
			rteluc[dest2][nextHop2]--;
			if (rteluc[dest2][nextHop2]==0){
				if(rt->getState(dest2.c_str(),nextHop2.c_str())==VALID){				  

					rt->deleteRoute(dest2,nextHop2);
				}
			}
		}
		rtExpireSeq.pop();
		//it is possible that multiple entry expires at the same time
		while(!rtExpireSeq.empty() && rtExpireSeq.top().lifetime - simTime().dbl()<=0){
			if(!rtExpireSeq.top().canceled){
				string dest3 = string(rtExpireSeq.top().destination);
				string nextHop3 = string(rtExpireSeq.top().nextHop);				
				rteluc[dest3][nextHop3]--;
				if (rteluc[dest3][nextHop3]==0){		  
					if(rt->getState(dest3.c_str(),nextHop3.c_str())==VALID){
						rt->deleteRoute(dest3,nextHop3);
					}
				}
			}
			rtExpireSeq.pop();
		}
		if(!rtExpireSeq.empty()){
			    setTimer(REL_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER, rtExpireSeq.top().lifetime - simTime().dbl());
			
		}
	}
		break;
	
	case REL_RREQ_RATE_LIMIT_TIMER:
		if(!rreqBuffer.empty()){
			processrreqBuffer();
		}
		break;

	case REL_HELLO_MESSAGE_REFRESH_TIMER:
		sendHelloMessage();
		break;

	case REL_HELLO_MESSAGE_EXPIRE_TIMER:
	{
		string neib = hmExpireSeq.top().destination;
		hmeluc[neib]--;
		if (hmeluc[neib]==0){   
			list<string>* affectedDest = new list<string>();
			list<string>* affectedPrecursor = new list<string>();
			rt->reportLinkBroken(neib.c_str(), affectedDest, affectedPrecursor);
			resetLifetimeEntry(neib.c_str());
			sendRerrMessage(affectedDest, affectedPrecursor);
                        delete affectedDest;
                        delete affectedPrecursor;
		}
		hmExpireSeq.pop();
		while(!hmExpireSeq.empty() && hmExpireSeq.top().lifetime - simTime().dbl()<=0){
			string dest3 = hmExpireSeq.top().destination;
			hmeluc[dest3]--;
			if (hmeluc[dest3]==0){
				list<string>* affectedDest = new list<string>();
				list<string>* affectedPrecursor = new list<string>();
				rt->reportLinkBroken(dest3.c_str(), affectedDest, affectedPrecursor);
				resetLifetimeEntry(dest3.c_str());
				sendRerrMessage(affectedDest, affectedPrecursor);
	                        delete affectedDest;
	                        delete affectedPrecursor;
			}
			hmExpireSeq.pop();
		}
		if(!hmExpireSeq.empty()){
			setTimer(REL_HELLO_MESSAGE_EXPIRE_TIMER, hmExpireSeq.top().lifetime - simTime().dbl());
		}
	}
		break;

	case REL_RERR_RATE_LIMIT_TIMER:
		if(!rerrBuffer.empty()){
			toMacLayer(rerrBuffer.front(), resolveNetworkAddress(rerrBuffer.front()->getDestination()));
			rerrBuffer.pop();
			setTimer(REL_RERR_RATE_LIMIT_TIMER, (double)1/rerrRatelimit);
		}
		break;

	case REL_RREP_ACK_WAIT_TIMER:
		if(!rrepAcked[string(rrepAckExpire.top().destination)]){
			rreqBlacklist.push_front(string(rrepAckExpire.top().destination));
			setRreqBlacklistTimer(rrepAckExpire.top().destination.c_str());
		}
		rrepAckExpire.pop();
		if(!rrepAckExpire.empty()){

			while(((rrepAckExpire.top().lifetime - simTime().dbl())<0)&&!rrepAckExpire.empty()){
			  rrepAckExpire.pop();
			}

			if(!rrepAckExpire.empty()){
				setTimer(REL_RREP_ACK_WAIT_TIMER, rrepAckExpire.top().lifetime - simTime().dbl());
			}
		}
		break;

	case REL_RREQ_BLACKLIST_REMOVE_TIMER:
		{
			string node = string(rreqBlacklistTimeout.top().destination);
			rreqBlacklist.remove(node);
			rreqBlacklistTimeout.pop();
			if(!rreqBlacklistTimeout.empty()){
                double t = max(0.0, rreqBlacklistTimeout.top().lifetime - simTime().dbl());
				setTimer(REL_RREQ_BLACKLIST_REMOVE_TIMER, t);
			}
		}
		break;
	case REL_RADV_SEND_TIMER:
		if(updateSelfPercentualEnergy());
		    sendRadvPacket();
		setTimer(REL_RADV_SEND_TIMER, 1);
		break;
	default:
		return;

	}
	return;
}

void RELRouting::sendHelloMessage(){
	RELHelloMessage* helloMsg = new RELHelloMessage("REL hello message packet", NETWORK_LAYER_PACKET);
	helloMsg->setSource(SELF_NETWORK_ADDRESS);
	helloMsg->setDestination(BROADCAST_NETWORK_ADDRESS);
	helloMsg->setId(REL_HELLO_MESSAGE_PACKET);
	toMacLayer(helloMsg, BROADCAST_MAC_ADDRESS);
	setTimer(REL_HELLO_MESSAGE_REFRESH_TIMER, helloInterval);
}

void RELRouting::processrreqBuffer(){
	string dest=string(rreqBuffer.front()->getDestinationREL());
	++rreqRetryCount[dest];
	setRreqBoardcastDropTimer(rreqBuffer.front()->getDestinationREL(),
								rreqBuffer.front()->getSourceREL(),
								rreqBuffer.front()->getRreqID());
	if(string(rreqBuffer.front()->getSourceREL()).compare(SELF_NETWORK_ADDRESS)==0){
		setRreqExpirationTimer(dest.c_str());
	}
	
	toMacLayer(rreqBuffer.front(), BROADCAST_MAC_ADDRESS);
	rreqBuffer.pop();
	setTimer(REL_RREQ_RATE_LIMIT_TIMER, (double)1/rreqRatelimit);
}

void RELRouting::sendRrepPacket(int hopCount, const char* orig, const char* destAodv,
								unsigned long dSeq, double lifetime,  bool forwarding,int energy){
	
	if(!rt->getNextHop(orig,hopCountThreshould)) return;
	RELRREPPacket *rrepPackage =
					new RELRREPPacket("REL routing RREP packet", NETWORK_LAYER_PACKET);
	rrepPackage->setHopCount(hopCount);
	rrepPackage->setOriginator(orig);
	rrepPackage->setDestinationREL(destAodv);
	rrepPackage->setDestinationSeqNum(dSeq);
	rrepPackage->setSource(SELF_NETWORK_ADDRESS);
	rrepPackage->setDestination(rt->getNextHop(orig,hopCountThreshould));
	rrepPackage->setLifetime(lifetime);
	rrepPackage->setEnergy(energy);
	rrepPackage->setId(2);

	if(string(destAodv).compare(SELF_NETWORK_ADDRESS)!=0){
		rt->updatePrecursor(destAodv, rt->getNextHop(orig,hopCountThreshould));
	}
	else{
		rrepPackage->setLinkCost(0);
	}
	if(forwarding){
		updateLifetimeEntry(orig, rt->getNextHop(orig,hopCountThreshould),activeRouteTimeout);
	}
	toMacLayer(rrepPackage, resolveNetworkAddress(rt->getNextHop(orig,hopCountThreshould)));
	setRrepAckTimer(rt->getNextHop(orig,hopCountThreshould));
}

void RELRouting::processHelloMsg(RELHelloMessage* msg){
	RoutingTableEntryExpireTimerSet hmeet;
	hmeluc[string(msg->getSource())]++;
	hmeet.destination = string(msg->getSource());
	hmeet.lifetime = allowedHelloLoss * helloInterval + simTime().dbl();
	hmExpireSeq.push(hmeet);
	if(getTimer(REL_HELLO_MESSAGE_EXPIRE_TIMER).dbl()<=0){
		setTimer(REL_HELLO_MESSAGE_EXPIRE_TIMER, hmExpireSeq.top().lifetime - simTime().dbl());
	}
}

void RELRouting::sendRerrMessage(list<string>* affDest, list<string>* affPrecur){
	if(affDest->empty()||affPrecur->empty()){
		return;
	}
	for (list<string>::const_iterator i=affPrecur->begin();i!=affPrecur->end();++i){
		sendSingleRerrMessage(affDest, affPrecur, (*i).c_str());
	}
}

void RELRouting::sendSingleRerrMessage(list<string>* affDest, list<string>* affPrecur, const char* dest){
	int destCnt = affDest->size();
	RELRERRPacket *rerrPackage =
						new RELRERRPacket("REL routing RERR packet", NETWORK_LAYER_PACKET);
	rerrPackage->setSource(SELF_NETWORK_ADDRESS);
	rerrPackage->setDestCount(destCnt);
	rerrPackage->setUnreachableDestAddrArraySize(destCnt);
	rerrPackage->setUnreachableDestSeqNumArraySize(destCnt);
	rerrPackage->setDestination(dest);
	rerrPackage->setId(REL_RERR_PACKET);
	for(int i=0;i<destCnt;i++){
		rerrPackage->setUnreachableDestAddr(i,affDest->front().c_str());
		rerrPackage->setUnreachableDestSeqNum(i,rt->getDestSeqNum(affDest->front().c_str()));
		affDest->pop_front();
	}
	if (getTimer(REL_RERR_RATE_LIMIT_TIMER).dbl() <= 0){
		toMacLayer(rerrPackage, resolveNetworkAddress(dest));
		setTimer(REL_RERR_RATE_LIMIT_TIMER, (double)1/rerrRatelimit);
	} else {
		rerrBuffer.push(rerrPackage);
	}
}

void RELRouting::processRELRerrPackage(RELRERRPacket* rerrPacket,int srcMacAddress, double rssi, double lqi){

	if(string(rerrPacket->getDestination()).compare(SELF_NETWORK_ADDRESS)!=0){
		return;
	}
	unsigned int affDestCount = (unsigned int)rerrPacket->getDestCount();
	if (affDestCount!=rerrPacket->getUnreachableDestAddrArraySize() ||
		affDestCount!=rerrPacket->getUnreachableDestSeqNumArraySize()){
		//broken rerr packet
		return;
	}
	list<string>* affDest = new list<string>();
	list<string>* affPre = new list<string>();
	for (unsigned int i = 0; i < affDestCount; i++){
		affDest->push_front(string(rerrPacket->getUnreachableDestAddr(i)));
		rt->updateSeqNum(rerrPacket->getUnreachableDestAddr(i),rerrPacket->getUnreachableDestSeqNum(i));
	}
	rt->forwardLinkBroken(rerrPacket->getSource(),affDest,affPre);
	sendRerrMessage(affDest,affPre);
        delete affDest;
        delete affPre;
}

void RELRouting::sendRrepAckPacket(const char* neib){
	RELRREPAckPacket *rrepaPacket =
						new RELRREPAckPacket("REL routing RREP ACK packet", NETWORK_LAYER_PACKET);
	rrepaPacket->setSource(SELF_NETWORK_ADDRESS);
	rrepaPacket->setDestination(neib);
	rrepaPacket->setId(REL_RREP_ACK_PACKET);
	toMacLayer(rrepaPacket, resolveNetworkAddress(neib));
}

void RELRouting::sendRadvPacket(){
  RELRADVPacket* radvPacket = new RELRADVPacket("REL routing RADV packet", NETWORK_LAYER_PACKET);
  radvPacket->setSource(SELF_NETWORK_ADDRESS);
  radvPacket->setOriginator(SELF_NETWORK_ADDRESS);
  radvPacket->setEnergy(currEnergy);
  radvPacket->setId(REL_RADV_PACKET);
  toMacLayer(radvPacket, BROADCAST_MAC_ADDRESS);
}
bool RELRouting::updateSelfPercentualEnergy(){
	int newEnergy = (int)((resourceMng->getRemainingEnergy())/(resourceMng->initialEnergy)*100);
	if((currEnergy-newEnergy)>=energyThreshould){
	  currEnergy = currEnergy-energyThreshould;
	  return true;
	}
	else{
	  return false;
	}
}

void RELRouting::updateNeibPercentualEnergy(string neib,int energy){
    neibEnergy[neib]= energy;
}

void RELRouting::lqiUpdate(string neib,double lqi){
	if(lqiVector.find(string(neib)) == lqiVector.end()){
	  lqiVector.insert(pair<string, list<double>* >(neib,new list<double>));
	}
	  
	if(lqiVector[neib]->size()>=lqiVectorSize){
		double mean=0;
		for(list<double>::const_iterator i=lqiVector[neib]->begin();i!=lqiVector[neib]->end();++i){
			mean +=  (*i) ;
        	}
		mean = mean/lqiVectorSize;
		lqiMean[neib] = mean;
		lqiVector[neib]->clear();		
	}
	lqiVector[neib]->push_back(lqi);
}
int RELRouting::increaseLinkCost(int lqi,int linkCost){
	return linkCost+(int)(30/3.75);
}


int RELRouting::getMaxNeibEnergy(){
    int max=0;
    for(map<string,int>::const_iterator i=neibEnergy.begin();i!=neibEnergy.end();++i){
		if(i->second>max)
		  max = i->second;
    }    
    return max;
}


int RELRouting::getMinNeibEnergy(){
    int min=100;
    for(map<string,int>::const_iterator i=neibEnergy.begin();i!=neibEnergy.end();++i){
		if(i->second<min)
		  min = i->second;
    }
    return min;
}
