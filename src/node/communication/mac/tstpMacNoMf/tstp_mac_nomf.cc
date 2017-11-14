#include "tstp_mac_nomf.h"
#include <tstp.h>

Define_Module(TSTP_MAC_NOMF_NED);

unsigned int TSTP_MAC_NOMF_NED::_units;

void TSTP_MAC_NOMF_NED::startup()
{
    trace() << "TSTP_MAC_NOMF_NED::startup()";

    OFFSET_GENERAL_UPPER_BOUND = OFFSET_UPPER_BOUND = PERIOD = par("period").doubleValue();

    int_disable();
    power(PM_FULL);
    toRadioLayer(createRadioCommand(SET_STATE, RX));

    RoutingPacket * pkt = new RoutingPacket("TSTP pointer setup", NETWORK_LAYER_PACKET);
    pkt->setBitLength(reinterpret_cast<int64>(reinterpret_cast<NIC*>(this)));
    trace() << "TSTP_MAC_NOMF_NED: this = " << reinterpret_cast<int64>(reinterpret_cast<NIC*>(this));
    toNetworkLayer(pkt);

    Radio_Engine::start();

    MAC::constructor_epilogue();
}

void TSTP_MAC_NOMF_NED::finishSpecific()
{
    trace() << "TSTP_MAC_NOMF_NED::finishSpecific()";
    if(_unit == 0) {
        recordScalar("Global Keep Alives sent", _global_keep_alives_sent);
    }
    recordScalar("Hash collisions", _hash_collisions);
    recordScalar("Retries", _total_retries);
    recordScalar("ACKs sent", _acks_sent);
    recordScalar("ACKs received", _acks_received);
    recordScalar("Data packets received", _data_received);
    recordScalar("Data packet successfully sent", statistics().tx_packets);
    recordScalar("Data packets dropped", _tx_drop);
    recordScalar("DUTY_CYCLE", 1000000);
    Radio_Engine::end();
}
