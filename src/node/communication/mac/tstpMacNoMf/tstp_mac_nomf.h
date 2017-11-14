#ifndef _TSTP_MAC_NOMF_H_
#define _TSTP_MAC_NOMF_H_

#include <omnetpp.h>
#include <TSTPMACPacket_m.h>
#include <ieee802_15_4.h>
#include <tstp_common.h>

__USING_SYS

template<typename Radio>
class TSTP_MAC_NOMF: public TSTP_Common, public Radio
{
public:
    typedef typename Radio::Timer Timer;
    typedef typename Radio::Timer::Time_Stamp Time_Stamp;

    const Statistics & statistics() { return _stats; }
    unsigned int period() { return PERIOD; }

protected:
    static const bool check_hash_collisions = true;
    static const bool random_backoff = true;

    unsigned int PERIOD;
    static const unsigned int OFFSET_LOWER_BOUND = 2 * CCA_TX_GAP;
    static const unsigned int OFFSET_GENERAL_LOWER_BOUND = 2 * OFFSET_LOWER_BOUND;
    unsigned int OFFSET_GENERAL_UPPER_BOUND;
    unsigned int OFFSET_UPPER_BOUND;

protected:
    static void update_tx_schedule(const Interrupt_Id & id) { _instance[id]->update_tx_schedule(); }
    void update_tx_schedule() {
        Radio::trace() << "State: Update TX Schedule";

        // State: Update TX Schedule
        //Radio::power(Power_Mode::PM_SLEEP);

        Buffer * tx_already_pending = _tx_pending;
        _tx_pending = 0;

        Time_Stamp now_ts = Timer::read();
        Microsecond now_us = Timer::count2us(now_ts);

        // Fetch next message and remove expired ones
        // TODO: Turn _tx_schedule into an ordered list
        for(Buffer::Element * el = _tx_schedule.head(); el; ) {
            Buffer::Element * next = el->next();
            Buffer * b = el->object();
            if(drop_expired && (b->deadline <= now_us)) {
                _tx_schedule.remove(el);
                delete b;
                _buffer_inout_n--;
                _buffer_inout->record(_buffer_inout_n);
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
                    } else if((b->random_backoff_exponent == _tx_pending->random_backoff_exponent) && (b->deadline < _tx_pending->deadline)) {
                        _tx_pending = b;
                    }
                }
            } else {
                if(b->random_backoff_exponent < _tx_pending->random_backoff_exponent) {
                    _tx_pending = b;
                } else if((b->random_backoff_exponent == _tx_pending->random_backoff_exponent) && (b->deadline < _tx_pending->deadline)) {
                    _tx_pending = b;
                }
            }
            el = next;
        }

        if(_tx_pending) {
            if(_tx_pending != tx_already_pending) { // Transition: [TX pending]
                Radio::trace() << "Transition: [TX pending]";

                Time_Stamp offset;
                offset = _tx_pending->offset;
                if(random_backoff) {
                    // Increase ACK priority and decrease non-ACK priority by a random component,
                    // based on number of transmission attempts.
                    // This prevents permanent inteference by a given pair of nodes, and
                    // makes unresponded messages have the lowest priorities
                    unsigned int lim = Timer::us2count(CCA_TX_GAP) << _tx_pending->random_backoff_exponent;
                    if(!lim)
                        lim = Timer::us2count(OFFSET_UPPER_BOUND);
                    if(_tx_pending->destined_to_me) {
                        offset -= ((unsigned int)(Random::random()) % lim);// * Timer::us2count(G);
                        if((offset < Timer::us2count(OFFSET_LOWER_BOUND)) || (offset > Timer::us2count(OFFSET_UPPER_BOUND))) {
                            offset = Timer::us2count(OFFSET_LOWER_BOUND);
                        }
                    } else {
                        offset += ((unsigned int)(Random::random()) % lim);// * Timer::us2count(G);
                        if(offset < Timer::us2count(OFFSET_GENERAL_LOWER_BOUND)) {
                            offset = Timer::us2count(OFFSET_GENERAL_LOWER_BOUND);
                        } else if(offset > Timer::us2count(OFFSET_UPPER_BOUND)) {
                            offset = Timer::us2count(OFFSET_UPPER_BOUND);
                        }
                    }
                }

                Radio::trace() << "offset = " << offset;

                Radio::schedule_cca(now_ts + offset, Timer::us2count(Radio::CCA_TIME + 1), cca_tx);
            }
        } else
            Timer::int_disable();

        rx();
    }

    void rx() {
        //Radio::power(Power_Mode::PM_FULL);
        //Radio::listen();
    }

    static void cca_tx(const Interrupt_Id & id) { _instance[id]->cca_tx(); }
    void cca_tx() {
        Radio::trace() << "TSTP_MAC_NOMF::cca_tx()";

        if(Radio::cca()) {
            // The precise time when this frame is actually sent by the physical layer
            _tx_pending->frame()->data<Header>()->last_hop_time(Timer::read() + Timer::us2count(Radio::RX_TO_TX_DELAY));
            Radio::copy_to_nic(_tx_pending->frame(), _tx_pending->size());
            Radio::transmit_no_cca();
            _ids_txed->record(_tx_pending->id);
            _stats.tx_packets++;
            _stats.tx_bytes += _tx_pending->size();
            if(!_tx_pending->is_new)
                _stats.tx_relayed++;

            Timer::interrupt(Timer::read() + Timer::us2count((PHY_HEADER_SIZE + _tx_pending->size()) * 1000000 / BYTE_RATE + Radio::TX_TO_RX_DELAY + PERIOD), update_tx_schedule);
            bool is_keep_alive = (_tx_pending->frame()->data<Header>()->type() == CONTROL) && (_tx_pending->frame()->data<Control>()->subtype() == KEEP_ALIVE);
            if(is_keep_alive)
                _global_keep_alives_sent++;
            if(_tx_pending->destined_to_me || is_keep_alive) {
                _tx_schedule.remove(_tx_pending->link());
                delete _tx_pending;
                _buffer_inout_n--;
                _buffer_inout->record(_buffer_inout_n);
            }
            else
                _tx_pending->random_backoff_exponent++;
            _tx_pending = 0;
        } else {
            _tx_pending->random_backoff_exponent++;
            _tx_pending = 0;
            update_tx_schedule();
        }
    }

    static unsigned int _seq;
    bool _idle;
    unsigned int _backoff;
    unsigned int _retry;

public:
    // Statistics
    unsigned int _total_retries;
    unsigned int _acks_sent;
    unsigned int _acks_received;
    unsigned int _data_received;
    unsigned int _tx_drop;


protected:

    typedef IEEE802_15_4 Phy_Layer;

    TSTP_MAC_NOMF(unsigned int unit) : Radio(unit), _unit(unit), _buffer_inout(0),  _buffer_inout_n(0), _ids_txed(0), _hash_collisions(0) {
        _instances++;
        _instance[unit] = this;
        _global_keep_alives_sent = 0;
    }

    ~TSTP_MAC_NOMF() {
        Radio::trace() << "~TSTP_MAC_NOMF()" << endl;
        for(Buffer::Element * el = _tx_schedule.head(); el; el = _tx_schedule.head()) {
            Buffer * b = el->object();
            _tx_schedule.remove(el);
            if(b)
                free(b);
            _buffer_inout_n--;
            _buffer_inout->record(_buffer_inout_n);
        }
        if(_buffer_inout)
            delete _buffer_inout;
        if(_ids_txed)
            delete _ids_txed;
        _instance[_unit] = 0;
        _instances--;
    }

    // Called after the Radio's constructor
    void constructor_epilogue() {
        Radio::trace() << "TSTP_MAC_NOMF::constructor_epilogue()";
        _buffer_inout = new cOutVector("Buffers queued");
        _ids_txed = new cOutVector("Frame IDs transmitted");
        _idle = true;
        _total_retries = 0;
        _acks_sent = 0;
        _acks_received = 0;
        _data_received = 0;
        _tx_drop = 0;
        _tx_pending = 0;
        update_tx_schedule();
    }

    // Filter and assemble RX Buffer Metainformation
    bool pre_notify(Buffer * buf) {
        Radio::trace() << "TSTP_MAC_NOMF::pre_notify(b=" << buf << ")";
        _data_received++;

        // Initialize Buffer Metainformation
        buf->is_new = false;
        buf->is_microframe = false;
        buf->trusted = false;
        buf->random_backoff_exponent = 0;
        buf->progress_bits = 0;
        buf->relevant = false;
        buf->offset = 1000000llu * OFFSET_GENERAL_UPPER_BOUND;
        buf->id = id(buf);

        // Clear scheduled messages with same ID
        Buffer::Element * next;
        for(Buffer::Element * el = _tx_schedule.head(); el; el = next) {
            next = el->next();
            assert(!_tx_schedule.empty());
            Buffer * queued_buf = el->object();
            if(queued_buf->id == buf->id) {
                if(!queued_buf->destined_to_me) {
                    Radio::trace() << "TSTP_MAC::pre_notify: ACK received";
                    if(queued_buf == _tx_pending) {
                        _tx_pending = 0;
                        Timer::int_disable();
                    }
                    _tx_schedule.remove(el);
                    delete queued_buf;
                    _buffer_inout_n--;
                    _buffer_inout->record(_buffer_inout_n);
                }
            }
        }

        return true;
    }

    bool post_notify(Buffer * buf) {
        assert(!_tx_schedule.search(buf));
        free(buf);
        update_tx_schedule();
        return true;
    }

    void free(Buffer * b);

public:
    // Assemble TX Buffer Metainformation and MAC Header
    void marshal(Buffer * buf, const Address & src, const Address & dst, const Type & type) {
        Radio::trace() << "TSTP_MAC_NOMF::marshal(b=" << buf << ")";
        buf->id = 0;
        buf->is_microframe = false;
        buf->trusted = false;
        buf->is_new = true;
        buf->random_backoff_exponent = 0;
        buf->microframe_count = 0;
        buf->progress_bits = 0;
        buf->offset = 1000000llu * OFFSET_GENERAL_UPPER_BOUND;
    }

    unsigned int unmarshal(Buffer * buf, Address * src, Address * dst, Type * type, void * data, unsigned int size) {
        Radio::trace() << "TSTP_MAC_NOMF::unmarshal(b=" << buf << ")";
        return 0;
    }

    int send(Buffer * buf) {
        bool keep_alive = (buf->frame()->data<Header>()->type() == CONTROL) && (buf->frame()->data<Control>()->subtype() == KEEP_ALIVE);
        if(keep_alive && !_tx_schedule.empty()) {
            delete buf;
            return 0;
        }

        Radio::trace() << "TSTP_MAC_NOMF::send(b=" << buf << ")";
        Enter_Method("send(Buffer * buf)");
        if(buf->is_new)
            buf->id = id(buf);

        // Components calculate the offset in microseconds according to their own metrics. Transform to Time_Stamp.
        buf->offset = Timer::us2count(buf->offset / 1000000llu);

        if(buf->offset < Timer::us2count(OFFSET_GENERAL_LOWER_BOUND))
            buf->offset = Timer::us2count(OFFSET_GENERAL_LOWER_BOUND);
        else if(buf->offset > Timer::us2count(OFFSET_GENERAL_UPPER_BOUND))
            buf->offset = Timer::us2count(OFFSET_GENERAL_UPPER_BOUND);

        // Check if we have a message with the same ID. If so, replace it
        Buffer::Element * next;
        for(Buffer::Element * el = _tx_schedule.head(); el; el = next) {
            next = el->next();
            Buffer * queued_buf = el->object();
            if(equals(queued_buf, buf)) {
                if(_tx_pending && (_tx_pending == queued_buf))
                    _tx_pending = buf;
                _tx_schedule.remove(queued_buf->link());
                delete queued_buf;
                _buffer_inout_n--;
                _buffer_inout->record(_buffer_inout_n);
            }
        }

        _tx_schedule.insert(buf->link());
        _buffer_inout_n++;
        _buffer_inout->record(_buffer_inout_n);

        update_tx_schedule();

        return buf->size();
    }

    Frame_ID id(Buffer * buf) {
        Radio::trace() << "TSTP_MAC_NOMF::id(b = " << buf << ")";

        /*
        Frame_ID ret;
        if(buf->frame()->data<Header>()->type() == TSTP_Common::RESPONSE)
            ret = *buf->frame()->data<Response>()->data<Frame_ID>();
        else if((buf->frame()->data<Header>()->type() == TSTP_Common::CONTROL) && (buf->frame()->data<Control>()->subtype() == TSTP_Common::KEEP_ALIVE))
            ret = buf->frame()->data<Keep_Alive>()->_crc;
        else
            throw cRuntimeError("Wrong message type!");
        */

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
        //ret = ret & Microframe::ID_MAX;

        // Restore masked out fields
        header->time_request(tr);
        header->confidence(conf);

        if(check_hash_collisions) {
            for(unsigned int i = 0; i < _instances; i++) {
                if(i == _unit)
                    continue;
                TSTP_MAC_NOMF * mac = _instance[i];
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
                    //ret2 = ret2 & Microframe::ID_MAX;

                    // Restore masked out fields
                    other_header->time_request(tr);
                    other_header->confidence(conf);

                    if(ret2 != other_buffer->id) {
                        std::cout << "Swapped buffer ID at node " << i << "!" << std::endl;
                        std::cout << "Buffer = ";
                        print(other_buffer);
                        std::cout << "ID = " << other_buffer->id << ", should be " << ret2 << "!" << std::endl;
                        throw cRuntimeError("Swapped buffer ID!");
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
//                            std::cout << "Hash collision detected!" << std::endl;
//                            print(buf);
//                            print(other_buffer);
//                            std::cout << other_buffer->frame()->data<Header>()->time() << " " << other_buffer->frame()->data<Header>()->origin() << " " << other_buffer->id << " " << i << std::endl;
//                            std::cout << buf->frame()->data<Header>()->time() << " " << buf->frame()->data<Header>()->origin() << " " << ret << " " << _unit << std::endl;
//                            throw cRuntimeError("Hash collision detected!");
                        }
                    }
                }
            }
        }

        return ret;
    }

    void print(Buffer * b);
    bool equals(Buffer *, Buffer *);

protected:
    static TSTP_MAC_NOMF<Radio> * _instance[2048];
    Buffer::List _tx_schedule;
    Buffer * _tx_pending;
    static unsigned int _instances;
    unsigned int _unit;
    cOutVector * _buffer_inout;
    unsigned int _buffer_inout_n;
    cOutVector * _ids_txed;
    Statistics _stats;
    unsigned int _hash_collisions;
    static unsigned int _global_keep_alives_sent;
};

// Class attributes
template<typename Radio>
TSTP_MAC_NOMF<Radio> * TSTP_MAC_NOMF<Radio>::_instance[2048];

template<typename Radio>
unsigned int TSTP_MAC_NOMF<Radio>::_instances;

template<typename Radio>
unsigned int TSTP_MAC_NOMF<Radio>::_seq;

template<typename Radio>
unsigned int TSTP_MAC_NOMF<Radio>::_global_keep_alives_sent;

#include <castalia_nic.h>

class TSTP_MAC_NOMF_NED: public IEEE802_15_4::NIC_Base<IEEE802_15_4, true>, public TSTP_MAC_NOMF<Radio_Engine>
{
    typedef TSTP_MAC_NOMF<Radio_Engine> MAC;
    typedef IEEE802_15_4::NIC_Base<IEEE802_15_4, true> NIC;

public:
    typedef MAC::Buffer Buffer;
    typedef NIC::Statistics Statistics;
    typedef NIC::Address Address;
    typedef NIC::Protocol Protocol;

    // NIC Interface
    unsigned int period() { return MAC::period(); }
    const NIC::Statistics & statistics() { return MAC::statistics(); }
    void reset() { }

    // NIC Timer Interface
    Timer::Time_Stamp read() { return Timer::read(); }
    Timer::Time_Stamp frequency() { return Timer::frequency(); }
    double frequency_error() { return Timer::frequency_error(); }
    Timer::Time_Stamp sfd() { return Timer::read(); } // Not implemented
    void adjust(const Timer::Time_Stamp & t2, const Timer::Time_Stamp & t3) { Timer::adjust(t2, t3); }
    Timer::Time_Stamp us2count(const Timer::Microsecond & us) { return Timer::us2count(us); }
    Timer::Microsecond count2us(const Timer::Time_Stamp & ts) { return Timer::count2us(ts); }

public:
    typedef IEEE802_15_4::Observer Observer;
    typedef IEEE802_15_4::Observed Observed;

    static unsigned int _units;

    TSTP_MAC_NOMF_NED() : MAC(_units++) {
        Timer::_radio = this;
    }

    void startup();

    void finishSpecific();

    void fromNetworkLayer(cPacket *, int) {} // Unused, but required by Castalia

    int send(const Address & dst, const Protocol & prot, const void * data, unsigned int size) {
        Buffer * b = alloc(0, dst, prot, 0, 0, size);
        memcpy(b->frame()->data<void>(), data, size);
        return send(b);
    }

    Buffer * alloc(void * nic, const Address & dst, const Protocol & prot, unsigned int once, unsigned int always, unsigned int payload) {
        // Initialize the buffer
        Buffer * buf = new Buffer(0);
        new (buf) Buffer(nic, once + always + payload + sizeof(MAC::Header));
        MAC::marshal(buf, address(), dst, prot);

        return buf;
    }

    void free(Buffer * buf) { MAC::free(buf); }

    int send(Buffer * buf) { return MAC::send(buf); }

    int receive(Address * src, Protocol * prot, void * data, unsigned int size) { return 0; } // Unused
    const Address & address() { return _address; }
    void address(const Address & address) { }

    void fromRadioLayer(cPacket * pkt, double rssi, double lqi) {
        trace() << "TSTP_MAC_NOMF_NED::fromRadioLayer";
        TSTPMACPacket *macPkt = dynamic_cast <TSTPMACPacket*>(pkt);
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

        buf->sfd_time_stamp = Timer::read() - Timer::us2count(PROCESSING_DELAY * 1000000ull + buf->size() * 1000000ull / Phy_Layer::BYTE_RATE);

        if(MAC::pre_notify(buf)) {
            bool notified = notify(reinterpret_cast<IEEE802_15_4::Header *>(buf->frame())->type(), buf);
            if(!MAC::post_notify(buf) && !notified)
                delete buf; // No one was waiting for this frame
        } else {
            delete buf;
        }

        cancelAndDelete(pkt);
    }


    void mac2net(cPacket * pkt)
    {
        toNetworkLayer(pkt);
    }

private:
    Address _address;
};

#endif


