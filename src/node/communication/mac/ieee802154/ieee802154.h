#ifndef _IEEE802154_H_
#define _IEEE802154_H_

#include <omnetpp.h>
#include "VirtualMac.h"
#include "ieee802154Packet_m.h"
#include <ieee802_15_4.h>

class IEEE802154: public VirtualMac
{
    static const unsigned int CSMA_CA_MIN_BACKOFF_EXPONENT = 3;
    static const unsigned int CSMA_CA_MAX_BACKOFF_EXPONENT = 5;
    static const unsigned int CSMA_CA_UNIT_BACKOFF_PERIOD = 320; // us
    static const unsigned int CSMA_CA_RETRIES = 4;

    static const unsigned int ACK_TIMEOUT = 352 + (127 + _SYS::IEEE802_15_4::PHY_HEADER_SIZE) * 1000000ull / _SYS::IEEE802_15_4::BYTE_RATE;

    enum Timers
    {
        SEND,
        CCA,
        CCA_END,
        ACK_TIMED_OUT,
    };

protected:
    void startup();
    void finishSpecific();
    void fromRadioLayer(cPacket *, double, double);
    void fromNetworkLayer(cPacket *, int);
    void send(IEEE802154Packet *);
    void timerFiredCallback(int);
    void backoff_and_send();
    void cca_start();
    void cca_end();
    void retry();

    static unsigned int _seq;
    bool _idle;
    unsigned int backoff;
    unsigned int exp;
    unsigned int _retry;

    unsigned int _retries;
    unsigned int _acks_sent;
    unsigned int _acks_received;
    unsigned int _data_received;
    unsigned int _tx_ok;
    unsigned int _tx_drop;
};

#endif
