 //*
 //* @Author: Mathieu MICHEL (http://w3.umons.ac.be/staff/Michel.Mathieu/)
 //*
 //* Copyright (C) 2013  Mathieu MICHEL
 //* This program is free software; you can redistribute it and/or modify
 //* it under the terms of the GNU General Public License as published by
 //* the Free Software Foundation; either version 2 of the License, or
 //* (at your option) any later version.
 //* 
 //* This program is distributed in the hope that it will be useful,
 //* but WITHOUT ANY WARRANTY; without even the implied warranty of
 //* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 //* GNU General Public License for more details.
 //*
 //* You should have received a copy of the GNU General Public License
 //* along with this program; if not, write to the Free Software
 //* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 //*
 
package node.communication.routing.aodvTestRouting;

simple AodvTestRouting like node.communication.routing.iRouting
{
 parameters:

	// mandatory parameters as defined by iRouting.ned files

	bool collectTraceInfo = default (false);
	int maxNetFrameSize = default (0); // 0 or less means no limit		
	int netBufferSize = default (32);					
	int netDataFrameOverhead = default (10);
	
	bool shortestDelay = default (false);// by defaut the shortest_path mode is used

	// default values for AODV parameters as defined in AODV RFC 3561

	int activeRouteTimeout = default(60000); // in ms (default 60000)
	int allowedHelloLoss = default(4);  // (default 4)
	//int backlist_Timeout = rreqRetries*netTraversalTime
	//int deletePeriod
	int helloInterval = default(25000); // in ms (default 25000)
	int localAddTTL = default(2);
	//int maxRepairTTL = 0.3*netDiameter
	//int minRepairTTL
	int netDiameter = default(35);
	//int netTraversalTime = 2*nodeTraversalTime*netDiameter
	//int nextHopWait = nodeTraversalTime+10
	int nodeTraversalTime = default(4000); //in ms (default 4000)
	//int pathDiscoveryTime = 2*netTraversalTime
	int rerrRatelimit = default(10);
	//int ringTraversalTime = 2*nodeTraversalTime*(ttlValue + timeoutBuffer)
	int rreqRetries = default(2);
	int rreqRatelimit = default(10);
	int timeoutBuffer = default(2);
	int ttlStart = default(1);
	int ttlIncrement = default(2);
	int ttlThreshould = default(7);
	//int ttlValue
	int frameSize = default(155);// S_MAC fram size
	
	double latencyHistogramMax = default (10);
	double latencyHistogramMin = default (0);
	int latencyHistogramBuckets = default (10);
	
	int atmode = default(0); //timers mode 
							 // (0 = default; 
							 // 1 = route timers modification 
							 // 2 = route + rreq timers modification


 gates:
	output toCommunicationModule;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
}

