/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include "ThroughputTest.h"

Define_Module(ThroughputTest);

unsigned int ThroughputTest::_global_packets_sent = 0;

void ThroughputTest::startup()
{
    sink = par("sink");
    tempolimite = par("tempolimite");
    router = par("router");

	packet_rate = par("packet_rate");
	recipientAddress = par("nextRecipient").stringValue();
	recipientId = atoi(recipientAddress.c_str());
	startupDelay = par("startupDelay");
	delayLimit = par("delayLimit");
	packet_limit = par("packet_limit");

	packet_spacing = packet_rate > 0 ? packet_rate:-1;
	dataSN = 0;
	pacotesrecebidos = 0;
	numNodes = getParentModule()->getParentModule()->par("numNodes");
	packetsSent.clear();
	packetsReceived.clear();
	bytesReceived.clear();
	etedmean = 0.0;

	if (packet_limit != 0 && packet_spacing > 0 && recipientAddress.compare(SELF_NETWORK_ADDRESS) != 0)
	    setTimer(SEND_PACKET, packet_spacing + uniform(1, 1 + startupDelay));
	else
	    trace() << "Not sending packets";

	declareOutput("Packets received per node");
}

bool ThroughputTest::isDup(int sequence_number)
{
    for(auto it = received_seq_nums.begin(); it != received_seq_nums.end(); ++it) {
        if(*it == sequence_number) {
            received_seq_nums.erase(it);
            received_seq_nums.push_back(sequence_number);
            return true;
        }
    }

    if(received_seq_nums.size() >= RECEIVED_SEQ_NUMS_SIZE)
        received_seq_nums.erase(received_seq_nums.begin());

    received_seq_nums.push_back(sequence_number);

    return false;
}

void ThroughputTest::fromNetworkLayer(ApplicationPacket * rcvPacket,
		const char *source, double rssi, double lqi)
{
	int sequenceNumber = rcvPacket->getSequenceNumber();
	int sourceId = atoi(source);

	if(isDup(sequenceNumber))
	    return;

	pacotesrecebidos++;
	etedmean += (simTime() - rcvPacket->getAppNetInfoExchange().timestamp);

	// This node is the final recipient for the packet
	if (recipientAddress.compare(SELF_NETWORK_ADDRESS) == 0) {
		if (delayLimit == 0 || (simTime() - rcvPacket->getCreationTime()) <= delayLimit) {
			trace() << "Received packet #" << sequenceNumber << " from node " << source;
			collectOutput("Packets received per node", sourceId);
			packetsReceived[sourceId]++;
			bytesReceived[sourceId] += rcvPacket->getByteLength();
		} else {
			trace() << "Packet #" << sequenceNumber << " from node " << source <<
				" exceeded delay limit of " << delayLimit << "s";
		}
	// Packet has to be forwarded to the next hop recipient
	} else {
		ApplicationPacket* fwdPacket = rcvPacket->dup();
		// Reset the size of the packet, otherwise the app overhead will keep adding on
		fwdPacket->setByteLength(0);
		toNetworkLayer(fwdPacket, recipientAddress.c_str());
	}
}

void ThroughputTest::timerFiredCallback(int index)
{
	switch (index) {
		case SEND_PACKET:{
			trace() << "Sending packet #" << dataSN;
			toNetworkLayer(createGenericDataPacket(0, _global_packets_sent, sizeof(uint32)), recipientAddress.c_str());
			packetsSent[recipientId]++;
			dataSN++;
			_global_packets_sent++;

			if((tempolimite == -1) || (getClock() < tempolimite)) {
			    if((packet_limit == -1) || (dataSN < packet_limit))
			        setTimer(SEND_PACKET, packet_spacing);
			}
			else
			    trace() << "Terminou a geração de dados na APP";

			break;
		}
	}
}

// This method processes a received carrier sense interupt. Used only for demo purposes
// in some simulations. Feel free to comment out the trace command.
void ThroughputTest::handleRadioControlMessage(RadioControlMessage *radioMsg)
{
	switch (radioMsg->getRadioControlMessageKind()) {
		case CARRIER_SENSE_INTERRUPT:
			trace() << "CS Interrupt received! current RSSI value is: " << radioModule->readRSSI();
                        break;
	}
}

void ThroughputTest::finishSpecific() {

    recordScalar("Pacotes Enviados",dataSN);
    recordScalar("Global packets sent", _global_packets_sent);
    if(pacotesrecebidos) {
        recordScalar("Pacotes Recebidos",pacotesrecebidos);
        recordScalar("E-T-E Delay",etedmean/pacotesrecebidos);
        recordScalar("Delivery Ratio", (((double)pacotesrecebidos) / ((double)_global_packets_sent)));
    }




    cTopology *topo;    // temp variable to access packets received by other nodes
    topo = new cTopology("topo");
    topo->extractByNedTypeName(cStringTokenizer("node.Node").asVector());

    long bytesDelivered = 0;
    for (int i = 0; i < numNodes; i++) {
        ThroughputTest *appModule = dynamic_cast<ThroughputTest*>
            (topo->getNode(i)->getModule()->getSubmodule("Application"));
        if (appModule) {
            int packetsSent = appModule->getPacketsSent(self);
            if (packetsSent > 0) { // this node sent us some packets
                float rate = (float)packetsReceived[i]/packetsSent;
                collectOutput("Packets reception rate", i, "total", rate);
                collectOutput("Packets loss rate", i, "total", 1-rate);
            }

            bytesDelivered += appModule->getBytesReceived(self);
        }
    }
    delete(topo);

    if (packet_rate > 0 && bytesDelivered > 0) {
        double energy = (resMgrModule->getSpentEnergy() * 1000000000)/(bytesDelivered * 8); //in nanojoules/bit
        declareOutput("Energy nJ/bit");
        collectOutput("Energy nJ/bit","",energy);
        recordScalar("Energy nJ/bit",energy); //in nanojoules/bit
    }
}
