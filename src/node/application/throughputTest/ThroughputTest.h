/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#ifndef _THROUGHPUTTEST_H_
#define _THROUGHPUTTEST_H_

#include "VirtualApplication.h"
#include <map>

using namespace std;

enum ThroughputTestTimers {
	SEND_PACKET = 1,

};

class ThroughputTest: public VirtualApplication {
        static const unsigned int RECEIVED_SEQ_NUMS_SIZE = 10000;

 private:
	int packet_rate;
	int packet_limit;
	double startupDelay;
	double delayLimit;
	float packet_spacing;
	int dataSN;
	int recipientId;
	string recipientAddress;
	bool router;
	int pacotesrecebidos;
	bool sink;
	simtime_t etedmean;
	
	//variables below are used to determine the packet delivery rates.	
	int numNodes;
	map<long,int> packetsReceived;
	map<long,int> bytesReceived;
	map<long,int> packetsSent;

	simtime_t tempolimite;


 protected:
	void startup();
	void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
	void handleRadioControlMessage(RadioControlMessage *);
	void timerFiredCallback(int);
	void finishSpecific();

 public:
	int getPacketsSent(int addr) { return packetsSent[addr]; }
	int getPacketsReceived(int addr) { return packetsReceived[addr]; }
	int getBytesReceived(int addr) { return bytesReceived[addr]; }
	
 private:
	bool isDup(int sequence_number);
	std::vector<int> received_seq_nums;
	static unsigned int _global_packets_sent;
};

#endif				// _THROUGHPUTTEST_APPLICATIONMODULE_H_
