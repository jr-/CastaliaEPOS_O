//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef GEOSYNC_H_
#define GEOSYNC_H_

#include "VirtualApplication.h"
#include "PTPPacket_m.h"
#include "HECOPSPacket_m.h"
#include "DataPacket_m.h"
#include "lisha_iot.h"

class GeoSync: public VirtualApplication
{
    static const unsigned int RECEIVED_SEQ_NUMS_SIZE = 10000;

    enum TIMER {
        PTPPERIOD,
        HECOPSPERIOD,
        DATAPERIOD,
    };

    enum PACKETTYPE {
        DATAPACKET,
        PTPPACKET,
        HECOPSPACKET,
    };

    enum PTPTYPE { // TODO
        PTPSYNC,
        PTPDELAYREQ,
        PTPDELAYRESP,
    };

public:
    GeoSync();
    virtual ~GeoSync();

    void startup();
    void finishSpecific();
    void timerFiredCallback(int timer);
    void fromNetworkLayer(ApplicationPacket *, const char *, double, double);


protected:
	bool isDup(int sequence_number);

    void processDataPacket(DataPacket * pkt);
    void processTSTPDataPacket(ApplicationPacket * pkt);
    void processHecopsPacket(HECOPSPacket * pkt);
    void processPtpPacket(PTPPacket * pkt);

    void sendPtpSync();
    void sendHecops();
    void sendData();

    simtime_t nextPtpSyncTime;
    string ptpPeer;

    // NED Parameters
    unsigned int dataPacketLength;
    unsigned int ptpSyncPacketLength;
    unsigned int ptpDelayReqPacketLength;
    unsigned int ptpDelayRespPacketLength;
    unsigned int hecopsPacketLength;
    double dataPacketGenerationChance;

    simtime_t packetTimeLimit;
    unsigned int dataPacketLimit;
    unsigned int hecopsPacketLimit;
    unsigned int ptpSyncPacketLimit;

    string recipientAddress;

    double ptpPeriod;
    double hecopsPeriod;
    double dataPeriod;

    bool ptpMaster;

    // Statistics
    unsigned int receivedDataPackets;
    unsigned int receivedPtpPackets;
    unsigned int receivedHecopsPackets;
    unsigned int receivedPackets;

    unsigned int sentDataPackets;
    unsigned int sentPtpPackets;
    unsigned int sentPtpSyncPackets;
    unsigned int sentHecopsPackets;
    unsigned int sentPackets;

    static unsigned int globalSentDataPackets;
    static unsigned int globalSentPtpPackets;
    static unsigned int globalSentPtpSyncPackets;
    static unsigned int globalSentHecopsPackets;
    static unsigned int globalSentPackets;

    static unsigned int globalReceivedDataPackets;
    static unsigned int globalReceivedPtpPackets;
    static unsigned int globalReceivedHecopsPackets;
    static unsigned int globalReceivedPackets;

    simtime_t etedMin;
    simtime_t etedMean;
    simtime_t etedMax;
    cOutVector etedVector;

    LISHA_IoT_Connector * dataSource;

    // Other
    std::vector<int> received_seq_nums;
};

#endif /* GEOSYNC_H_ */
