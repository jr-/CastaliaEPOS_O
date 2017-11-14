package node.communication.routing.tstp;

simple TSTP like node.communication.routing.iRouting {
 parameters:
    // Castalia parameters
	bool collectTraceInfo = default (false);	
	int maxNetFrameSize = default (0); // bytes
	int netDataFrameOverhead = default (0); // bytes
	int netBufferSize = default (0); // number of messages
	
	bool sink = default(false);
	
	bool anchor = default (true);

	double expiry = default (30s) @unit(us);
	double keepAlivePeriod = default (5s) @unit(us); // Set to 0 to disable Life_Keeper
	double ptpExplicitSyncPeriod = default (30s) @unit(us);
	double radioRange = default (70m) @unit(cm);
	double expiryBetha = default (1.0);
	
	bool dropExpired = default (true);
	bool usePtpExplicitSync = default (true);
	bool startSynchronized = default (true);

	bool useExpiryMetric = default (true);
	bool useOldExpiryMetric = default (false);
	bool useEffortMetric = default (false);
	bool useRandomMetric = default (false);

    bool encrypt = default (false);
    bool startWithKeysEstablished = default (true);
    double keyExpiry = default (1h) @unit(us);
    double keyManagerPeriod = default (10s) @unit(us);
    double dhRequestProcessingDelay = default (1s) @unit(us);
    double securityTimeWindow = default (60s) @unit(us);
    
 gates:
	output toCommunicationModule;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
}
