#ifndef CASTALIA_NIC_H_
#define CASTALIA_NIC_H_

#include <epos_common.h>
#include <ieee802_15_4.h>

#include <omnetpp.h>
#include "VirtualMac.h"
#include "VirtualMobilityManager.h"
#include <ApplicationPacket_m.h>
#include <TSTPPacket_m.h>
#include <TSTPMACPacket_m.h>
#include <Radio.h>

__USING_SYS

class Timer: public VirtualMac
{
protected:
    enum TIMERS {
        TIMER_INTERRUPT,
        CCA_START,
        CCA_UPDATE,
        CCA_END,
        WATCHDOG,
        TRACK_CLOCK_ERROR_INTERRUPT,
    };

public:
    static const int64 CLOCK = 100000000;
    static const int TIMER_EXPONENT = -8;
    bool watchdog_enabled;
    static const bool track_clock_error = true;
    static constexpr double TRACK_CLOCK_ERROR_PERIOD = 1.0;
    static const unsigned int CCA_TIME = 160;

    typedef NIC_Common::Timer::Time_Stamp Time_Stamp;
    typedef NIC_Common::Timer::Offset Offset;
    typedef NIC_Common::Timer::Microsecond Microsecond;

    Time_Stamp frequency() { return CLOCK; }

    Time_Stamp read();
    void adjust(double t2, double t3);

    void register_clock_error();

    double frequency_error() { return _frequency_error_divider; }
    Timer(unsigned int unit);

    void interrupt(const Time_Stamp & when, const Interrupt_Handler & h);
    void timerFiredCallback(int state);
    void int_disable();
    void schedule_cca(const Time_Stamp & when, const Time_Stamp & time, const Interrupt_Handler & callback);

    static Time_Stamp us2count(const Microsecond & us);
    static Microsecond count2us(const Time_Stamp & ts);

    // Watchdog functions
    void enable() {
        if(watchdog_enabled)
            setTimer(WATCHDOG, _watchdog_period);
    }

    void kick() {
        if(watchdog_enabled) {
            cancelTimer(WATCHDOG);
            enable();
        }
    }

protected:
    Offset _offset;
    double _frequency_error_divider;
    double _t0;
    double _t1;
    Interrupt_Handler _handler;
    static Timer * _instance[2048];
    static unsigned int _instances;
    class Radio_Engine * _radio;
    unsigned int _unit;
    simtime_t _watchdog_period;
    double _average_clock_error;
    unsigned int _n_clock_error_registers;
    bool _adjusted;
    cOutVector * _timer_error_vector;
};

class Radio_Engine: public Timer
{
    friend class Timer;
protected:
    static const unsigned int TX_TO_RX_DELAY = 2; // Radio takes extra 2us to go from TX to RX or idle
    static const unsigned int RX_TO_TX_DELAY = 10;
    static const unsigned int SLEEP_TO_TX_DELAY = 194;
    static const unsigned int SLEEP_TO_RX_DELAY = 194;
    static const unsigned int TX_TO_SLEEP_DELAY = 50;

    typedef uint8 Reg8;
    typedef uint16 Reg16;
    typedef uint32 Reg32;
    typedef uint64 Microsecond;

    // Should be called by the MAC
    void start();
    void end();

public:
    Radio_Engine(unsigned int unit) : Timer(unit), _power(PM_OFF), _crc_errors(0), _channel_clear(false) {}

    bool tx_done();
    bool cca(const Microsecond & time = 0);
    void transmit_no_cca();
    bool transmit();
    void listen();
    void power(const Power_Mode & mode, bool delay = false);
    void channel(unsigned int c);
    void copy_to_nic(const void * frame, unsigned int size);
    unsigned int copy_from_nic(TSTPMACPacket * pkt, void * frame);

protected:
    Power_Mode _power;
    unsigned int _crc_errors;
    bool _enable_crc;
    bool _channel_clear;
};

#endif /* CASTALIA_NIC_H_ */
