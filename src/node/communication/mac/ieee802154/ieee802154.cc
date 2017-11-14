#include "ieee802154.h"

Define_Module(IEEE802154);

unsigned int IEEE802154::_seq = 0;

void IEEE802154::startup()
{
    _idle = true;
    _retries = 0;
    _acks_sent = 0;
    _acks_received = 0;
    _data_received = 0;
    _tx_ok = 0;
    _tx_drop = 0;
    toRadioLayer(createRadioCommand(SET_STATE, RX));
}

void IEEE802154::finishSpecific()
{
    recordScalar("Retries", _retries);
    recordScalar("ACKs sent", _acks_sent);
    recordScalar("ACKs received", _acks_received);
    recordScalar("Data packets received", _data_received);
    recordScalar("Data packet successfully sent", _tx_ok);
    recordScalar("Data packets dropped", _tx_drop);
}

void IEEE802154::fromNetworkLayer(cPacket * pkt, int destination)
{
    IEEE802154Packet *macFrame = new IEEE802154Packet("IEEE802154 packet",
                                                        MAC_LAYER_PACKET);
    encapsulatePacket(macFrame, pkt);
    macFrame->setSource(SELF_MAC_ADDRESS);
    macFrame->setDestination(destination);
    macFrame->setSequenceNumber(_seq++);

    bufferPacket(macFrame);

    if(_idle)
        setTimer(SEND, 0);
}

void IEEE802154::timerFiredCallback(int timer)
{
    switch(timer) {
    case SEND: {
        if(!TXBuffer.empty())
            backoff_and_send();
    } break;
    case CCA: {
        cca_start();
    } break;
    case CCA_END: {
        cca_end();
    } break;
    case ACK_TIMED_OUT: {
        trace() << "ACK_TIMED_OUT";
        retry();
    } break;
    }
}

void IEEE802154::retry()
{
    if(_retry < CSMA_CA_RETRIES) {
        _retries++;
        setTimer(CCA, simtime_t((uniform(1, backoff)) * CSMA_CA_UNIT_BACKOFF_PERIOD, SIMTIME_US));
    } else {
        cancelAndDelete(TXBuffer.front());
        TXBuffer.pop();
        _idle = true;
        _tx_drop++;
    }
}

void IEEE802154::backoff_and_send()
{
    _idle = false;

    exp = CSMA_CA_MIN_BACKOFF_EXPONENT;
    backoff = pow(2, exp);

    _retry = 0;

    setTimer(CCA, simtime_t((uniform(1, backoff)) * CSMA_CA_UNIT_BACKOFF_PERIOD, SIMTIME_US));
}

void IEEE802154::cca_start()
{
    toRadioLayer(createRadioCommand(SET_STATE, RX));
    setTimer(CCA_END, simtime_t(320, SIMTIME_US));
}

void IEEE802154::cca_end()
{
    _retry++;
    if(exp < CSMA_CA_MAX_BACKOFF_EXPONENT) {
        exp++;
        backoff *= 2;
    }

    if(radioModule->isChannelClear()) {
        toRadioLayer(TXBuffer.front()->dup());
        toRadioLayer(createRadioCommand(SET_STATE, TX));
        if(reinterpret_cast<IEEE802154Packet*>(TXBuffer.front())->getDestination() == BROADCAST_MAC_ADDRESS) {
            cancelAndDelete(TXBuffer.front());
            TXBuffer.pop();
            _idle = true;
            _tx_ok++;
        } else
            setTimer(ACK_TIMED_OUT, simtime_t(ACK_TIMEOUT, SIMTIME_US));
    } else
        retry();

    if(_idle && !TXBuffer.empty())
        setTimer(SEND, 0);
}

void IEEE802154::fromRadioLayer(cPacket * pkt, double rssi, double lqi)
{
    IEEE802154Packet *macPkt = dynamic_cast<IEEE802154Packet*>(pkt);
    if(macPkt == NULL)
        return;

    if(macPkt->getIEEE802154PacketType() == IEEE802154_ACK_PACKET) {
        trace() << "ACK received";
        if((!TXBuffer.empty()) && (macPkt->getSequenceNumber() == reinterpret_cast<IEEE802154Packet*>(TXBuffer.front())->getSequenceNumber())) {
            cancelTimer(CCA);
            cancelTimer(CCA_END);
            cancelTimer(ACK_TIMED_OUT);
            cancelAndDelete(TXBuffer.front());
            TXBuffer.pop();
            _idle = true;
            if(_idle && !TXBuffer.empty())
                setTimer(SEND, 0);

            _tx_ok++;
            _acks_received++;
        }

        cancelAndDelete(macPkt);
        return;
    }

    if(macPkt->getDestination() == SELF_MAC_ADDRESS) {
        IEEE802154Packet * ack = new IEEE802154Packet("IEEE802154 ACK", MAC_LAYER_PACKET);
        ack->setSequenceNumber(macPkt->getSequenceNumber());
        ack->setByteLength(5);
        ack->setIEEE802154PacketType(IEEE802154_ACK_PACKET);

        _acks_sent++;

        toRadioLayer(ack);
        toRadioLayer(createRadioCommand(SET_STATE, TX));
    }

    if(macPkt->getDestination() == SELF_MAC_ADDRESS
        || macPkt->getDestination() == BROADCAST_MAC_ADDRESS) {
        _data_received++;
        toNetworkLayer(decapsulatePacket(macPkt));
    }

    cancelAndDelete(macPkt);
}
