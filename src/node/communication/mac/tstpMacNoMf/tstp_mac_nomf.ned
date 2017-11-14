//****************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2010                         *
//*  Developed at the ATP lab, Networked Systems research theme              *
//*  Author(s): Yuriy Tselishchev                                            *
//*  This file is distributed under the terms in the attached LICENSE file.  *
//*  If you do not find this file, copies can be found by writing to:        *
//*                                                                          *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
//*      Attention:  License Inquiry.                                        *
//*                                                                          *  
//****************************************************************************/

package node.communication.mac.tstpMacNoMf;

simple TSTP_MAC_NOMF_NED like node.communication.mac.iMac {
 parameters:
	bool collectTraceInfo = default (false);
	int macMaxPacketSize = default (127);
	int macPacketOverhead = default (11);
	int macBufferSize = default (128);
	
	double period = default (0.150s) @unit(us);

    // radio engine parameters
    bool enableCrc = default (false);
    
 gates:
	output toNetworkModule;
	output toRadioModule;
	input fromNetworkModule;
	input fromRadioModule;
	input fromCommModuleResourceMgr;
}

