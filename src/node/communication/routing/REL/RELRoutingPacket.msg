 //***********************************************************************************************
 //*  Copyright (c) Federal University of Para, brazil - 2012                                    *
 //*  Developed at the Research Group on Computer Network and Multimedia Communication (GERCOM)  *
 //*  All rights reserved                                                                        *
 //*                                                                                             *
 //*  Permission to use, copy, modify, and distribute this protocol and its documentation for    *
 //*  any purpose, without fee, and without written agreement is hereby granted, provided that   *
 //*  the above copyright notice, and the author appear in all copies of this protocol.          *
 //*                                                                                             *
 //*  Author(s): Kassio Machado <kassio.comp@gmail.com> ,                                        *
 //*             Thiago Oliveira <thiagofdso.ufpa@gmail.com>                                     *
 //***********************************************************************************************

cplusplus {{

#include "PacketId_m.h"
}}

class PacketId;
//id =1
packet RELRREQPacket extends PacketId {
	int hopCount;
	int linkCost;
	int rreqID;
	int energy;
	string sourceREL;
	unsigned long sourceSeqNum;
	unsigned long destinationSeqNum;
	string destinationREL;
}
//id =5
packet RELHelloMessage extends PacketId{

}
//id =4
packet RELRREPAckPacket extends PacketId{

}

//id =3
packet RELRERRPacket extends PacketId {	
	int destCount;
	string unreachableDestAddr[];
	unsigned long unreachableDestSeqNum[];
}

//id =7
packet RELRADVPacket extends PacketId{
	int energy;
	string originator;

}

