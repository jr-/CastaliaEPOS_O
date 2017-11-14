//****************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2012                         *
//*  Developed at the ATP lab, Networked Systems research theme              *
//*  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
//*  This file is distributed under the terms in the attached LICENSE file.  *
//*  If you do not find this file, copies can be found by writing to:        *
//*                                                                          *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
//*      Attention:  License Inquiry.                                        *
//*                                                                          *  
//****************************************************************************/

cplusplus {{
#include "MacPacket_m.h"
}}

class MacPacket;

enum IEEE802154Packet_type {
	IEEE802154_BEACON_PACKET = 1001;
	IEEE802154_ASSOCIATE_PACKET = 1002;
	IEEE802154_DATA_PACKET = 1003;
	IEEE802154_ACK_PACKET = 1004;
	IEEE802154_GTS_REQUEST_PACKET = 1005;
}

packet IEEE802154Packet extends MacPacket {
	int IEEE802154PacketType enum (IEEE802154Packet_type);
	bool ackRequest;
	char seqNum;
}

