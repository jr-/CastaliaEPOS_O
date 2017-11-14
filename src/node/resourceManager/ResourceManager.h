/*******************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                            *
 *  Developed at the ATP lab, Networked Systems research theme                 *
 *  Author(s): Athanassios Boulis, Dimosthenis Pediaditakis, Yuriy Tselishchev *
 *  This file is distributed under the terms in the attached LICENSE file.     *
 *  If you do not find this file, copies can be found by writing to:           *
 *                                                                             *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia                *
 *      Attention:  License Inquiry.                                           *
 *                                                                             *  
 *******************************************************************************/

#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <map>
#include <algorithm>
#include "CastaliaModule.h"
#include "ResourceManagerMessage_m.h"

using namespace std;

enum ResourceManagerTimers {
	PERIODIC_ENERGY_CALCULATION = 1,
};

class ResourceManager: public CastaliaModule {
 private:
    class TemperatureDrift {
    public:
        TemperatureDrift(double t, double m = 0, double s = 0) : temperature(t), mean(m), stdDev(s) {}

        double temperature;
        double mean;
        double stdDev;

        bool operator<(const TemperatureDrift & b) const { return temperature < b.temperature; }
    };

    std::set<TemperatureDrift> temperatureModel;
    double lastTemperature;
    double lastDrift;
    simtime_t lastDriftChange;
    simtime_t driftChangeInterval;

	// parameters and variables
	/*--- The .ned file's parameters ---*/
	double sigmaCPUClockDrift;
	double cpuClockDrift;
	double ramSize;
	double baselineNodePower;
	double currentNodePower;
	simtime_t timeOfLastCalculation;
	double periodicEnergyCalculationInterval;

	/*--- Custom class parameters ---*/
	double remainingEnergy;
	double totalRamData;
	double temperature;

	map<int,double> storedPowerConsumptions;

	cMessage *energyMsg;
	bool disabled;

 protected:
	virtual void initialize();
	virtual void handleMessage(cMessage * msg);
	virtual void finishSpecific();
	void calculateEnergySpent();

 private:
	void parseTemperatureModel();

 public:
	double getCPUClockDrift(void);
	double getTemperature();
	void setTemperature(double t);
	double getTemperatureDrift();
	void consumeEnergy(double amount);
	double getSpentEnergy(void);
	double estimateLifetime();
	void destroyNode(void);
	int RamStore(int numBytes);
	void RamFree(int numBytes);
    double initialEnergy;
    double getRemainingEnergy();

};

#endif				// _RESOURCEGENERICMANAGER_H_
