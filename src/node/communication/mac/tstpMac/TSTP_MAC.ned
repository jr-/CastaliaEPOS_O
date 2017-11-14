package node.communication.mac.tstpMac;

simple TSTP_MAC_NED like node.communication.mac.iMac {
 parameters:
     
    // debug parameters
	bool collectTraceInfo = default (false);
	bool printStateTransitions = default (false);
	bool enableWatchdog = default (false);
	
	// mac layer parameters
	int NMF = default(0);
	int period = default(0us) @unit(us); // if period == 0, period is calculated according to dutyCycle
	double dutyCycle = default (0.01);   // if period != 0, dutyCycle is calculated according to period		
	int macMaxPacketSize = default (127);	// Maximum payload for 802.15.4
	int macPacketOverhead = 0;
	int macBufferSize = default (1024); // unused
	int microframeSize = default (-1); // -1 means sizeof(Microframe)
	int intHandlingDelay = default (0);
	double dataListenMarginMultiplier = default(3);
	bool alwaysTxData = default (false);
	bool enableSynchronousOptimization = default (false);
	int synchronousOptimizationMaximumDrift = default (1);

    // radio engine parameters
    bool enableCrc = default (false);
    
	// parameters dependent on physical layer
	double phyDelayForValidCS = default (0.128);
	double phyDataRate = default (250);
	int phyFrameOverhead = default (6);

	// parameters dependent on physical layer
	// some are essential and are not defined as default
	double phyDelaySleep2Tx = default (0.2); //in ms
	double phyDelayRx2Tx = default (0.02);	//in ms

	// random backoff configuration
	bool randomBackoff = default (true);
    double offsetGeneralLowerBound = default (3 * 0.000320s) @unit(us);
    double offsetGeneralUpperBoundReduction = default (3 * 0.000320s) @unit(us);
    double offsetLowerBound = default (0.000320s) @unit(us);

 gates:
	output toNetworkModule;
	output toRadioModule;
	input fromNetworkModule;
	input fromRadioModule;
	input fromCommModuleResourceMgr;
}

