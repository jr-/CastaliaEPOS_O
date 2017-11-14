#include <tstp_mac.h>

Define_Module(TSTP_MAC_NED);

unsigned int TSTP_MAC_NED::_units = 0;

void TSTP_MAC_NED::startup()
{
    trace() << "TSTPMAC::startup()";

    Radio_Engine::start();

    first_send = 0;

    int_disable();
    toRadioLayer(createRadioCommand(SET_STATE, SLEEP));

    string st = getParentModule()->getSubmodule("Radio")->par("stateAfterTX");
    if(st.compare("RX") == 0)
        radio_sleeps_after_tx = false;
    else if(st.compare("SLEEP") == 0)
        radio_sleeps_after_tx = true;
    else
        throw cRuntimeError("stateAfterTX parameter not valid!");

    watchdog_enabled = par("enableWatchdog");
    //always_tx_data = par("alwaysTxData");

    RoutingPacket * pkt = new RoutingPacket("TSTP pointer setup", NETWORK_LAYER_PACKET);
    pkt->setBitLength(reinterpret_cast<int64>(reinterpret_cast<NIC*>(this)));
    trace() << "TSTPMAC::startup(): this = " << reinterpret_cast<int64>(reinterpret_cast<NIC*>(this));
    toNetworkLayer(pkt);

    int mfs = par("microframeSize");

    if(mfs < 0)
        MICROFRAME_SIZE = sizeof(Microframe);
    else
        MICROFRAME_SIZE = mfs;

    SLEEP_TO_TX_DELAY = INT_HANDLING_DELAY + Radio_Engine::SLEEP_TO_TX_DELAY;
    RX_TO_TX_DELAY = INT_HANDLING_DELAY + Radio_Engine::RX_TO_TX_DELAY;
    SLEEP_TO_RX_DELAY = Radio_Engine::SLEEP_TO_RX_DELAY;

    // Time to send a single Microframe (including PHY headers)
    Ts = (MICROFRAME_SIZE + Phy_Layer::PHY_HEADER_SIZE) * 1000000ull / Phy_Layer::BYTE_RATE;
    MICROFRAME_TIME = Ts;

    INT_HANDLING_DELAY = par("intHandlingDelay"); // Time delay between scheduled tx_mf interrupt and actual Radio TX

    unsigned int radio_delay = INT_HANDLING_DELAY + 1;
    if(radio_sleeps_after_tx)
        radio_delay += Radio_Engine::SLEEP_TO_TX_DELAY + Radio_Engine::TX_TO_SLEEP_DELAY;
    else
        radio_delay += Radio_Engine::RX_TO_TX_DELAY + Radio_Engine::TX_TO_RX_DELAY;

    Ti = (radio_delay > Tu) ? radio_delay : Tu;
    TIME_BETWEEN_MICROFRAMES = Ti;

    Tr = 2*Ts + Ti + Radio_Engine::SLEEP_TO_RX_DELAY + (PROCESSING_DELAY * 1000000);
    RX_MF_TIMEOUT = Tr;

    NMF = par("NMF");
    if(NMF == 0) {
        unsigned int DESIRED_PERIOD = par("period");
        if(DESIRED_PERIOD == 0) {
            // Calculate period based on user-defined dutyCycle
            DESIRED_DUTY_CYCLE = par("dutyCycle").doubleValue() * 1000000.0;

            NMF = 1 + (((1000000ull * Tr) / DESIRED_DUTY_CYCLE) + (Ti + Ts) - 1) / (Ti + Ts);
            if(NMF > Microframe::COUNT_MAX + 1)
                NMF = Microframe::COUNT_MAX + 1;
            N_MICROFRAMES = NMF;

            CI = Ts + (NMF - 1) * (Ts + Ti);
            PERIOD = CI;

            DUTY_CYCLE = Tr * 1000000ull / CI;

            if(DUTY_CYCLE > DESIRED_DUTY_CYCLE)
                throw cRuntimeError("TSTP_MAC is unable to provide a duty cycle smaller than or equal to DESIRED_DUTY_CYCLE");

        } else {
            // Calculate duty cycle based on user-defined period
            NMF = 1 + ((DESIRED_PERIOD - Ts) / (Ti + Ts));
            if(NMF > Microframe::COUNT_MAX + 1)
                NMF = Microframe::COUNT_MAX + 1;
            N_MICROFRAMES = NMF;

            CI = Ts + (NMF - 1) * (Ts + Ti);
            PERIOD = CI;

            if(PERIOD > DESIRED_PERIOD)
                throw cRuntimeError("TSTP_MAC is unable to provide a period smaller than or equal to DESIRED_PERIOD");

            DUTY_CYCLE = Tr * 1000000ull / CI;
        }
    } else {
        if(NMF > Microframe::COUNT_MAX + 1)
            NMF = Microframe::COUNT_MAX + 1;
        N_MICROFRAMES = NMF;
        CI = Ts + (NMF - 1) * (Ts + Ti);
        PERIOD = CI;
        DUTY_CYCLE = Tr * 1000000ull / CI;
    }

    SLEEP_PERIOD = CI - RX_MF_TIMEOUT;

    DATA_LISTEN_MARGIN = (TIME_BETWEEN_MICROFRAMES + MICROFRAME_TIME) * par("dataListenMarginMultiplier").doubleValue(); // Subtract this amount when calculating time until data transmission
    if(DATA_LISTEN_MARGIN < PROCESSING_DELAY * 1000000)
        DATA_LISTEN_MARGIN = PROCESSING_DELAY * 1000000;
    //DATA_SKIP_TIME = DATA_LISTEN_MARGIN + 4500;

    DATA_SKIP_TIME = (Phy_Layer::MTU + Phy_Layer::PHY_HEADER_SIZE) * 1000000ull / Phy_Layer::BYTE_RATE;

    RX_DATA_TIMEOUT = DATA_SKIP_TIME + DATA_LISTEN_MARGIN;

    CCA_TIME = Ti + 3*CCA_TX_GAP + Radio_Engine::SLEEP_TO_RX_DELAY;

    synchronous_optimization = par("enableSynchronousOptimization");
    MAXIMUM_DRIFT = par("synchronousOptimizationMaximumDrift");
    Mmf = 2 * ((MAXIMUM_DRIFT + Ts + Ti + 1) / (Ts + Ti));

    random_backoff = par("randomBackoff");
    OFFSET_LOWER_BOUND = CCA_TX_GAP;
    OFFSET_GENERAL_LOWER_BOUND = 2 * OFFSET_LOWER_BOUND;
    OFFSET_UPPER_BOUND = SLEEP_PERIOD - SLEEP_TO_RX_DELAY - CCA_TIME - RX_TO_TX_DELAY - MICROFRAME_TIME;
    OFFSET_GENERAL_UPPER_BOUND = OFFSET_UPPER_BOUND;

    if(_unit == 0) {
        std::cout << "NMF = " << NMF << std::endl;
        std::cout << "CI = " << CI << std::endl;
        std::cout << "DUTY_CYCLE = " << DUTY_CYCLE << std::endl;
    }
    MAC::constructor_epilogue();
}

void TSTP_MAC_NED::finishSpecific()
{
    int_disable();

//    if(_tx_schedule.size())
//        throw cRuntimeError("Buffers in TX Schedule!");
    recordScalar("TX Schedule final size", _tx_schedule.size());
    recordScalar("Microframes sent", _mfs_sent);
    recordScalar("Data frames sent", _data_sent);
    recordScalar("Hash collisions", _hash_collisions);
    recordScalar("Max times txed", _max_times_txed);

    recordScalar("drop_expired", drop_expired);
    recordScalar("sniffer", sniffer);
    recordScalar("state_machine_debugged", state_machine_debugged);
    recordScalar("First data frame send time", first_send);
    recordScalar("Last data frame send time", last_send);
    recordScalar("Radio RX processing delay", PROCESSING_DELAY);

    if(_unit == 0) {
        recordScalar("Global Keep Alives sent", _global_keep_alives_sent);
        recordScalar("MICROFRAME_SIZE", MICROFRAME_SIZE);
        recordScalar("DESIRED_DUTY_CYCLE", DESIRED_DUTY_CYCLE);
        recordScalar("INT_HANDLING_DELAY", INT_HANDLING_DELAY);
        recordScalar("SLEEP_TO_TX_DELAY", SLEEP_TO_TX_DELAY);
        recordScalar("RX_TO_TX_DELAY", RX_TO_TX_DELAY);
        recordScalar("Tu", Tu);
        recordScalar("Ti", Ti);
        recordScalar("TIME_BETWEEN_MICROFRAMES", TIME_BETWEEN_MICROFRAMES);
        recordScalar("Ts", Ts);
        recordScalar("MICROFRAME_TIME", MICROFRAME_TIME);
        recordScalar("Tr", Tr);
        recordScalar("RX_MF_TIMEOUT", RX_MF_TIMEOUT);
        recordScalar("NMF", NMF);
        recordScalar("N_MICROFRAMES", N_MICROFRAMES);
        recordScalar("CI", CI);
        recordScalar("PERIOD", PERIOD);
        recordScalar("SLEEP_PERIOD", SLEEP_PERIOD);
        recordScalar("DUTY_CYCLE", DUTY_CYCLE);
        recordScalar("DATA_LISTEN_MARGIN", DATA_LISTEN_MARGIN);
        recordScalar("DATA_SKIP_TIME", DATA_SKIP_TIME);
        recordScalar("RX_DATA_TIMEOUT", RX_DATA_TIMEOUT);
        recordScalar("G", G);
        recordScalar("CCA_TIME", CCA_TIME);
        recordScalar("OFFSET_GENERAL_LOWER_BOUND", OFFSET_GENERAL_LOWER_BOUND);
        recordScalar("OFFSET_GENERAL_UPPER_BOUND", OFFSET_GENERAL_UPPER_BOUND);
        recordScalar("OFFSET_UPPER_BOUND", OFFSET_UPPER_BOUND);
        recordScalar("OFFSET_LOWER_BOUND", OFFSET_LOWER_BOUND);
        recordScalar("Preamble time", ((double)N_MICROFRAMES * ((double)MICROFRAME_TIME + (double)TIME_BETWEEN_MICROFRAMES) / 1000000.0));
    }

    if(_unit == 0) {
        unsigned long long sum = 0;
        unsigned long long sum_sq = 0;
        unsigned long long hash_collision_sum = _hash_collisions;
        unsigned long long crc_error_sum = _crc_errors;

        // Do not count the sink for fairness index
        for(unsigned int i = 1; i < _instances; i++) {
            TSTP_MAC<Radio_Engine> * mac = _instance[i];
            sum += statistics().tx_packets;
            sum_sq += statistics().tx_packets * statistics().tx_packets;
            hash_collision_sum += mac->_hash_collisions;
            crc_error_sum += mac->_crc_errors;
        }
        double num = sum * sum;
        double den = (_instances - 1) * sum_sq;
        num /= den;
        recordScalar("Fairness Index", num);
        recordScalar("Total hash collisions", hash_collision_sum);
        recordScalar("Instances", _instances);
    }

    Radio_Engine::end();

    _units = 0;
}
