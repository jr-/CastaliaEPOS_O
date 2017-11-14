#include "castalia_nic.h"

Timer * Timer::_instance[2048];
unsigned int Timer::_instances;

void Radio_Engine::start()
{
    _enable_crc = par("enableCrc");
    if((_unit == 0) && track_clock_error)
        setTimer(TRACK_CLOCK_ERROR_INTERRUPT, TRACK_CLOCK_ERROR_PERIOD);
    _timer_error_vector = new cOutVector("Timer Error");
}

void Radio_Engine::end()
{
    if(_enable_crc)
        recordScalar("CRC errors", _crc_errors);

    if((_unit == 0) && track_clock_error && _n_clock_error_registers) {
        _average_clock_error /= (double)_n_clock_error_registers;
        recordScalar("Average clock error (ts)", _average_clock_error);
        recordScalar("Average clock error (us)", Timer::count2us(_average_clock_error));
    }

    delete _timer_error_vector;
}

Timer::Time_Stamp Timer::read()
{
    return (getClock().inUnit(TIMER_EXPONENT) + _offset) / _frequency_error_divider;
}

Timer::Time_Stamp Timer::us2count(const Microsecond & us) { return us * (CLOCK / 1000000ull); }
Timer::Microsecond Timer::count2us(const Time_Stamp & ts) { return ts / (CLOCK / 1000000ull); }

void Timer::adjust(double t2, double t3) {
    if((_t0 == 0) || (_t1 == 0)) {
        _t0 = t2;
        _t1 = t3;
    } else {
        double t3_raw = t3 * _frequency_error_divider - _offset;

        _offset = ((t2 - _t1) - (t3 - _t0)) / (_t0 - t2);
        _frequency_error_divider = (_t1 + _offset) / _t0;

        _t0 = t2;
        _t1 = t3_raw + _offset / _frequency_error_divider;
        _adjusted = true;
    }
    return;
}

Timer::Timer(unsigned int unit) : _offset(0), _frequency_error_divider(1.0), _t0(0), _t1(0), _handler(0), _unit(unit), _watchdog_period(1, SIMTIME_S), _average_clock_error(0), _n_clock_error_registers(0), _adjusted(false)
{
    _instance[_unit] = this;
    if(_unit > _instances)
        _instances = _unit+1;
}

void Timer::interrupt(const Time_Stamp & when, const Interrupt_Handler & h)
{
    int_disable();
    _handler = h;
    simtime_t t(when, TIMER_EXPONENT);
    simtime_t now(read(), TIMER_EXPONENT);
    _radio->trace() << "Timer::interrupt: now = " << now << ", interrupt at = " << t << ", time interval = " << t - now;

    //if(t <= now)
    //    throw cRuntimeError("castalia_nic::Timer::interrupt: t <= now!");

    //assert(t > now);

    if(t <= now) {
        _radio->trace() << "castalia_nic::Timer::interrupt: t <= now! t = " << t << " now = " << now;
        setTimer(TIMER_INTERRUPT, 0);
    }
    else
        setTimer(TIMER_INTERRUPT, t - now);
}

void Timer::register_clock_error()
{
    bool entered = false;
    double den = _instances - 1;
    for(unsigned int i = 0; i < _instances; i++) {
        if((i == _unit) || (!_instance[i]->_adjusted))
            continue;
        entered = true;
        long long error = read() - _instance[i]->read();
        error = abs(error);
        double num = error;
        _average_clock_error += num / den;
    }
    if(entered) {
        _n_clock_error_registers++;
        _timer_error_vector->record(_average_clock_error / _n_clock_error_registers);
    }
}

void Timer::timerFiredCallback(int state)
{
    _radio->trace() << "Timer::timerFiredCallback(s=" << state << "): h=" << _handler << ",u=" << _unit;

    switch(state) {
    case CCA_START:
        _radio->_channel_clear = false;
        _radio->toRadioLayer(createRadioCommand(SET_STATE, RX));
        setTimer(CCA_UPDATE, simtime_t(CCA_TIME, SIMTIME_US));
        break;

    case CCA_UPDATE:
        _radio->_channel_clear = _radio->radioModule->isChannelClear() == CLEAR;
        if(_radio->_channel_clear)
            setTimer(CCA_UPDATE, simtime_t(CCA_TIME, SIMTIME_US));
        break;

    case CCA_END:
        _radio->_channel_clear = _radio->radioModule->isChannelClear() == CLEAR;
        cancelTimer(CCA_UPDATE);
        // fallthrough
    case TIMER_INTERRUPT:
        if(_handler)
            _handler(_unit);
        break;
    case WATCHDOG:
        throw cRuntimeError("Watchdog timer expired!");
        break;

    case TRACK_CLOCK_ERROR_INTERRUPT:
        register_clock_error();
        setTimer(TRACK_CLOCK_ERROR_INTERRUPT, TRACK_CLOCK_ERROR_PERIOD);
        break;
    }
}

void Timer::int_disable()
{
    cancelTimer(TIMER_INTERRUPT);
    cancelTimer(CCA_START);
    cancelTimer(CCA_UPDATE);
    cancelTimer(CCA_END);
    _handler = 0;
}

void Timer::schedule_cca(const Time_Stamp & when,
                         const Time_Stamp & time,
                         const Interrupt_Handler & callback)
{
    int_disable();
    _handler = callback;

    simtime_t t(when - read(), TIMER_EXPONENT);
    simtime_t t2(when - read() + time, TIMER_EXPONENT);

    setTimer(CCA_START, t);
    setTimer(CCA_END, t2);
}


bool Radio_Engine::tx_done()
{
    return true; // TODO
}

bool Radio_Engine::cca(const Microsecond & time)
{
    return _channel_clear && (_radio->radioModule->isChannelClear() == CLEAR);
}

void Radio_Engine::transmit_no_cca()
{
    _radio->toRadioLayer(createRadioCommand(SET_STATE, TX));
}

bool Radio_Engine::transmit()
{
    bool ret = cca();
    if(ret)
        _radio->toRadioLayer(createRadioCommand(SET_STATE, TX));
    return ret;
}

void Radio_Engine::listen()
{
    _radio->toRadioLayer(createRadioCommand(SET_STATE, RX));
}

void Radio_Engine::power(const Power_Mode & mode, bool delay)
{
    _power = mode;
    switch(mode) {
    case PM_FULL: // Able to receive and transmit
    case PM_LIGHT: // Able to sense channel and transmit
        _radio->toRadioLayer(createRadioCommand(SET_STATE, RX));
    break;
    case PM_SLEEP: // Receiver off
    case PM_OFF: // Radio unit shut down
        if(delay)
            _radio->sendDelayed(createRadioCommand(SET_STATE, SLEEP), simtime_t(11, SIMTIME_US), "toRadioModule");
        else
            _radio->toRadioLayer(createRadioCommand(SET_STATE, SLEEP));
    break;
    }
}

void Radio_Engine::channel(unsigned int c)
{
    assert((c > 10) && (c < 27));
    double freq = 2405 + 5 * (c - 11);
    _radio->toRadioLayer(createRadioCommand(SET_CARRIER_FREQ, freq));
}

void Radio_Engine::copy_to_nic(const void * frame, unsigned int size)
{
    assert(size < 127);

    TSTPMACPacket * p = new TSTPMACPacket("TSTP MAC Frame", MAC_LAYER_PACKET);
    for(unsigned int i = 0; i < size; i++)
        p->setData(i, reinterpret_cast<const char *>(frame)[i]);

    if(_enable_crc) {
        NIC_Common::CRC16 crc = NIC_Common::crc16(frame, size - sizeof(NIC_Common::CRC16));
        p->setData(size - 2, crc & 0x00ff);
        p->setData(size - 1, (crc & 0xff00) >> 8);
    }

    p->setByteLength(size);
    _radio->toRadioLayer(p);
}

unsigned int Radio_Engine::copy_from_nic(TSTPMACPacket * p, void * frame)
{
    unsigned int ret = p->getByteLength();
    for(unsigned int i = 0; i < ret; i++)
        reinterpret_cast<char *>(frame)[i] = p->getData(i);

    if(_enable_crc) {
        unsigned int lsb = p->getData(ret - 2);
        lsb &= 0x00ff;
        unsigned int msb = p->getData(ret - 1);
        msb &= 0x00ff;
        NIC_Common::CRC16 crc = (msb << 8) | lsb;
        if(NIC_Common::crc16(frame, ret - sizeof(NIC_Common::CRC16)) != crc) {
            _crc_errors++;
            trace() << "CRC error!";
            //throw cRuntimeError("CRC error!");
            return 0;
        }
    }

    return ret;
}

#include <tstp_common.h>
#include <tstp_mac.h>
#include <tstp_mac_nomf.h>
#include <tstp.h>

// TSTP binding
template<typename Radio>
void TSTP_MAC<Radio>::free(Buffer * b) { delete b; }
template void TSTP_MAC<Radio_Engine>::free(Buffer * b);
template<typename Radio>
void TSTP_MAC<Radio>::print(Buffer * b) { std::cout << *b->frame()->data<TSTP::Packet>() << std::endl; }
template void TSTP_MAC<Radio_Engine>::print(Buffer * b);
template<typename Radio>
bool TSTP_MAC<Radio>::equals(Buffer * b0, Buffer * b1) {
    if(b0->id != b1->id)
        return false;
    TSTP::Header * header = b0->frame()->data<TSTP::Header>();
    TSTP::Header * other_header = b1->frame()->data<TSTP::Header>();
    return
            (other_header->version() == header->version()) &&
            (other_header->type() == header->type()) &&
            (other_header->scale() == header->scale()) &&
            (other_header->time() == header->time()) &&
            (other_header->origin() == header->origin());
}
template bool TSTP_MAC<Radio_Engine>::equals(Buffer *, Buffer *);


template<typename Radio>
void TSTP_MAC_NOMF<Radio>::free(Buffer * b) { delete b; }
template void TSTP_MAC_NOMF<Radio_Engine>::free(Buffer * b);
template<typename Radio>
void TSTP_MAC_NOMF<Radio>::print(Buffer * b) { std::cout << *b->frame()->data<TSTP::Packet>() << std::endl; }
template void TSTP_MAC_NOMF<Radio_Engine>::print(Buffer * b);
template<typename Radio>
bool TSTP_MAC_NOMF<Radio>::equals(Buffer * b0, Buffer * b1) {
    if(b0->id != b1->id)
        return false;
    TSTP::Header * header = b0->frame()->data<TSTP::Header>();
    TSTP::Header * other_header = b1->frame()->data<TSTP::Header>();
    return
            (other_header->version() == header->version()) &&
            (other_header->type() == header->type()) &&
            (other_header->scale() == header->scale()) &&
            (other_header->time() == header->time()) &&
            (other_header->origin() == header->origin());
}
template bool TSTP_MAC_NOMF<Radio_Engine>::equals(Buffer *, Buffer *);
