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
packet LabileRREQPacket extends PacketId {
	bool flagJ;
	bool flagR;
	bool flagG;
	bool flagD;
	bool flagU;
	int hopCount;
	int weakLinks;
	int rreqID;
	string sourceLABILE;
	unsigned long sourceSeqNum;
	unsigned long destinationSeqNum;
	string destinationLABILE;
}
//id =5
packet LabileHelloMessage extends PacketId{

}
//id =4
packet LabileRREPAckPacket extends PacketId{

}

//id =3
packet LabileRERRPacket extends PacketId {
	bool flagN;
	int destCount;
	string unreachableDestAddr[];
	unsigned long unreachableDestSeqNum[];
}



