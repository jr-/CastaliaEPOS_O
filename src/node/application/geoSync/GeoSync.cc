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

#include <geoSync/GeoSync.h>

#include "GeoSyncPacketType_m.h"

Define_Module(GeoSync);

unsigned int GeoSync::globalSentDataPackets = 0;
unsigned int GeoSync::globalSentPtpPackets = 0;
unsigned int GeoSync::globalSentPtpSyncPackets = 0;
unsigned int GeoSync::globalSentHecopsPackets = 0;
unsigned int GeoSync::globalSentPackets = 0;
unsigned int GeoSync::globalReceivedDataPackets = 0;
unsigned int GeoSync::globalReceivedPtpPackets = 0;
unsigned int GeoSync::globalReceivedHecopsPackets = 0;
unsigned int GeoSync::globalReceivedPackets = 0;

GeoSync::GeoSync()
    : nextPtpSyncTime(0),
        receivedDataPackets(0),
        receivedPtpPackets(0),
        receivedHecopsPackets(0),
        receivedPackets(0),
        sentDataPackets(0),
        sentPtpPackets(0),
        sentPtpSyncPackets(0),
        sentHecopsPackets(0),
        sentPackets(0),
        etedMin(1000000000, SIMTIME_US),
        etedMean(0),
        etedMax(0),
        etedVector("End-to-end Delay")
{
    globalSentDataPackets = 0;
    globalSentPtpPackets = 0;
    globalSentPtpSyncPackets = 0;
    globalSentHecopsPackets = 0;
    globalSentPackets = 0;
    globalReceivedDataPackets = 0;
    globalReceivedPtpPackets = 0;
    globalReceivedHecopsPackets = 0;
    globalReceivedPackets = 0;
}

GeoSync::~GeoSync()
{
    delete dataSource;
}

void GeoSync::startup()
{
    ptpPeriod = par("ptpPeriod");
    ptpMaster = par("ptpMaster");
    hecopsPeriod = par("hecopsPeriod");
    dataPeriod = par("dataPeriod");
    double startupDelay = par("startupDelay");
    double startupRandomDelay = par("startupRandomDelay");
    recipientAddress = par("nextRecipient").stringValue();
    dataPacketLength = par("dataPacketLength");
    ptpSyncPacketLength = par("ptpSyncPacketLength");
    ptpDelayReqPacketLength = par("ptpDelayReqPacketLength");
    ptpDelayRespPacketLength = par("ptpDelayRespPacketLength");
    hecopsPacketLength = par("hecopsPacketLength");
    packetTimeLimit = par("packetTimeLimit");
    dataPacketLimit = par("dataPacketLimit");
    hecopsPacketLimit = par("hecopsPacketLimit");
    ptpSyncPacketLimit = par("ptpSyncPacketLimit");
    dataPacketGenerationChance = par("dataPacketGenerationChance");
    string iotSeriesName = par("LISHAIoTSeriesName");
    dataSource = new LISHA_IoT_Connector(iotSeriesName.c_str());

    if(ptpMaster && (ptpPeriod > 0))
        setTimer(PTPPERIOD, startupDelay + uniform(0, startupRandomDelay));
    if(hecopsPeriod > 0)
        setTimer(HECOPSPERIOD, startupDelay + uniform(0, startupRandomDelay));
    if(dataPeriod > 0)
        setTimer(DATAPERIOD, startupDelay + uniform(0, startupRandomDelay));
}

void GeoSync::finishSpecific()
{
    recordScalar("PTP Period", ptpPeriod);
    recordScalar("HECOPS Period", hecopsPeriod);
    recordScalar("Data Period", dataPeriod);
    recordScalar("Data Packet Generation Chance", dataPacketGenerationChance);

    recordScalar("Data Packet Limit", dataPacketLimit);
    recordScalar("Packet Time Limit", packetTimeLimit);
    recordScalar("ptpMaster", ptpMaster);

    recordScalar("Data Packets Sent", sentDataPackets);
    recordScalar("PTP Packets Sent", sentPtpPackets);
    recordScalar("HECOPS Packets Sent", sentHecopsPackets);
    recordScalar("Total Packets Sent", sentPackets);

    if(self == 0) {
        recordScalar("Global Data Packets Sent", globalSentDataPackets);
        recordScalar("Global PTP Packets Sent", globalSentPtpPackets);
        recordScalar("Global HECOPS Packets Sent", globalSentHecopsPackets);
        recordScalar("Global Total Packets Sent", globalSentPackets);
    }

    recordScalar("Data Packets Received", receivedDataPackets);
    recordScalar("PTP Packets Received", receivedPtpPackets);
    recordScalar("HECOPS Packets Received", receivedHecopsPackets);
    recordScalar("Total Packets Received", receivedPackets);

    if(self == 0) {
        recordScalar("Global Data Packets Received", globalReceivedDataPackets);
        recordScalar("Global PTP Packets Received", globalReceivedPtpPackets);
        recordScalar("Global HECOPS Packets Received", globalReceivedHecopsPackets);
        recordScalar("Global Total Packets Received", globalReceivedPackets);
    }

    if(receivedDataPackets) {
        recordScalar("E-T-E Delay Min", etedMin);
        recordScalar("E-T-E Delay Mean", etedMean / receivedDataPackets);
        recordScalar("E-T-E Delay Max", etedMax);
        double num = globalReceivedDataPackets;
        double den = globalSentDataPackets;
        recordScalar("Delivery Ratio", num / den);
    }

    if(self == 0) {
        unsigned int numNodes = getParentModule()->getParentModule()->par("numNodes");
        unsigned long long sum = 0;
        unsigned long long sum_sq = 0;

        cTopology topo("topo");    // temp variable to access packets sent by other nodes
        topo.extractByNedTypeName(cStringTokenizer("node.Node").asVector());

        // Do not count the sink
        for(unsigned int i = 1; i < numNodes; i++) {
            Radio * r = dynamic_cast<Radio*>(topo.getNode(i)->getModule()->getSubmodule("Communication")->getSubmodule("Radio"));
            if(r) {
                trace() << "r";
                sum += r->stats.transmissions;
                sum_sq += r->stats.transmissions * r->stats.transmissions;
            } else
                trace() << "not r";
        }
        double num = sum * sum;
        double den = (numNodes - 1) * sum_sq;
        num /= den;
        recordScalar("Fairness Index", num);
    }
}

void GeoSync::timerFiredCallback(int timer)
{
    if((packetTimeLimit > 0) && (simTime() >= packetTimeLimit))
        return;

    switch(timer) {
    case PTPPERIOD: {
        if((ptpSyncPacketLimit <= 0)
            || (sentPtpSyncPackets < ptpSyncPacketLimit)) {
            setTimer(PTPPERIOD, ptpPeriod);
            sendPtpSync();
        }
    }
    break;
    case HECOPSPERIOD: {
        if((hecopsPacketLimit <= 0)
            || (sentHecopsPackets < hecopsPacketLimit)) {
            setTimer(HECOPSPERIOD, hecopsPeriod);
            sendHecops();
        }
    }
    break;
    case DATAPERIOD: {
        if((dataPacketLimit <= 0) || (sentDataPackets < dataPacketLimit)) {
            setTimer(DATAPERIOD, dataPeriod);
            if(uniform(0, 1) < dataPacketGenerationChance)
                sendData();
        }
    }
    break;
    }
}

void GeoSync::fromNetworkLayer(ApplicationPacket * pkt,
                               const char * src,
                               double rssi,
                               double lqi)
{
    trace() << pkt << " from " << src;
    int sequenceNumber = pkt->getSequenceNumber();

    if(isDup(sequenceNumber))
        return;

    GeoSyncPacketType * pt = dynamic_cast<GeoSyncPacketType *>(pkt);
    if(!pt) {
        processTSTPDataPacket(pkt);
        return;
    }

    receivedPackets++;
    globalReceivedPackets++;

    switch(pt->getType()) {
    case DATAPACKET: {
        processDataPacket(dynamic_cast<DataPacket *>(pkt));
    }
    break;
    case PTPPACKET: {
        processPtpPacket(dynamic_cast<PTPPacket *>(pkt));
    }
    break;
    case HECOPSPACKET: {
        processHecopsPacket(dynamic_cast<HECOPSPacket *>(pkt));
    }
    break;
    }
}

bool GeoSync::isDup(int sequence_number)
{
    for(auto it = received_seq_nums.begin(); it != received_seq_nums.end();
        ++it) {
        if(*it == sequence_number) {
            received_seq_nums.erase(it);
            received_seq_nums.push_back(sequence_number);
            trace() << "Duplicate packet, seq = " << sequence_number;
            return true;
        }
    }

    if(received_seq_nums.size() >= RECEIVED_SEQ_NUMS_SIZE)
        received_seq_nums.erase(received_seq_nums.begin());

    received_seq_nums.push_back(sequence_number);

    return false;
}

void GeoSync::processTSTPDataPacket(ApplicationPacket * pkt)
{
    if(pkt) {
        trace() << "TSTP Data packet received";

        receivedDataPackets++;
        globalReceivedDataPackets++;
        simtime_t eted = (simTime() - pkt->getAppNetInfoExchange().timestamp);
        if(eted < etedMin)
            etedMin = eted;
        if(eted > etedMax)
            etedMax = eted;
        etedMean += eted;
        etedVector.record(eted);
    }
}

void GeoSync::processDataPacket(DataPacket * pkt)
{
    if(pkt) {
        receivedDataPackets++;
        globalReceivedDataPackets++;
        simtime_t eted = (simTime() - pkt->getAppNetInfoExchange().timestamp);
        if(eted < etedMin)
            etedMin = eted;
        if(eted > etedMax)
            etedMax = eted;
        etedMean += eted;
    }
}

void GeoSync::processPtpPacket(PTPPacket * pkt)
{
    if(pkt) {
        receivedPtpPackets++;
        globalReceivedPtpPackets++;

        if(pkt->getPtpType() == PTPSYNC) {
            if((!ptpMaster) && (simTime() >= nextPtpSyncTime)
                && ((ptpPeer == "")
                    || (ptpPeer == pkt->getAppNetInfoExchange().source.c_str()))) {

                nextPtpSyncTime = pkt->getCreationTime() + ptpPeriod;

                PTPPacket * req = new PTPPacket(
                    "GeoSync PTP Delay Request packet", APPLICATION_PACKET);
                req->setType(PTPPACKET);
                req->setPtpType(PTPDELAYREQ);
                req->setSequenceNumber(globalSentPackets);
                req->setByteLength(ptpDelayReqPacketLength);

                globalSentPtpPackets++;
                globalSentPackets++;
                sentPtpPackets++;
                sentPackets++;

                toNetworkLayer(req,
                               pkt->getAppNetInfoExchange().source.c_str());
            }
        } else if(pkt->getPtpType() == PTPDELAYREQ) {
            PTPPacket * req = new PTPPacket("GeoSync PTP Delay Response packet",
                                            APPLICATION_PACKET);
            req->setType(PTPPACKET);
            req->setPtpType(PTPDELAYRESP);
            req->setSequenceNumber(globalSentPackets);
            req->setByteLength(ptpDelayRespPacketLength);

            globalSentPtpPackets++;
            globalSentPackets++;
            sentPtpPackets++;
            sentPackets++;

            toNetworkLayer(req, pkt->getAppNetInfoExchange().source.c_str());
        } else if(pkt->getPtpType() == PTPDELAYRESP) {
            if(!ptpMaster) {

                ptpPeer = pkt->getAppNetInfoExchange().source.c_str();

                ; // Do actual clock synchronization

                if((ptpSyncPacketLimit <= 0)
                    || (sentPtpSyncPackets < ptpSyncPacketLimit)) {
                    sendPtpSync(); // Sync neighbors further from the sink
                }
            }
        }
    }
}

void GeoSync::processHecopsPacket(HECOPSPacket * pkt)
{
    if(pkt) {
        receivedHecopsPackets++;
        globalReceivedHecopsPackets++;
    }
}

void GeoSync::sendPtpSync()
{
    PTPPacket * pkt = new PTPPacket("GeoSync PTP Sync packet",
                                    APPLICATION_PACKET);
    pkt->setType(PTPPACKET);
    pkt->setPtpType(PTPSYNC);
    pkt->setSequenceNumber(globalSentPackets);
    pkt->setByteLength(ptpSyncPacketLength);

    globalSentPtpPackets++;
    globalSentPtpSyncPackets++;
    globalSentPackets++;
    sentPtpPackets++;
    sentPtpSyncPackets++;
    sentPackets++;

    toNetworkLayer(pkt, BROADCAST_NETWORK_ADDRESS);
}

void GeoSync::sendHecops()
{
    HECOPSPacket * pkt = new HECOPSPacket("GeoSync HECOPS packet",
                                          APPLICATION_PACKET);
    pkt->setType(HECOPSPACKET);
    pkt->setSequenceNumber(globalSentPackets);
    pkt->setByteLength(hecopsPacketLength);

    globalSentHecopsPackets++;
    globalSentPackets++;
    sentHecopsPackets++;
    sentPackets++;

    toNetworkLayer(pkt, BROADCAST_NETWORK_ADDRESS);
}

void GeoSync::sendData()
{
    DataPacket * pkt = new DataPacket("GeoSync Data packet",
                                      APPLICATION_PACKET);
    pkt->setData(dataSource->value());
    pkt->setSequenceNumber(globalSentPackets);
    pkt->setByteLength(dataPacketLength);

    globalSentDataPackets++;
    globalSentPackets++;
    sentDataPackets++;
    sentPackets++;

    toNetworkLayer(pkt, recipientAddress.c_str());
}
