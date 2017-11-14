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

#include "LabileRouting.h"
#include <csimulation.h>

Define_Module(LabileRouting);

void LabileRouting::startup()
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
	lqiThreshould = par("lqiThreshould");
	hopCountThreshould = par("hopCountThreshould");
	currSeqNum = 1; 
	currRreqID = 0; 
	rt = new LabileTable();
}

void LabileRouting::finish(){
	VirtualRouting::finish();
	LabileRREQPacket* rreqpkt;
	LabileRERRPacket* rerrpkt;
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

void LabileRouting::setRreqBoardcastDropTimer(const char* dest, const char* source, int id){
	if(checkExpireEntry(&rreqBroadcast, source, id)){
		return;
	}
	RreqExpireTimerSet rets;
	rets.canceled = false;
	rets.dest = string(dest);
	rets.lifetime = simTime().dbl() + 2 * nodeTraversalTime * 1000; //PATH_DISCOVERY_TIME = 2 * NODE_TRAVERSAL_TIME
	rets.originator = string(source);
	rets.rreqID = id;
	
	if(!rreqBroadcast.empty() && rets.lifetime < rreqBroadcast.top().lifetime){
		cancelTimer(LABILE_RREQ_BROADCAST_DROP_TIMER);
	}
	rreqBroadcast.push(rets);
	if(getTimer(LABILE_RREQ_BROADCAST_DROP_TIMER).dbl()<=0){
		setTimer(LABILE_RREQ_BROADCAST_DROP_TIMER, rreqBroadcast.top().lifetime - simTime().dbl());
	}

}

bool LabileRouting::isInRreqBlacklist(const char* dest){
    for(list<string>::const_iterator i=rreqBlacklist.begin();i!=rreqBlacklist.end();++i){
        string s = *i;
        if(s.compare(dest)==0){
            return true;
        }
    }
    return false;
}

bool LabileRouting::checkExpireEntry(const iterable_priority_queue <RreqExpireTimerSet, vector<RreqExpireTimerSet>, RreqExpireLifetimeCompare>* exq,
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

void LabileRouting::cancelExpireEntry(iterable_priority_queue <RreqExpireTimerSet, vector<RreqExpireTimerSet>, RreqExpireLifetimeCompare>* exq,
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

void LabileRouting::setRrepAckTimer(const char* neib){
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
	if(getTimer(LABILE_RREP_ACK_WAIT_TIMER).dbl()<=0){
		setTimer(LABILE_RREP_ACK_WAIT_TIMER, rrepAckExpire.top().lifetime - simTime().dbl());
	}
}

void LabileRouting::setRreqBlacklistTimer(const char* neib){
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
	if(getTimer(LABILE_RREQ_BLACKLIST_REMOVE_TIMER).dbl()<=0){
		setTimer(LABILE_RREQ_BLACKLIST_REMOVE_TIMER, rreqBlacklistTimeout.top().lifetime - simTime().dbl());
	}
}

void LabileRouting::setRreqExpirationTimer(const char* dest){
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
	//if the first to expire changes
	if(!rreqRequest.empty() && rets.lifetime < rreqRequest.top().lifetime){
		cancelTimer(LABILE_RREQ_EXPIRATION_TIMER);
	}
	rreqRequest.push(rets);
	if(getTimer(LABILE_RREQ_EXPIRATION_TIMER).dbl()<=0){
		setTimer(LABILE_RREQ_EXPIRATION_TIMER,rreqRequest.top().lifetime - simTime().dbl());
	}
}

bool LabileRouting::isRreqRequstActive(const char* dest){
    for(const auto & i : rreqRequest)
        if(i.dest.compare(dest) == 0)
            return true;
    for(const auto i : rreqBuffer)
        if(string(i->getDestinationLABILE()).compare(dest)==0)
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
//            if(string(tmp.front()->getDestinationLABILE()).compare(dest)==0)
//                return true;
//            tmp.pop();
//        }
//    }

    return false;
}

bool LabileRouting::checkExpireEntry(const iterable_priority_queue <RreqExpireTimerSet, vector<RreqExpireTimerSet>, RreqExpireLifetimeCompare>* exq,
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

double LabileRouting::getRemainingLifetimeTime(const char * dest){
    double x = 0;
    for(const auto & t : rtExpireSeq) {
        if(!t.canceled && t.destination.compare(dest) == 0)
            if(t.lifetime > x)
                x = t.lifetime;
    }
    return x;

//    if(!rtExpireSeq.empty()){
//        auto tmp = rtExpireSeq;
//        while(!tmp.empty()) {
//            if (!tmp.top().canceled && tmp.top().destination.compare(dest)==0)
//                if (tmp.top().lifetime > x)
//                    x = tmp.top().lifetime;
//            tmp.pop();
//        }
//    }
//	return x;
}

void LabileRouting::updateLifetimeEntry(const char * dest, double time){
	if(rt->getState(dest)!=VALID){
		return;
	}
	RoutingTableEntryExpireTimerSet rteet;
	rteluc[string(dest)]++;
	rteet.destination = string(dest);
	rteet.lifetime = simTime().dbl() + time;
	rteet.canceled = false;
	rtExpireSeq.push(rteet);
}

void LabileRouting::resetLifetimeEntry(const char * dest){
    for(auto&& i : rtExpireSeq) {
        if(i.destination.compare(dest) == 0) {
            i.canceled = true;
            return;
        }
    }
    rteluc[string(dest)]=0;

//    auto exq = &rtExpireSeq;
//    if(!exq->empty()) {
//        priority_queue<RoutingTableEntryExpireTimerSet,vector<RoutingTableEntryExpireTimerSet,allocator<RoutingTableEntryExpireTimerSet>>,RoutingTableEntryLifetimeCompare> tmp;
//        while(!exq->empty()) {
//            auto q0 = exq->top();
//            if(q0.destination.compare(dest)==0) {
//                q0.canceled = true;
//                tmp.push(q0);
//                exq->pop();
//                exq->swap(tmp);
//                return;
//            }
//            tmp.push(q0);
//            exq->pop();
//        }
//        exq->swap(tmp);
//    }
//    rteluc[string(dest)]=0;

//	RoutingTableEntryExpireTimerSet* q0;
//	if(!(rtExpireSeq.empty())){
//		q0 = (RoutingTableEntryExpireTimerSet*)&(rtExpireSeq.top());
//		for(unsigned int i = 0; i < rtExpireSeq.size(); i++){
//			if(q0[i].destination.compare(dest)==0){
//				q0[i].canceled = true;
//				return;
//			}
//		}
//	}
//	rteluc[string(dest)]=0;
//	return;
}

void LabileRouting::updateRTEntry(const char* destinationAddr,unsigned long destinationSeqNum,
					bool isDestinationValid,RoutingFlag state,int hopCount, int weakLinks,
					const char* nextHopAddr,std::list<std::string>* precursor){
	//refer to RFC3561 chapter 6.2
	RoutingTableEntryExpireTimerSet rteet;
	double oldLifetime = getRemainingLifetimeTime(destinationAddr);
	rteluc[string(destinationAddr)]++;
	rteet.destination = string(destinationAddr);
	rteet.lifetime = oldLifetime<=0 ? simTime().dbl() + activeRouteTimeout : oldLifetime + activeRouteTimeout;
	rteet.canceled = false;

	if(!rtExpireSeq.empty() && rteet.lifetime < rtExpireSeq.top().lifetime){
		cancelTimer(LABILE_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER);
	}
	rtExpireSeq.push(rteet);
	if(getTimer(LABILE_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER).dbl()<=0){
		setTimer(LABILE_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER, rtExpireSeq.top().lifetime - simTime().dbl());
	}
	rt->updateEntry(destinationAddr,  destinationSeqNum, isDestinationValid, state, hopCount,weakLinks, nextHopAddr, precursor,hopCountThreshould);
}

void LabileRouting::updateRTEntry(const char* destinationAddr,unsigned long destinationSeqNum,
					bool isDestinationValid,RoutingFlag state,int hopCount, int weakLinks,
					const char* nextHopAddr,std::list<std::string>* precursor,
					double lifetime){
	RoutingTableEntryExpireTimerSet rteet;
	double oldLifetime = getRemainingLifetimeTime(destinationAddr);
	rteluc[string(destinationAddr)]++;
	rteet.destination = string(destinationAddr);
	rteet.lifetime = oldLifetime<=0 ? simTime().dbl() + lifetime : oldLifetime + lifetime;
	rteet.canceled = false;

	if(!rtExpireSeq.empty() && rteet.lifetime < rtExpireSeq.top().lifetime){
		cancelTimer(LABILE_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER);
	}
	rtExpireSeq.push(rteet);
	if(getTimer(LABILE_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER).dbl()<=0){
		setTimer(LABILE_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER, rtExpireSeq.top().lifetime - simTime().dbl());
	}
	rt->updateEntry(destinationAddr,  destinationSeqNum, isDestinationValid, state, hopCount,weakLinks, nextHopAddr, precursor,hopCountThreshould);
}

void LabileRouting::sendRreqRequest(int hopCount, int rreqID, const char* sourceAodv, const char* destAodv,
								unsigned long sSeq, unsigned long dSeq){
	LabileRREQPacket *rreqPacket =
	    new LabileRREQPacket("LABILE routing RREQ packet", NETWORK_LAYER_PACKET);
	rreqPacket->setFlagD(false);
	rreqPacket->setFlagG(false);
	rreqPacket->setFlagJ(false);
	rreqPacket->setFlagR(false);
	rreqPacket->setFlagU(false);
	rreqPacket->setHopCount(hopCount);
	rreqPacket->setRreqID(rreqID);
	rreqPacket->setDestinationLABILE(destAodv);
	rreqPacket->setDestinationSeqNum(dSeq);
	rreqPacket->setSourceLABILE(sourceAodv);
	rreqPacket->setSourceSeqNum(sSeq);
	rreqPacket->setSource(SELF_NETWORK_ADDRESS);
	rreqPacket->setDestination(destAodv);
	rreqPacket->setId(LABILE_RREQ_PACKET);
	//initial value to number of weak links
	if(string(rreqPacket->getSourceLABILE()).compare(string(SELF_NETWORK_ADDRESS)))
		rreqPacket->setWeakLinks(0);
	if (getTimer(LABILE_RREQ_RATE_LIMIT_TIMER).dbl() <= 0){
		++rreqRetryCount[string(destAodv)];
		setRreqBoardcastDropTimer(destAodv, sourceAodv, rreqID);
		if(string(sourceAodv).compare(SELF_NETWORK_ADDRESS)==0){
			setRreqExpirationTimer(destAodv);
		}
		setTimer(LABILE_RREQ_RATE_LIMIT_TIMER, (double)1/rreqRatelimit);
		toMacLayer(rreqPacket, BROADCAST_MAC_ADDRESS);
	} else {
		rreqBuffer.push(rreqPacket);
	}

}


void LabileRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{
	//the application module can only send data packets
	LabileDataPacket *dataPacket =
	    new LabileDataPacket("LABILE routing data packet", NETWORK_LAYER_PACKET);
	dataPacket->setSource(SELF_NETWORK_ADDRESS);
	dataPacket->setDestinationLABILE(destination);
	dataPacket->setDestination(destination);
	dataPacket->setId(LABILE_DATA_PACKET);
	encapsulatePacket(dataPacket, pkt);
	if(string(destination).compare(BROADCAST_NETWORK_ADDRESS)==0){
		//do not route broadcast package
	        toMacLayer(dataPacket, BROADCAST_MAC_ADDRESS);
		return;
	}
	
	//refer to RFC3561 chapter 6.3
	//if in the routing table there is already an valid entry for the destination, send the package
	if(rt->isRouteValid(destination)){
		updateLifetimeEntry(destination, activeRouteTimeout);
		dataPacket->setDestination(rt->getNextHop(destination));
		toMacLayer(dataPacket, resolveNetworkAddress(rt->getNextHop(destination)));
		return;
	} else if(!isRreqRequstActive(destination)){
		//if in current time there are no rreq requests on going for the destination
		//send the RREQ to find a route
		currSeqNum++;
		currRreqID++;
		sendRreqRequest(0, currRreqID,SELF_NETWORK_ADDRESS, destination,
						currSeqNum, rt->getDestSeqNum(destination));
	}
	//buffer the packet
	bufferPacket(dataPacket);
}

void LabileRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
	switch((dynamic_cast <PacketId*>(pkt))->getId()){	
		case LABILE_DATA_PACKET:
		{
			LabileDataPacket *dataPacket = dynamic_cast <LabileDataPacket*>(pkt);
			if(string(dataPacket->getDestinationLABILE()).compare(BROADCAST_NETWORK_ADDRESS)==0){
				toApplicationLayer(dataPacket->decapsulate());
				return;
			}
			if(string(dataPacket->getDestination()).compare(SELF_NETWORK_ADDRESS)!=0){
				return;
			}
			processDataPackage(dataPacket);
		}
			break;
		case LABILE_RREQ_PACKET:
		{
			LabileRREQPacket *rreqPacket = dynamic_cast <LabileRREQPacket*>(pkt);
			processLabileRreqPackage(rreqPacket, srcMacAddress, rssi, lqi);
		}
			break;
		case LABILE_RREP_PACKET:
		{
			LabileRREPPacket *rrepPacket = dynamic_cast <LabileRREPPacket*>(pkt);
			if(string(rrepPacket->getDestination()).compare(SELF_NETWORK_ADDRESS)!=0){
				return;
			}
			processLabileRrepPackage(rrepPacket, srcMacAddress, rssi, lqi);
		}
			break;
		case LABILE_RREP_ACK_PACKET:
		{
			LabileRREPAckPacket* rrepaPacket = dynamic_cast <LabileRREPAckPacket*>(pkt);
			rrepAcked[string(rrepaPacket->getSource())] = true;
		}
			break;
		case LABILE_HELLO_MESSAGE_PACKET:
		{
			LabileHelloMessage* helloMsg = dynamic_cast <LabileHelloMessage*>(pkt);
			processHelloMsg(helloMsg);
		}
			break;
		case LABILE_RERR_PACKET:
		{
			LabileRERRPacket *rerrPacket = dynamic_cast <LabileRERRPacket*>(pkt);
			processLabileRerrPackage(rerrPacket, srcMacAddress, rssi, lqi);
		}
			break;
		default:
			return;
	}
	return;

}

void LabileRouting::processDataPackage(LabileDataPacket* pkt){
	LabileDataPacket *ndPacket = pkt->dup();
	if(string(pkt->getDestinationLABILE()).compare(SELF_NETWORK_ADDRESS)==0){
		toApplicationLayer(pkt->decapsulate());
		cancelAndDelete(ndPacket);
		return;
	} else if(rt->isRouteValid(pkt->getDestinationLABILE())&&rt->getNextHop(pkt->getDestinationLABILE())){
		updateLifetimeEntry(pkt->getDestinationLABILE(), activeRouteTimeout);
		ndPacket->setDestination(rt->getNextHop(pkt->getDestinationLABILE()));

		toMacLayer(ndPacket, resolveNetworkAddress(rt->getNextHop(pkt->getDestinationLABILE())));
		return;
	} else if(!isRreqRequstActive(pkt->getDestinationLABILE())){
		//if in current time there are no rreq requests on going for the destination
		//send the RREQ to find a route
		currSeqNum++;
		currRreqID++;
		sendRreqRequest(0, currRreqID,SELF_NETWORK_ADDRESS, pkt->getDestinationLABILE(),
								currSeqNum, rt->getDestSeqNum(pkt->getDestinationLABILE()));
		//buffer the packet
		bufferPacket(ndPacket);
	} else {
	    cancelAndDelete(ndPacket);
	}
}

void LabileRouting::processLabileRreqPackage(LabileRREQPacket * rreqPackage,int srcMacAddress, double rssi, double lqi){
	//update WL of RREQ
	if(lqi<lqiThreshould)
		rreqPackage->setWeakLinks(rreqPackage->getWeakLinks()+1);
	if(isInRreqBlacklist(rreqPackage->getSource())){
		return;
	}
	//updates a route to the previous hop without a valid seq number
	updateRTEntry(rreqPackage->getSource(), 0, false, VALID, 1, rreqPackage->getWeakLinks(),rreqPackage->getSource(),NULL);

	//check if this node has already processed for this rreq, if yes drop packet
	if(checkExpireEntry(&rreqBroadcast,rreqPackage->getSourceLABILE(), rreqPackage->getRreqID())){
		return;
	}
	//update route for the originator
	updateRTEntry(rreqPackage->getSourceLABILE(), rreqPackage->getSourceSeqNum(), true, VALID, rreqPackage->getHopCount() + 1,rreqPackage->getWeakLinks(), rreqPackage->getSource(),NULL);

	if(!((string(rreqPackage->getDestinationLABILE()).compare(SELF_NETWORK_ADDRESS)==0))){
		//forwarding the rreq
		//refer to RFC3561 chapter 6.5
		sendRreqRequest(rreqPackage->getHopCount() + 1,
						rreqPackage->getRreqID(),
						rreqPackage->getSourceLABILE(),
						rreqPackage->getDestinationLABILE(),
						rreqPackage->getSourceSeqNum(),
						rt->getDestSeqNum(rreqPackage->getDestinationLABILE()) > rreqPackage->getDestinationSeqNum() ?
							rt->getDestSeqNum(rreqPackage->getDestinationLABILE()) : rreqPackage->getDestinationSeqNum());
	}

	else if(rt->getNextHop(rreqPackage->getDestinationLABILE())){
		//current node has an active route to the destination
		//refer to RFC3561 chapter 6.6.2
		setRreqBoardcastDropTimer(rreqPackage->getDestinationLABILE(), rreqPackage->getSourceLABILE(), rreqPackage->getRreqID());
		sendRrepPacket(rt->getHopCount(rreqPackage->getDestinationLABILE()),
						rreqPackage->getSourceLABILE(),
						rreqPackage->getDestinationLABILE(),
						rt->getDestSeqNum(rreqPackage->getDestinationLABILE()),
						getRemainingLifetimeTime(rreqPackage->getDestinationLABILE())-simTime().dbl(),
						false);
	} else {
		//current node is the destination
		//refer to RFC3561 chapter 6.6.1
		if(rreqPackage->getDestinationSeqNum()==currSeqNum){
			currSeqNum++;
		}
		setRreqBoardcastDropTimer(rreqPackage->getDestinationLABILE(), rreqPackage->getSourceLABILE(), rreqPackage->getRreqID());
		sendRrepPacket(0, rreqPackage->getSourceLABILE(), SELF_NETWORK_ADDRESS,
						currSeqNum, 2 * activeRouteTimeout,  false);
	}
}

void LabileRouting::processLabileRrepPackage(LabileRREPPacket * rrepPackage,int srcMacAddress, double rssi, double lqi){
	//update field WL of RREP
	if(lqi<lqiThreshould)
		rrepPackage->setWeakLinks(rrepPackage->getWeakLinks()+1);
	sendRrepAckPacket(rrepPackage->getSource());
	if(getTimer(LABILE_HELLO_MESSAGE_REFRESH_TIMER).dbl()<=0){
		sendHelloMessage();
	}


	//updates a route to the previous hop without a valid seq number
	updateRTEntry(rrepPackage->getSource(), 0, false, VALID, 1,rrepPackage->getWeakLinks(), rrepPackage->getSource(),NULL);
	//update route for the destination
	updateRTEntry(rrepPackage->getDestinationLABILE(), rrepPackage->getDestinationSeqNum(),
				true, VALID, rrepPackage->getHopCount() + 1,rrepPackage->getWeakLinks(), rrepPackage->getSource(),
				NULL,rrepPackage->getLifetime());
	if (!(string(rrepPackage->getOriginator()).compare(SELF_NETWORK_ADDRESS)==0)){
		//forward the rrep
		sendRrepPacket(rrepPackage->getHopCount() + 1,
						rrepPackage->getOriginator(),
						rrepPackage->getDestinationLABILE(),
						rrepPackage->getDestinationSeqNum(),
						rrepPackage->getLifetime(),
						true);
	} else {
		//routing table already updated for destination
		//send buffered packages for destination
		rreqRetryCount[string(rrepPackage->getDestinationLABILE())] = 0;
		processPackagesInBuffer(rrepPackage->getDestinationLABILE(), false);
		cancelExpireEntry(&rreqRequest,rrepPackage->getDestinationLABILE());
	}
}

void LabileRouting::processPackagesInBuffer(const char * dest, bool drop){
	queue< cPacket* > bufferTemp;
	LabileDataPacket* currPkt;
	while (!TXBuffer.empty()) {
		currPkt = dynamic_cast <LabileDataPacket*>(TXBuffer.front());
		if(currPkt && string(dest).compare(currPkt->getDestinationLABILE())==0){
			if(!drop && rt->getNextHop(dest)){
				currPkt->setDestination(rt->getNextHop(dest));
				toMacLayer(currPkt, resolveNetworkAddress(rt->getNextHop(dest)));
			} else
                cancelAndDelete(currPkt);
		}
		else{
			bufferTemp.push(TXBuffer.front());
		}
		TXBuffer.pop();
		updateLifetimeEntry(dest, activeRouteTimeout);
	}
	while (!bufferTemp.empty()) {
		TXBuffer.push(bufferTemp.front());
		bufferTemp.pop();
	}

}

void LabileRouting::timerFiredCallback(int index){
	switch(index){
	case LABILE_RREQ_BROADCAST_DROP_TIMER:
		rreqBroadcast.pop();
		if(!rreqBroadcast.empty()){
			setTimer(LABILE_RREQ_BROADCAST_DROP_TIMER, rreqBroadcast.top().lifetime - simTime().dbl());
		}
		break;

	case LABILE_RREQ_EXPIRATION_TIMER:
	{
		if (rreqRequest.top().canceled){ 
			rreqRequest.pop();
			if(!rreqRequest.empty()){
				setTimer(LABILE_RREQ_EXPIRATION_TIMER, rreqRequest.top().lifetime - simTime().dbl());
			}
			break;
		}

		string dest1 = string(rreqRequest.top().dest);
		rreqRequest.pop();
		if(!rreqRequest.empty()){
			setTimer(LABILE_RREQ_EXPIRATION_TIMER, rreqRequest.top().lifetime - simTime().dbl());
		}
		if(rreqRetryCount[dest1] < rreqRetries){
			//send another rreq
			currRreqID++;
			sendRreqRequest(0, currRreqID, SELF_NETWORK_ADDRESS, dest1.c_str(),
							currSeqNum, rt->getDestSeqNum(dest1.c_str()));
		} else {
			rreqRetryCount[dest1] = 0;
			processPackagesInBuffer(dest1.c_str(),true);
		}
	}
		break;

	case LABILE_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER:
	{
		if(!rtExpireSeq.top().canceled){
			string dest2 = string(rtExpireSeq.top().destination);
			rteluc[dest2]--;
			if (rteluc[dest2]==0){
				if(rt->getState(dest2.c_str())==VALID){
					rt->updateState(dest2, INVALID);
				}
			}
		}
		rtExpireSeq.pop();
		//it is possible that multiple entry expires at the same time
		while(!rtExpireSeq.empty() && rtExpireSeq.top().lifetime - simTime().dbl()<=0){
			if(!rtExpireSeq.top().canceled){
				string dest3 = string(rtExpireSeq.top().destination);
				rteluc[dest3]--;

				if (rteluc[dest3]==0){
					if(rt->getState(dest3.c_str())==VALID){
						rt->updateState(dest3, INVALID);
					}
				}
			}
			rtExpireSeq.pop();
		}
		if(!rtExpireSeq.empty()){
			setTimer(LABILE_ROUTING_TABLE_ENTRY_EXPIRATION_TIMER, rtExpireSeq.top().lifetime - simTime().dbl());
		}
	}
		break;

	case LABILE_RREQ_RATE_LIMIT_TIMER:
		if(!rreqBuffer.empty()){
			processrreqBuffer();
		}
		break;

	case LABILE_HELLO_MESSAGE_REFRESH_TIMER:
		sendHelloMessage();
		break;

	case LABILE_HELLO_MESSAGE_EXPIRE_TIMER:
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
			setTimer(LABILE_HELLO_MESSAGE_EXPIRE_TIMER, hmExpireSeq.top().lifetime - simTime().dbl());
		}
	}
		break;

	case LABILE_RERR_RATE_LIMIT_TIMER:
		if(!rerrBuffer.empty()){
			toMacLayer(rerrBuffer.front(), resolveNetworkAddress(rerrBuffer.front()->getDestination()));
			rerrBuffer.pop();
			setTimer(LABILE_RERR_RATE_LIMIT_TIMER, (double)1/rerrRatelimit);
		}
		break;

	case LABILE_RREP_ACK_WAIT_TIMER:
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
				setTimer(LABILE_RREP_ACK_WAIT_TIMER, rrepAckExpire.top().lifetime - simTime().dbl());
			}
		}
		break;

	case LABILE_RREQ_BLACKLIST_REMOVE_TIMER:
		{
			string node = string(rreqBlacklistTimeout.top().destination);
			rreqBlacklist.remove(node);
			rreqBlacklistTimeout.pop();
			if(!rreqBlacklistTimeout.empty()){
		                double t = max(0.0, rreqBlacklistTimeout.top().lifetime - simTime().dbl());
				setTimer(LABILE_RREQ_BLACKLIST_REMOVE_TIMER, t);
			}
		}
		break;
	default:
		return;

	}
	return;
}

void LabileRouting::sendHelloMessage(){
	LabileHelloMessage* helloMsg = new LabileHelloMessage("LABILE hello message packet", NETWORK_LAYER_PACKET);
	helloMsg->setSource(SELF_NETWORK_ADDRESS);
	helloMsg->setDestination(BROADCAST_NETWORK_ADDRESS);
	helloMsg->setId(LABILE_HELLO_MESSAGE_PACKET);
	toMacLayer(helloMsg, BROADCAST_MAC_ADDRESS);
	setTimer(LABILE_HELLO_MESSAGE_REFRESH_TIMER, helloInterval);
}

void LabileRouting::processrreqBuffer(){
	string dest=string(rreqBuffer.front()->getDestinationLABILE());
	++rreqRetryCount[dest];
	setRreqBoardcastDropTimer(rreqBuffer.front()->getDestinationLABILE(),
								rreqBuffer.front()->getSourceLABILE(),
								rreqBuffer.front()->getRreqID());
	if(string(rreqBuffer.front()->getSourceLABILE()).compare(SELF_NETWORK_ADDRESS)==0){
		setRreqExpirationTimer(dest.c_str());
	}
	toMacLayer(rreqBuffer.front(), BROADCAST_MAC_ADDRESS);
	rreqBuffer.pop();
	setTimer(LABILE_RREQ_RATE_LIMIT_TIMER, (double)1/rreqRatelimit);
}

void LabileRouting::sendRrepPacket(int hopCount, const char* orig, const char* destAodv,
								unsigned long dSeq, double lifetime,  bool forwarding){
	
	if(!rt->getNextHop(orig)) return;
	LabileRREPPacket *rrepPackage =
					new LabileRREPPacket("LABILE routing RREP packet", NETWORK_LAYER_PACKET);
	rrepPackage->setFlagA(true);
	rrepPackage->setFlagR(false);
	rrepPackage->setPrefixSz(0);
	rrepPackage->setHopCount(hopCount);
	rrepPackage->setOriginator(orig);
	rrepPackage->setDestinationLABILE(destAodv);
	rrepPackage->setDestinationSeqNum(dSeq);
	rrepPackage->setSource(SELF_NETWORK_ADDRESS);
	rrepPackage->setDestination(rt->getNextHop(orig));
	rrepPackage->setLifetime(lifetime);
	rrepPackage->setId(LABILE_RREP_PACKET);
	if(string(destAodv).compare(SELF_NETWORK_ADDRESS)!=0){
		rt->updatePrecursor(destAodv, rt->getNextHop(orig));
	}
	else{
		rrepPackage->setWeakLinks(0);
	}
	if(forwarding){
		updateLifetimeEntry(orig, activeRouteTimeout);
	}

	toMacLayer(rrepPackage, resolveNetworkAddress(rt->getNextHop(orig)));
	setRrepAckTimer(rt->getNextHop(orig));
}

void LabileRouting::processHelloMsg(LabileHelloMessage* msg){
	RoutingTableEntryExpireTimerSet hmeet;
	hmeluc[string(msg->getSource())]++;
	hmeet.destination = string(msg->getSource());
	hmeet.lifetime = allowedHelloLoss * helloInterval + simTime().dbl();
	hmExpireSeq.push(hmeet);
	if(getTimer(LABILE_HELLO_MESSAGE_EXPIRE_TIMER).dbl()<=0){
		setTimer(LABILE_HELLO_MESSAGE_EXPIRE_TIMER, hmExpireSeq.top().lifetime - simTime().dbl());
	}
}

void LabileRouting::sendRerrMessage(list<string>* affDest, list<string>* affPrecur){
	if(affDest->empty()||affPrecur->empty()){
		return;
	}
	for (list<string>::const_iterator i=affPrecur->begin();i!=affPrecur->end();++i){
		sendSingleRerrMessage(affDest, affPrecur, (*i).c_str());
	}
}

void LabileRouting::sendSingleRerrMessage(list<string>* affDest, list<string>* affPrecur, const char* dest){
	int destCnt = affDest->size();
	LabileRERRPacket *rerrPackage =
						new LabileRERRPacket("LABILE routing RERR packet", NETWORK_LAYER_PACKET);
	rerrPackage->setSource(SELF_NETWORK_ADDRESS);
	rerrPackage->setFlagN(true);
	rerrPackage->setDestCount(destCnt);
	rerrPackage->setUnreachableDestAddrArraySize(destCnt);
	rerrPackage->setUnreachableDestSeqNumArraySize(destCnt);
	rerrPackage->setDestination(dest);
	rerrPackage->setId(LABILE_RERR_PACKET);
	for(int i=0;i<destCnt;i++){
		rerrPackage->setUnreachableDestAddr(i,affDest->front().c_str());
		rerrPackage->setUnreachableDestSeqNum(i,rt->getDestSeqNum(affDest->front().c_str()));
		affDest->pop_front();
	}
	if (getTimer(LABILE_RERR_RATE_LIMIT_TIMER).dbl() <= 0){
		toMacLayer(rerrPackage, resolveNetworkAddress(dest));
		setTimer(LABILE_RERR_RATE_LIMIT_TIMER, (double)1/rerrRatelimit);
	} else {
		rerrBuffer.push(rerrPackage);
	}
}

void LabileRouting::processLabileRerrPackage(LabileRERRPacket* rerrPacket,int srcMacAddress, double rssi, double lqi){
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

void LabileRouting::sendRrepAckPacket(const char* neib){
	LabileRREPAckPacket *rrepaPacket =
						new LabileRREPAckPacket("LABILE routing RREP ACK packet", NETWORK_LAYER_PACKET);
	rrepaPacket->setSource(SELF_NETWORK_ADDRESS);
	rrepaPacket->setDestination(neib);
	rrepaPacket->setId(LABILE_RREP_ACK_PACKET);
	toMacLayer(rrepaPacket, resolveNetworkAddress(neib));
}
