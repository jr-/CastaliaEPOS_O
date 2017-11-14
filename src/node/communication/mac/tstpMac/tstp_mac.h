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

#ifndef TSTP_MAC_H_
#define TSTP_MAC_H_

#include <tstp_common.h>
#include <observer.h>

__USING_SYS

template<typename Radio>
class TSTP_MAC: public TSTP_Common, public Radio
{
    friend class TSTP_MAC_NED;
    static const bool check_hash_collisions = true;

protected:
    typedef IEEE802_15_4 Phy_Layer;

    enum State {
        UPDATE_TX_SCHEDULE = 0,
        SLEEP_S            = 1,
        RX_MF              = 2,
        SLEEP_DATA         = 3,
        RX_DATA            = 4,
        BACKOFF            = 5,
        CCA                = 6,
        TX_MF              = 7,
        TX_DATA            = 8,
    };

public:
    using TSTP_Common::Address;
    using TSTP_Common::Header;
    using TSTP_Common::Frame;
    typedef typename Radio::Timer Timer;
    typedef typename Radio::Timer::Time_Stamp Time_Stamp;

    //static const unsigned int MTU = Frame::MTU;

    const Statistics & statistics() { return _stats; }
    unsigned int period() { return PERIOD; }

private:
public: //TODO: public for debugging

    static const bool sniffer = false;
    static const bool state_machine_debugged = true;
    static const bool always_tx_data = true;

    bool radio_sleeps_after_tx;
    unsigned int MICROFRAME_SIZE;
    unsigned int DESIRED_DUTY_CYCLE;
    unsigned int INT_HANDLING_DELAY; // Time delay between scheduled tx_mf interrupt and actual Radio TX
    unsigned int SLEEP_TO_TX_DELAY;
    unsigned int SLEEP_TO_RX_DELAY;
    unsigned int RX_TO_TX_DELAY;
    static const unsigned int Tu = IEEE802_15_4::TURNAROUND_TIME;
    unsigned int Ti;
    unsigned int TIME_BETWEEN_MICROFRAMES;
    unsigned int Ts; // Time to send a single Microframe (including PHY headers)
    unsigned int MICROFRAME_TIME;
    unsigned int Tr;
    unsigned int RX_MF_TIMEOUT;
    unsigned int NMF;
    unsigned int N_MICROFRAMES;
    unsigned int CI;
    unsigned int PERIOD;
    unsigned int SLEEP_PERIOD;
    unsigned int DUTY_CYCLE;
    unsigned int DATA_LISTEN_MARGIN;
    unsigned int DATA_SKIP_TIME;
    unsigned int RX_DATA_TIMEOUT;
    static const unsigned int G = IEEE802_15_4::CCA_TX_GAP;
    unsigned int CCA_TIME;

    bool synchronous_optimization;
    unsigned int MAXIMUM_DRIFT;
    unsigned int Mmf;

    bool random_backoff;
    unsigned int OFFSET_GENERAL_LOWER_BOUND;
    unsigned int OFFSET_GENERAL_UPPER_BOUND;
    unsigned int OFFSET_LOWER_BOUND;
    unsigned int OFFSET_UPPER_BOUND;

    typedef typename Radio::Timer Watchdog;

protected:
    TSTP_MAC(unsigned int unit = 0)
        : Radio(unit),
            _unit(unit),
            _mfs_sent(0),
            _data_sent(0),
            _data_relayed(0),
            _hash_collisions(0),
            _silence(0),
            _max_times_txed(0)
    {
        Radio::trace() << "TSTP_MAC(u=" << unit << ")" << endl;
        _instances++;
        _instance[unit] = this;
        _global_keep_alives_sent = 0;
    }

    ~TSTP_MAC() {
        for(Buffer::Element * el = _tx_schedule.head(); el; el = _tx_schedule.head()) {
            Buffer * b = el->object();
            _tx_schedule.remove(el);
            if(b)
                free(b);
        }
        delete _offset_vector;
        delete _pickup;
        delete _ids_txed;
        _instance[_unit] = 0;
        _instances--;
    }

    // Called after the Radio's constructor
    void constructor_epilogue() {
        Radio::trace() << "TSTP_MAC::constructor_epilogue()";
        Radio::trace() << "_unit = " << _unit;
        _offset_vector = new cOutVector("Offset");
        _pickup = new cOutVector("MAC Pickup Time");
        _ids_txed = new cOutVector("Frame IDs transmitted");
        if(sniffer)
            sniff();
        else {
            Watchdog::enable();
            update_tx_schedule();
        }
    }

    void sniff() {
        Radio::power(Power_Mode::PM_FULL);
        Radio::listen();
    }

    // Filter and assemble RX Buffer Metainformation
    bool pre_notify(Buffer * buf) {
//        if(sniffer) { // TODO
//            static unsigned int last_id = 0;
//            static unsigned int last_hint = 0;
//            if(buf->size() == MICROFRAME_SIZE) {
//                Microframe * mf = buf->frame()->data<Microframe>();
//                if((mf->id() != last_id) || (mf->hint() != last_hint)) {
//                    last_id = mf->id();
//                    last_hint = mf->hint();
//
//                    buf->id = last_id;
//                    buf->downlink = mf->all_listen();
//                    buf->is_new = false;
//                    buf->is_microframe = true;
//                    buf->microframe_count = mf->count();
//                    buf->offset = 1000000llu * OFFSET_GENERAL_UPPER_BOUND;
//                    buf->duplicate = false;
//
//                    // Forge a TSTP identifier to make the radio notify listeners
//                    //mf->all_listen(false);
//                    //mf->count(TSTP_Common::V0 >> 1);
//                    mf->all_listen(TSTP_Common::V0 & 0x01);
//                    mf->id(TSTP_Common::V0 >> 1);
//
//                    return true;
//                }
//                return false;
//            } else {
//                last_id = 0;
//                last_hint = 0;
//                buf->is_microframe = false;
//                return true;
//            }
//            return false;
//        }


        if(_in_rx_mf) { // State: RX MF (part 2/3)
            Radio::trace() << "State: RX MF (part 2/3)";
            if(buf->size() == MICROFRAME_SIZE) {

                assert(buf->frame()->data<Microframe>()->count() < N_MICROFRAMES);

                Timer::int_disable();

                Radio::power(Power_Mode::PM_SLEEP);

                _in_rx_mf = false;

                Microframe * mf = buf->frame()->data<Microframe>();
                Frame_ID id = mf->id();

//                if(!always_tx_data) { // TODO
//                    assert(false);
//                    // This is an explicit ACK. No data frame will follow it.
//                    if((!mf->all_listen()) && (mf->hint() == 0)) {
//                        // Clear scheduled messages with same ID
//                        Buffer::Element * next;
//                        for(Buffer::Element * el = _tx_schedule.head(); el; el = next) {
//                            next = el->next();
//                            Buffer * queued_buf = el->object();
//                            if(queued_buf->id == id) {
//                                if(!queued_buf->destined_to_me) {
//                                    Radio::trace()
//                                        << "TSTP_MAC::pre_notify: ACK received: "
//                                        << *mf << " at "
//                                        << Radio::Timer::count2us(
//                                            buf->sfd_time_stamp) << endl;
//                                    _tx_schedule.remove(el);
//                                    delete queued_buf;
//                                }
//                            }
//                        }
//                        Time_Stamp data_time = (buf->sfd_time_stamp
//                            + Timer::us2count(TIME_BETWEEN_MICROFRAMES)
//                            + mf->count()
//                                * Timer::us2count(
//                                    TIME_BETWEEN_MICROFRAMES + MICROFRAME_TIME));
//                        Watchdog::kick();
//                        Radio::trace() << "Transition: [Irrelevant MF] (1)";
//                        Timer::interrupt(
//                            data_time + Timer::us2count(DATA_SKIP_TIME),
//                            update_tx_schedule);
//                        return false;
//                    }
//                }

                // Initialize Buffer Metainformation
                buf->id = id;
                buf->downlink = mf->all_listen();
                buf->is_new = false;
                buf->is_microframe = true;
                buf->relevant = sniffer || mf->all_listen();
                buf->trusted = false;
                buf->hint = mf->hint();
                buf->microframe_count = mf->count();
                buf->offset = 1000000llu * OFFSET_GENERAL_UPPER_BOUND;
                buf->progress_bits = 0;
                buf->times_txed = 0;

                // Forge a TSTP identifier to make the radio notify listeners
                mf->all_listen(false);
                mf->count(TSTP_Common::V0 >> 1);

                return true;
            }
            return false;
        } else if(_in_rx_data) { // State: RX Data (part 2/3)
            Radio::trace() << "State: RX Data (part 2/3)";

            buf->is_new = false;

            if((buf->size() == MICROFRAME_SIZE) || (id(buf) != _receiving_data_id)) {
                return false;
            }

            Radio::power(Power_Mode::PM_SLEEP);

            // Initialize Buffer Metainformation
            buf->id = _receiving_data_id;
            buf->hint = _receiving_data_hint;
            buf->is_microframe = false;
            buf->trusted = false;
            buf->random_backoff_exponent = 0;
            buf->microframe_count = 0;
            buf->offset = 1000000llu * OFFSET_GENERAL_UPPER_BOUND;
            buf->progress_bits = 0;
            buf->times_txed = 0;

            Radio::trace() << "TSTP_MAC::pre_notify: Data frame of size "
                << buf->size() << " received: " << buf->frame() << " at "
                << Radio::Timer::count2us(buf->sfd_time_stamp) << endl;

            return true;
        } else {
            Radio::trace() << "MAC not in RX state";
            return false;
        }
    }

    bool post_notify(Buffer * buf) {
        if(buf->is_microframe && !sniffer) { // State: RX MF (part 3/3)
            Radio::trace() << "State: RX MF (part 3/3)";
            Time_Stamp data_time = buf->sfd_time_stamp
                + Timer::us2count(
                    MICROFRAME_SIZE * 1000000 / Phy_Layer::BYTE_RATE)
                + Timer::us2count(TIME_BETWEEN_MICROFRAMES)
                + buf->microframe_count
                    * Timer::us2count(
                        TIME_BETWEEN_MICROFRAMES + MICROFRAME_TIME);

            Watchdog::kick();

            unsigned char received_bitmap = 1; // In multisink, this comes from the microframe
            // Clear scheduled messages with same ID
            Buffer::Element * next;
            for(Buffer::Element * el = _tx_schedule.head(); el; el = next) {
                next = el->next();
                Buffer * queued_buf = el->object();
                if(queued_buf->id == buf->id) {
                    if(!queued_buf->destined_to_me) {
                        for(unsigned int bit_position = 0; bit_position < sizeof(unsigned char); bit_position++) {
                            /* For each bit, perform the following operation:
                                 1 @ 0 @ 0 = 0
                                 1 @ 0 @ 1 = 0
                                 1 @ 1 @ 0 = 1
                                 1 @ 1 @ 1 = 1
                                 0 @ X @ Y = Y
                               with queued_buf->progress_bits = received_bitmap @ buf->progress_bits @ queued_buf->progress_bits
                            */
                            if(received_bitmap & (1 << bit_position)) {
                                unsigned char bit =
                                    (received_bitmap & (1 << bit_position))
                                        & (buf->progress_bits
                                            & (1 << bit_position));
                                queued_buf->progress_bits &=
                                    ~(1 << bit_position);
                                queued_buf->progress_bits &= bit;
                            }
                        }
                        if(queued_buf->progress_bits == 0) {
                            Radio::trace()
                                << "TSTP_MAC::pre_notify: ACK received, ID="
                                << queued_buf->id << " at "
                                << Timer::count2us(buf->sfd_time_stamp) << endl;
                            _tx_schedule.remove(el);
                            delete queued_buf;
                        }
                    }
                }
            }

            if(buf->relevant) { // Transition: [Relevant MF]
                _receiving_data_id = buf->id;
                _receiving_data_hint = buf->hint;
                // State: Sleep until Data
                Radio::trace() << "State: Sleep until Data";
                if(data_time > buf->sfd_time_stamp + 2 * Timer::us2count(DATA_LISTEN_MARGIN))
                    data_time -= Timer::us2count(DATA_LISTEN_MARGIN);
                else
                    data_time -= Timer::us2count(10);
                Timer::interrupt(data_time - Timer::us2count(SLEEP_TO_RX_DELAY), rx_data);
            } else { // Transition: [Irrelevant MF]
                Radio::trace() << "Transition: [Irrelevant MF] (2)";
                Timer::interrupt(data_time + Timer::us2count(DATA_SKIP_TIME), update_tx_schedule);
            }
        }

        free(buf);
        return true;
    }

public:
    Frame_ID id(Buffer * buf) {
        Frame_ID ret = 0;

        Header * header = buf->frame()->data<Header>();
        auto conf = header->confidence();
        auto tr = header->time_request();
        header->time_request(false); // Mask out time_request field for hashing
        header->confidence(0); // Mask out confidence field for hashing

        // Hash over the fields config,Origin(t,x,y,z) of the TSTP header
        unsigned int hash = murmur_hash(buf->frame()->data<const char>(), 2 + sizeof(Time) + sizeof(Coordinates));
        for(unsigned int i = 0; i < sizeof(unsigned int) / sizeof(Frame_ID); i++)
            ret ^= reinterpret_cast<const Frame_ID *>(&hash)[i];
        ret = ret & Microframe::ID_MAX;

        // Restore masked out fields
        header->time_request(tr);
        header->confidence(conf);

        if(check_hash_collisions) {
            for(unsigned int i = 0; i < _instances; i++) {
                if(i == _unit)
                    continue;
                TSTP_MAC * mac = _instance[i];
                for(Buffer::Element * el = mac->_tx_schedule.head(); el; el = el->next()) {
                    Buffer * other_buffer = el->object();
                    Header * other_header = other_buffer->frame()->data<Header>();

                    Frame_ID ret2 = 0;
                    auto conf = other_header->confidence();
                    auto tr = other_header->time_request();
                    other_header->time_request(false); // Mask out time_request field for hashing
                    other_header->confidence(0); // Mask out confidence field for hashing

                    // Hash over the fields config,Origin(t,x,y,z) of the TSTP header
                    unsigned int hash = murmur_hash(other_buffer->frame()->data<const char>(), 2 + sizeof(Time) + sizeof(Coordinates));
                    for(unsigned int i = 0; i < sizeof(unsigned int) / sizeof(Frame_ID); i++)
                        ret2 ^= reinterpret_cast<const Frame_ID *>(&hash)[i];
                    ret2 = ret2 & Microframe::ID_MAX;

                    // Restore masked out fields
                    other_header->time_request(tr);
                    other_header->confidence(conf);

                    if(ret2 != other_buffer->id) {
                        std::cout << "Swapped buffer ID at node " << i << "!" << std::endl;
                        std::cout << "Buffer = ";
                        //print(other_buffer);
                        std::cout << "ID = " << other_buffer->id << ", should be " << ret2 << "!" << std::endl;
                        //throw cRuntimeError("Swapped buffer ID!");
                    }

                    if(other_buffer->id == ret) {
                        if( (other_header->version() != header->version()) ||
                            (other_header->type() != header->type()) ||
                            (other_header->scale() != header->scale()) ||
                            (other_header->time() != header->time()) ||
                            (other_header->origin() != header->origin()) )
                        {
                            _hash_collisions++;
                            Radio::trace() << "Hash collision detected!";
                            Radio::trace() << other_buffer->frame()->data<Header>()->time() << " " << other_buffer->frame()->data<Header>()->origin() << " " << other_buffer->id << " " << i;
                            Radio::trace() << buf->frame()->data<Header>()->time() << " " << buf->frame()->data<Header>()->origin() << " " << ret << " " << Radio::self;
                            std::cout << "Hash collision detected!" << std::endl;
                            print(buf);
                            print(other_buffer);
                            std::cout << other_buffer->frame()->data<Header>()->time() << " " << other_buffer->frame()->data<Header>()->origin() << " " << other_buffer->id << " " << i << std::endl;
                            std::cout << buf->frame()->data<Header>()->time() << " " << buf->frame()->data<Header>()->origin() << " " << ret << " " << _unit << std::endl;
                            //throw cRuntimeError("Hash collision detected!");
                        }
                    }
                }
            }
        }

        return ret;
    }

    // Assemble TX Buffer Metainformation
    void marshal(Buffer * buf,
                 const Address & src,
                 const Address & dst,
                 const Type & type)
    {
        buf->is_microframe = false;
        buf->trusted = false;
        buf->is_new = true;
        buf->random_backoff_exponent = 0;
        buf->microframe_count = 0;
        buf->offset = 1000000llu * OFFSET_GENERAL_UPPER_BOUND;
        buf->times_txed = 0;
    }

    unsigned int unmarshal(Buffer * buf,
                           Address * src,
                           Address * dst,
                           Type * type,
                           void * data,
                           unsigned int size)
    {
        *src = Address::BROADCAST;
        *dst = Address::BROADCAST;
        *type = buf->frame()->data<Header>()->version();
        memcpy(data, buf->frame()->data<Frame>(), (buf->size() < size ? buf->size() : size));
        return buf->size();
    }

    int send(Buffer * buf) {
        bool keep_alive = (buf->frame()->data<Header>()->type() == CONTROL) && (buf->frame()->data<Control>()->subtype() == KEEP_ALIVE);
        if(keep_alive && !_tx_schedule.empty()) {
            delete buf;
            return 0;
        }

        if(buf->is_new)
            buf->id = id(buf);

        if(sniffer) {
            delete buf;
            return 0;
        } else {

            if(buf->destined_to_me)
                buf->offset = Timer::us2count(OFFSET_LOWER_BOUND);
            else {
                // Components calculate the offset in microseconds according to their own metrics.
                // We finish the calculation here to keep SLEEP_PERIOD, G, and Timestamps
                // encapsulated by the MAC, and MAC::marshal() happens before the other components' marshal methods
                //buf->offset = Timer::us2count(((buf->offset * SLEEP_PERIOD) / (G*RADIO_RANGE)) * G);
                buf->offset = Timer::us2count(buf->offset / 1000000llu);

                if(buf->offset < Timer::us2count(OFFSET_GENERAL_LOWER_BOUND))
                    buf->offset = Timer::us2count(OFFSET_GENERAL_LOWER_BOUND);
                else if(buf->offset > Timer::us2count(OFFSET_GENERAL_UPPER_BOUND))
                    buf->offset = Timer::us2count(OFFSET_GENERAL_UPPER_BOUND);
            }

            // Check if we have a message with the same ID. If so, replace it
            Buffer::Element * next;
            for(Buffer::Element * el = _tx_schedule.head(); el; el = next) {
                next = el->next();
                Buffer * queued_buf = el->object();
                if(equals(queued_buf, buf)) {
                    if(_tx_pending && (_tx_pending == queued_buf))
                        _tx_pending = buf;
                    _tx_schedule.remove(queued_buf->link());
                    buf->progress_bits |= queued_buf->progress_bits;
                    delete queued_buf;
                }
            }
            _tx_schedule.insert(buf->link());

            return buf->size();
        }
    }

private:

    // State Machine

    static void update_tx_schedule(const Interrupt_Id & id) { _instance[id]->update_tx_schedule(); }
    void update_tx_schedule()
    {
        Timer::int_disable();
        Watchdog::kick();
        Radio::trace() << "State: Update TX Schedule";

        // State: Update TX Schedule
        Radio::power(Power_Mode::PM_SLEEP);
        _in_rx_data = false;
        _in_rx_mf = false;

        _tx_pending = 0;

        Time_Stamp now_ts = Timer::read();
        Microsecond now_us = Timer::count2us(now_ts);

        if(_silence)
            _silence--;
        else {
            // Fetch next message and remove expired ones
            // TODO: Turn _tx_schedule into an ordered list
            for(Buffer::Element * el = _tx_schedule.head(); el;) {
                Buffer::Element * next = el->next();
                Buffer * b = el->object();
                if(drop_expired && (b->deadline <= now_us)) {
                    _tx_schedule.remove(el);
                    delete b;
                } else if(!_tx_pending) {
                    _tx_pending = b;
                } else if(((_tx_pending->frame()->data<Header>()->type() == CONTROL) && (_tx_pending->frame()->data<Control>()->subtype() == KEEP_ALIVE))
                    && !((b->frame()->data<Header>()->type() == CONTROL) && (b->frame()->data<Control>()->subtype() == KEEP_ALIVE))) {
                    _tx_schedule.remove(_tx_pending->link());
                    delete _tx_pending;
                    _tx_pending = b;
                } else if(_tx_pending->destined_to_me) {
                    if(b->destined_to_me) {
                        if(b->random_backoff_exponent < _tx_pending->random_backoff_exponent) {
                            _tx_pending = b;
                        } else if((b->random_backoff_exponent
                            == _tx_pending->random_backoff_exponent)
                            && (b->deadline < _tx_pending->deadline)) {
                            _tx_pending = b;
                        }
                    }
                } else {
                    if(b->random_backoff_exponent < _tx_pending->random_backoff_exponent) {
                        _tx_pending = b;
                    } else if((b->random_backoff_exponent
                    == _tx_pending->random_backoff_exponent)
                    && (b->deadline < _tx_pending->deadline)) {
                        _tx_pending = b;
                    }
                }
                el = next;
            }
        }

        if(_tx_pending) { // Transition: [TX pending]
            Radio::trace() << "Transition: [TX pending]";
            // State: Contend CCA (Contend part)
            Radio::power(Power_Mode::PM_LIGHT);

            Watchdog::kick();
            Radio::trace() << "State: Contend CCA (Contend part)";

            if(_tx_pending->is_new && _tx_pending->random_backoff_exponent == 0)
                _pickup->record(
                    (simTime().inUnit(-6)
                        - _tx_pending->data()->data<Header>()->time())
                        / 1000000.0);

            Time_Stamp offset = _tx_pending->offset;
            if(random_backoff) {
                // Increase ACK priority and decrease non-ACK priority by a random component,
                // based on number of transmission attempts.
                // This prevents permanent inteference by a given pair of nodes, and
                // makes unresponded messages have the lowest priorities
                _tx_pending->random_backoff_exponent++;
                unsigned int lim = Timer::us2count(G) * _tx_pending->random_backoff_exponent;
                if(!lim)
                    lim = Timer::us2count(OFFSET_UPPER_BOUND);
                offset += ((unsigned int) (Random::random()) % lim);
                if(offset < Timer::us2count(OFFSET_GENERAL_LOWER_BOUND)) {
                    offset = Timer::us2count(OFFSET_GENERAL_LOWER_BOUND);
                    _tx_pending->random_backoff_exponent--;
                } else if(offset > Timer::us2count(OFFSET_UPPER_BOUND)) {
                    offset = Timer::us2count(OFFSET_UPPER_BOUND);
                    _tx_pending->random_backoff_exponent--;
                }
            }

            if(synchronous_optimization) {
                if(offset <= Timer::us2count(SLEEP_PERIOD / 2) - MAXIMUM_DRIFT) {
                    double num1 = (Timer::us2count(SLEEP_PERIOD / 2) - offset);
                    double den1 = Timer::us2count(Ts + Ti);
                    double num2 = Mmf;
                    double den2 = 2;
                    double res = num1 / den1 + num2 / den2 + 0.999;
                    _F = res;
                } else {
                    double num1 = (Timer::us2count(SLEEP_PERIOD) - offset);
                    double den1 = Timer::us2count(Ts + Ti);
                    double num2 = Mmf;
                    double den2 = 2;
                    double res = num1 / den1 + num2 / den2 + 0.999;
                    _F = res;
                    if(_F < Mmf)
                        _F = Mmf;
                }
                Radio::trace() << "_F = " << _F;
            }

            new (&_mf) Microframe(
                (!_tx_pending->destined_to_me) && _tx_pending->downlink,
                _tx_pending->id, N_MICROFRAMES - 1, _tx_pending->hint);

            Radio::trace() << "offset = " << offset;
            _offset_vector->record(Radio::count2us(offset) / 1000000.0);

            if(synchronous_optimization) {
                if(offset > Timer::us2count(SLEEP_PERIOD / 2 + Tr)) {
                    Radio::schedule_cca(
                        now_ts + Timer::us2count(SLEEP_PERIOD / 2),
                        Timer::us2count(CCA_TIME), cca_first);
                    _next_cca = now_ts + offset;
                } else
                    Radio::schedule_cca(now_ts + offset, Timer::us2count(CCA_TIME), cca);
            }
            else
                Radio::schedule_cca(now_ts + offset, Timer::us2count(CCA_TIME), cca);
            //Timer::interrupt(now_ts + offset, cca);
        } else { // Transition: [No TX pending]
            // State: Sleep S
            Radio::trace() << "Transition [No TX pending]";
            Radio::trace() << "State: Sleep S";
            Watchdog::kick();
            if(synchronous_optimization)
                Timer::interrupt(
                    now_ts
                        + (Timer::us2count(SLEEP_PERIOD)
                            - (now_ts % Timer::us2count(SLEEP_PERIOD))),
                    rx_mf);
            else
                Timer::interrupt(now_ts + Timer::us2count(SLEEP_PERIOD), rx_mf);
        }
    }

    // State: Contend CCA (CCA part)
    static void cca(const Interrupt_Id & id) { _instance[id]->cca(); }
    void cca()
    {
        Radio::trace() << "State: Contend CCA (CCA part)";
        Watchdog::kick();
        assert(N_MICROFRAMES > 1);

        //Radio::listen();

        // Try to send the first Microframe
        if(Radio::cca()) {
            Radio::copy_to_nic(&_mf, MICROFRAME_SIZE);
            _mf_time = Timer::read() + Timer::us2count(RX_TO_TX_DELAY);
            if(Radio::transmit()) { // Transition: [Channel free]
                _mfs_sent++;
                _mf_time += Timer::us2count(TIME_BETWEEN_MICROFRAMES + MICROFRAME_TIME);
                _ids_txed->record(_mf.id());
                _mf.dec_count();
                Watchdog::kick();
                while(!Radio::tx_done());
                //Radio::copy_to_nic(&_mf, MICROFRAME_SIZE);
                //tx_mf();
                if(radio_sleeps_after_tx)
                    Timer::interrupt(_mf_time - Timer::us2count(SLEEP_TO_TX_DELAY), tx_mf);
                else
                    Timer::interrupt(_mf_time - Timer::us2count(RX_TO_TX_DELAY), tx_mf);
            } else { // Transition: [Channel busy]
                rx_mf();
            }
        } else { // Transition: [Channel busy]
            rx_mf();
        }
    }

    static void cca_first(const Interrupt_Id & id) { _instance[id]->cca_first(); }
    void cca_first() {
        Watchdog::kick();
        Radio::schedule_cca(_next_cca, Timer::us2count(CCA_TIME), cca);

        Radio::trace() << "State: Contend CCA (CCA part)";
        assert(N_MICROFRAMES > 1);

        //Radio::listen();

        if(Radio::cca())
            Radio::power(Power_Mode::PM_SLEEP);
        else { // Transition: [Channel busy]
            Radio::int_disable();
            rx_mf();
        }
    }

    // State: RX MF (part 1/3)
    static void rx_mf(const Interrupt_Id & id) { _instance[id]->rx_mf(); }
    void rx_mf() {
        Radio::trace() << "State: RX MF (part 1/3)";
        Watchdog::kick();
        _in_rx_data = false;
        _in_rx_mf = true;

        Radio::trace() << "Timer::read() = " << Timer::read();
        Radio::trace() << "Timer::us2count(RX_MF_TIMEOUT) = "
            << Timer::us2count(RX_MF_TIMEOUT);
        // If timeout is reached, Transition: [No MF]
        Timer::interrupt(Timer::read() + Timer::us2count(RX_MF_TIMEOUT), update_tx_schedule);

        Radio::power(Power_Mode::PM_FULL);
        Radio::listen();
    }

    // State: RX Data (part 1/3)
    static void rx_data(const Interrupt_Id & id) { _instance[id]->rx_data(); }
    void rx_data() {
        Radio::trace() << "State: RX Data (part 1/3)";
        Watchdog::kick();
        _in_rx_data = true;
        _in_rx_mf = false;

        // Set timeout
        Timer::interrupt(Timer::read() + Timer::us2count(RX_DATA_TIMEOUT), update_tx_schedule);

        Radio::power(Power_Mode::PM_FULL);
        Radio::listen();
    }

    // State: TX MFs
    static void tx_mf(const Interrupt_Id & id) { _instance[id]->tx_mf(); }
    void tx_mf() {
        Radio::trace() << "State: TX MFs";
        Radio::trace() << _mf.count();
        //while(true) {

        //while(Timer::read() < _mf_time);

        // The first Microframe is sent at cca()
        if(synchronous_optimization) {
            if(_F > 1) {
                Radio::copy_to_nic(&_mf, MICROFRAME_SIZE);
                Radio::transmit_no_cca();
                _F--;
                _mfs_sent++;
                Radio::trace() << "_F = " << _F;
            } else {
                if(!radio_sleeps_after_tx)
                    Radio::power(PM_SLEEP);
            }
        } else {
            Radio::copy_to_nic(&_mf, MICROFRAME_SIZE);
            Radio::transmit_no_cca();
            _mfs_sent++;
        }

        _mf_time += Timer::us2count(TIME_BETWEEN_MICROFRAMES + MICROFRAME_TIME);

        Watchdog::kick();
        //if(state_machine_debugged)
        //kout << TX_MF;

        if(_mf.dec_count() > 0) {
            //while(!Radio::tx_done());
            //Radio::copy_to_nic(&_mf, MICROFRAME_SIZE);
            //Radio::transmit_no_cca();
            //_mfs_sent++;
            if(radio_sleeps_after_tx)
                Timer::interrupt(_mf_time - Timer::us2count(SLEEP_TO_TX_DELAY), tx_mf);
            else
                Timer::interrupt(_mf_time - Timer::us2count(RX_TO_TX_DELAY), tx_mf);
        } else {
            // The precise time when this frame is actually sent by the physical layer
            _tx_pending->frame()->data<Header>()->last_hop_time(_mf_time);
            Radio::trace() << "Data frame should arrive at "
                << _tx_pending->frame()->data<Header>()->last_hop_time();
            //while(!Radio::tx_done());
            //Radio::copy_to_nic(_tx_pending->frame(), _tx_pending->size());
            if(synchronous_optimization)
                Radio::power(PM_FULL);

            if(radio_sleeps_after_tx)
                Timer::interrupt(_mf_time - Timer::us2count(SLEEP_TO_TX_DELAY), tx_data);
            else
                Timer::interrupt(_mf_time - Timer::us2count(RX_TO_TX_DELAY), tx_data);
            //while(Timer::read() < _mf_time);
            //tx_data();
            //return;
        }
        //}
    }

    static void tx_data(const Interrupt_Id & id) { _instance[id]->tx_data(); }
    void tx_data() {
        bool transmit, remove;

        remove = _tx_pending->destined_to_me;
        transmit = always_tx_data || !_tx_pending->destined_to_me;

        if(transmit) { // Transition: [Is not dest.]
            _tx_pending->times_txed++;
            if(_tx_pending->times_txed > _max_times_txed)
                _max_times_txed = _tx_pending->times_txed;
            if(!_tx_pending->destined_to_me) {
                unsigned int r = Random::random();
                _silence += (r % _tx_pending->times_txed) + 1;
            }
            last_send = simTime();
            if(first_send == 0)
                first_send = simTime();

            // State: TX Data
            Radio::trace() << "State: TX Data";
            Radio::copy_to_nic(_tx_pending->frame(), _tx_pending->size());
            Radio::trace() << "TXing Data frame of size " << _tx_pending->size();
            Radio::transmit_no_cca();
            _stats.tx_packets++;
            _stats.tx_bytes += _tx_pending->size();
            if(!_tx_pending->is_new)
                _stats.tx_relayed++;
            Watchdog::kick();
            //while(!Radio::tx_done());

            _mf_time = Timer::read();

            // Keep Alive messages are never ACK'ed or forwarded
            if((_tx_pending->frame()->data<Header>()->type() == CONTROL)
                && (_tx_pending->frame()->data<Control>()->subtype()
                    == KEEP_ALIVE)) {
                _global_keep_alives_sent++;
                remove = true;
            }
        }
        if(remove) { // Transition: [Is dest.]
            Watchdog::kick();
            _tx_schedule.remove(_tx_pending->link());
            delete _tx_pending;
            Timer::interrupt(_mf_time + Timer::us2count(SLEEP_PERIOD), rx_mf);
        } else {
            // State: Sleep S
            Radio::trace() << "State: Sleep S";
            if(!radio_sleeps_after_tx)
                Radio::power(Power_Mode::PM_SLEEP, true);
            if(synchronous_optimization)
                Timer::interrupt(
                    _mf_time
                    + (Timer::us2count(SLEEP_PERIOD)
            - (_mf_time % Timer::us2count(SLEEP_PERIOD))),
            rx_mf);
            else
                Timer::interrupt(_mf_time + Timer::us2count(SLEEP_PERIOD), rx_mf);
        }
    }

    void free(Buffer * b);
    void print(Buffer * b);
    bool equals(Buffer *, Buffer *);

    Microframe _mf;
    Time_Stamp _mf_time;
    Time_Stamp _next_cca;
    Frame_ID _receiving_data_id;
    Hint _receiving_data_hint;
    Buffer::List _tx_schedule;
    Buffer * _tx_pending;
    bool _in_rx_mf;
    bool _in_rx_data;

protected:
public:
    static TSTP_MAC<Radio> * _instance[2048];
    static unsigned int _instances;
    unsigned int _unit;

public:
    static unsigned int _global_keep_alives_sent;
    unsigned int _mfs_sent;
    unsigned int _data_sent;
    unsigned int _data_relayed;
    simtime_t first_send;
    simtime_t last_send;
    unsigned int _hash_collisions;

protected:
    unsigned int _F;
    cOutVector * _offset_vector;
    cOutVector * _pickup;
    cOutVector * _ids_txed;

private:
    Statistics _stats;
    unsigned int _silence;
    unsigned int _max_times_txed;
};

// The compiler makes sure that template static variables are only defined once

// Class attributes
template<typename Radio>
TSTP_MAC<Radio> * TSTP_MAC<Radio>::_instance[2048];

template<typename Radio>
unsigned int TSTP_MAC<Radio>::_instances;

template<typename Radio>
unsigned int TSTP_MAC<Radio>::_global_keep_alives_sent;

#include <castalia_nic.h>

class TSTP_MAC_NED: public IEEE802_15_4::NIC_Base<IEEE802_15_4, true>, public TSTP_MAC<Radio_Engine>
{
    typedef TSTP_MAC<Radio_Engine> MAC;
    typedef IEEE802_15_4::NIC_Base<IEEE802_15_4, true> NIC;

public:
    typedef MAC::Buffer Buffer;
    typedef NIC::Statistics Statistics;
    typedef NIC::Address Address;
    typedef NIC::Protocol Protocol;

    unsigned int period() { return MAC::period(); }
    const NIC::Statistics & statistics() { return MAC::statistics(); }
    void reset() { }

    // NIC Timer Interface
    Timer::Time_Stamp read() { return Timer::read(); }
    Timer::Time_Stamp frequency() { return Timer::frequency(); }
    double frequency_error() { return Timer::frequency_error(); }
    Timer::Time_Stamp sfd() { return Timer::read(); } // Not implemented
    void adjust(const Timer::Time_Stamp & t2, const Timer::Time_Stamp & t3) { return Timer::adjust(t2, t3); }
    Timer::Time_Stamp us2count(const Timer::Microsecond & us) { return Timer::us2count(us); }
    Timer::Microsecond count2us(const Timer::Time_Stamp & ts) { return Timer::count2us(ts); }

public:
    typedef IEEE802_15_4::Observer Observer;
    typedef IEEE802_15_4::Observed Observed;

    static unsigned int _units;

    TSTP_MAC_NED()
        : TSTP_MAC<Radio_Engine>(_units++) {
        Timer::_radio = this;
        trace() << "TSTPMAC::TSTPMAC()";
    }

    void startup();

    void finishSpecific();

    void fromNetworkLayer(cPacket *, int) { } // Unused, but required by Castalia

    int send(const Address & dst,
             const Protocol & prot,
             const void * data,
             unsigned int size) {
        Buffer * b = alloc(0, dst, prot, 0, 0, size);
        memcpy(b->frame()->data<void>(), data, size);
        return send(b);
    }

    Buffer * alloc(void * nic,
                   const Address & dst,
                   const Protocol & prot,
                   unsigned int once,
                   unsigned int always,
                   unsigned int payload) {
        // Initialize the buffer
        Buffer * buf = new Buffer(0);
        new (buf) Buffer(nic, once + always + payload + sizeof(MAC::Header));
        MAC::marshal(buf, address(), dst, prot);

        return buf;
    }

    void free(Buffer * buf) {
        buf->size(0);
        buf->unlock();
    }

    int send(Buffer * buf) { return MAC::send(buf); }

    int receive(Address * src, Protocol * prot, void * data, unsigned int size) { return 0; } // Unused
    const Address & address() { return _address; }
    void address(const Address & address) {}

    void fromRadioLayer(cPacket * pkt, double rssi, double lqi) {
        trace() << "TSTPMAC::fromRadioLayer";
        TSTPMACPacket *macPkt = dynamic_cast<TSTPMACPacket*>(pkt);
        if(macPkt == NULL) {
            trace() << "macPkt == NULL";
            return;
        }

        if(_power != PM_FULL) {
            trace() << "_power != PM_FULL";
            cancelAndDelete(pkt);
            return;
        }

        Buffer * buf = new Buffer(0);
        buf->owner(this);
        buf->size(Radio_Engine::copy_from_nic(macPkt, buf->frame()));
        if(!buf->size()) {
            trace() << "!buf->size()";
            delete buf;
            cancelAndDelete(pkt);
            return;
        }

        buf->rssi = rssi; // TODO

        buf->sfd_time_stamp = Timer::read()
            - Timer::us2count(
                PROCESSING_DELAY * 1000000ull
                    + buf->size() * 1000000ull / Phy_Layer::BYTE_RATE);

        if(MAC::pre_notify(buf)) {
            bool notified = notify(
                reinterpret_cast<IEEE802_15_4::Header *>(buf->frame())->type(),
                buf);
            if(!MAC::post_notify(buf) && !notified)
                delete buf; // No one was waiting for this frame
        } else {
            delete buf;
        }

        cancelAndDelete(pkt);
    }

    void mac2net(cPacket * pkt) {
        toNetworkLayer(pkt);
    }

private:
    Address _address;
};

#endif /* TSTPMAC_H_ */
