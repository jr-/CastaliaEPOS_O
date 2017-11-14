#ifndef TSTP_H_
#define TSTP_H_

#include <ostream>
#include <cmath>

#include <omnetpp.h>
#include "VirtualRouting.h"
#include "VirtualMobilityManager.h"
#include "TSTPPacket_m.h"

#include <tstp_common.h>
#include <hash.h>
#include <tstp_mac_nomf.h>
#include <tstp_mac.h>
#include <array.h>
#include <list.h>
#include <diffie_hellman.h>
#include <poly1305.h>

__USING_SYS

typedef NIC_Common::NIC_Base<IEEE802_15_4, true> NIC;

class TSTP: public TSTP_Common, private NIC::Observer, public VirtualRouting
{
    template<typename> friend class Smart_Data;
    friend class Epoch;
    friend class Security;

    struct Value_Timestamp {
        typedef uint32 Value;
        typedef simtime_t Timestamp;

        Value value;
        Timestamp timestamp;
    };

protected:
    enum {
        LIFEKEEPER_TIMER,
        KEY_MANAGER_TIMER,
        AUTH_REQUEST_TIMER,
    };

    // Castalia bindings
    void startup();
    void finishSpecific();
    void fromApplicationLayer(cPacket *, const char *);
    void fromMacLayer(cPacket *, int, double, double);
    void timerFiredCallback(int);
    static char _app_id[2048];

public:
    typedef NIC::Buffer Buffer;
    typedef Radio_Engine Radio;

    // Packet
    template<Scale S>
    class _Packet: public Header
    {
    private:
        typedef unsigned char Data[MTU];

    public:
        _Packet() {}

        Header * header() { return this; }

        template<typename T>
        T * data() { return reinterpret_cast<T *>(&_data); }

        friend OStream & operator<<(OStream & db, const _Packet & p) {
            switch(reinterpret_cast<const Header &>(p).type()) {
                case INTEREST:
                    db << reinterpret_cast<const Interest &>(p);
                    break;
                case RESPONSE:
                    db << reinterpret_cast<const Response &>(p);
                    break;
                case COMMAND:
                    db << reinterpret_cast<const Command &>(p);
                    break;
                case CONTROL: {
                    switch(reinterpret_cast<const Control &>(p).subtype()) {
                        case DH_RESPONSE:
                            db << reinterpret_cast<const DH_Response &>(p);
                            break;
                        case AUTH_REQUEST:
                            db << reinterpret_cast<const Auth_Request &>(p);
                            break;
                        case DH_REQUEST:
                            db << reinterpret_cast<const DH_Request &>(p);
                            break;
                        case AUTH_GRANTED:
                            db << reinterpret_cast<const Auth_Granted &>(p);
                            break;
                        case REPORT:
                            db << reinterpret_cast<const Report &>(p);
                            break;
                        case KEEP_ALIVE:
                            db << reinterpret_cast<const Keep_Alive &>(p);
                            break;
                        case EPOCH:
                            db << reinterpret_cast<const Epoch &>(p);
                            break;
                        default:
                            break;
                    }
                }
                default:
                    break;
            }
            return db;
        }

    private:
        Data _data;
    } __attribute__((packed));
    typedef _Packet<SCALE> Packet;


    // TSTP observer/d conditioned to a message's address (ID)
    typedef _UTIL::Data_Observer<Buffer, int64> Observer;
    typedef _UTIL::Data_Observed<Buffer, int64> Observed;


    // Hash to store TSTP Observers by type
    class Interested;
    typedef _UTIL::Hash<Interested, 10, Unit> Interests;
    class Responsive;
    typedef _UTIL::Hash<Responsive, 10, Unit> Responsives;


    // TSTP Messages
    // Each TSTP message is encapsulated in a single package. TSTP does not need nor supports fragmentation.

    // Interest/Response Modes
    enum Mode {
        // Response
        SINGLE = 0, // Only one response is desired for each interest job (desired, but multiple responses are still possible)
        ALL    = 1, // All possible responses (e.g. from different sensors) are desired
        // Interest
        REVOKE = 2  // Revoke an interest
    };

    // Interest Message
    class Interest: public Header
    {
    public:
        Interest(const Region & region, const Unit & unit, const Mode & mode, const Error & precision, const Microsecond & expiry, const Microsecond & period = 0)
        //: Header(INTEREST, 0, 0, now(), here(), here()), _region(region), _unit(unit), _mode(mode), _precision(0), _expiry(expiry), _period(period) {}
        : Header(INTEREST, 0, 0, 0, Coordinates(0,0,0), Coordinates(0,0,0)), _region(region), _unit(unit), _mode(mode), _precision(0), _expiry(expiry), _period(period) {}

        const Unit & unit() const { return _unit; }
        const Region & region() const { return _region; }
        Microsecond period() const { return _period; }
        Time_Offset expiry() const { return _expiry; }
        Mode mode() const { return static_cast<Mode>(_mode); }
        Error precision() const { return static_cast<Error>(_precision); }

        bool time_triggered() { return _period; }
        bool event_driven() { return !time_triggered(); }

        friend OStream & operator<<(OStream & db, const Interest & m) {
            db << reinterpret_cast<const Header &>(m) << ",u=" << m._unit << ",m=" << ((m._mode == ALL) ? 'A' : 'S') << ",e=" << int(m._precision) << ",x=" << m._expiry << ",re=" << m._region << ",p=" << m._period;
            return db;
        }

    protected:
        Region _region;
        Unit _unit;
        unsigned char _mode : 2;
        unsigned char _precision : 6;
        Time_Offset _expiry;
        Microsecond _period; // TODO: should be Time_Offset
        CRC _crc;
    } __attribute__((packed));

    // Command Message
    class Command: public Header
    {
    private:
        typedef unsigned char Data[MTU - sizeof(Region) - sizeof(Unit) - sizeof(CRC)];

    public:
        Command(const Unit & unit, const Region & region)
        //: Header(COMMAND, 0, 0, now(), here(), here()), _region(region), _unit(unit) {}
        : Header(COMMAND, 0, 0, 0, Coordinates(0,0,0), Coordinates(0,0,0)), _region(region), _unit(unit) {}

        const Region & region() const { return _region; }
        const Unit & unit() const { return _unit; }

        template<typename T>
        T * command() { return reinterpret_cast<T *>(&_data); }

        template<typename T>
        T * data() { return reinterpret_cast<T *>(&_data); }

        friend OStream & operator<<(OStream & db, const Command & m) {
            db << reinterpret_cast<const Header &>(m) << ",u=" << m._unit << ",reg=" << m._region;
            return db;
        }

    protected:
        Region _region;
        Unit _unit;
        Data _data;
        CRC _crc;
    } __attribute__((packed));

    // Diffie-Hellman Request Security Bootstrap Control Message
    class DH_Request: public Control
    {
    public:
        DH_Request(const Region::Space & dst, const Public_Key & k)
        //: Control(DH_REQUEST, 0, 0, now(), here(), here()), _destination(dst), _public_key(k) { }
        : Control(DH_REQUEST, 0, 0, 0, Coordinates(0,0,0), Coordinates(0,0,0)), _destination(dst), _public_key(k) { }

        const Region::Space & destination() { return _destination; }
        void destination(const Region::Space & d) { _destination = d; }

        Public_Key key() { return _public_key; }
        void key(const Public_Key & k) { _public_key = k; }

        friend OStream & operator<<(OStream & db, const DH_Request & m) {
            db << reinterpret_cast<const Control &>(m) << ",d=" << m._destination << ",k=" << m._public_key;
            return db;
        }

    private:
        Region::Space _destination;
        Public_Key _public_key;
        CRC _crc;
    //} __attribute__((packed)); // TODO
    };

    // Diffie-Hellman Response Security Bootstrap Control Message
    class DH_Response: public Control
    {
    public:
        DH_Response(const Public_Key & k)
        //: Control(DH_RESPONSE, 0, 0, now(), here(), here()), _public_key(k) { }
        : Control(DH_RESPONSE, 0, 0, 0, Coordinates(0,0,0), Coordinates(0,0,0)), _public_key(k) { }

        Public_Key key() { return _public_key; }
        void key(const Public_Key & k) { _public_key = k; }

        friend OStream & operator<<(OStream & db, const DH_Response & m) {
            db << reinterpret_cast<const Control &>(m) << ",k=" << m._public_key;
            return db;
        }

    private:
        Public_Key _public_key;
        CRC _crc;
    //} __attribute__((packed)); // TODO
    };

    // Authentication Request Security Bootstrap Control Message
    class Auth_Request: public Control
    {
    public:
        Auth_Request(const Node_Auth & a, const OTP & o)
        //: Control(AUTH_REQUEST, 0, 0, now(), here(), here()), _auth(a), _otp(o) { }
        : Control(AUTH_REQUEST, 0, 0, 0, Coordinates(0,0,0), Coordinates(0,0,0)), _auth(a), _otp(o) { }


        const Node_Auth & auth() const { return _auth; }
        void auth(const Node_Auth & a) { _auth = a; }

        const OTP & otp() const { return _otp; }
        void otp(const OTP & o) { _otp = o; }

        friend OStream & operator<<(OStream & db, const Auth_Request & m) {
            db << reinterpret_cast<const Control &>(m) << ",a=" << m._auth << ",o=" << m._otp;
            return db;
        }

    private:
        Node_Auth _auth;
        OTP _otp;
        CRC _crc;
    } __attribute__((packed));

    // Authentication Granted Security Bootstrap Control Message
    class Auth_Granted: public Control
    {
    public:
        Auth_Granted(const Region::Space & dst, const Node_Auth & a)
        //: Control(AUTH_GRANTED, 0, 0, now(), here(), here()), _destination(dst), _auth(a) { }
        : Control(AUTH_GRANTED, 0, 0, 0, Coordinates(0,0,0), Coordinates(0,0,0)), _destination(dst), _auth(a) { }

        const Region::Space & destination() { return _destination; }
        void destination(const Coordinates  & d) { _destination = d; }

        const Node_Auth & auth() const { return _auth; }
        void auth(const Node_Auth & a) { _auth = a; }

        friend OStream & operator<<(OStream & db, const Auth_Granted & m) {
            db << reinterpret_cast<const Control &>(m) << ",d=" << m._destination << ",a=" << m._auth;
            return db;
        }

    private:
        Region::Space _destination;
        Node_Auth _auth;
        CRC _crc;
    // } __attribute__((packed)); // TODO
    };

    // Report Control Message
    class Report: public Control
    {
    public:
        Report(const Unit & unit, const Error & error = 0, bool epoch_request = false)
        //: Control(REPORT, 0, 0, now(), here(), here()), _unit(unit), _error(error), _epoch_request(epoch_request) { }
        : Control(REPORT, 0, 0, 0, Coordinates(0,0,0), Coordinates(0,0,0)), _unit(unit), _error(error), _epoch_request(epoch_request) { }

        const Unit & unit() const { return _unit; }
        Error error() const { return _error; }
        bool epoch_request() const { return _epoch_request; }

        friend OStream & operator<<(OStream & db, const Report & r) {
            db << reinterpret_cast<const Control &>(r) << ",u=" << r._unit << ",e=" << r._error;
            return db;
        }

    private:
        Unit _unit;
        Error _error;
        bool _epoch_request;
        CRC _crc;
    } __attribute__((packed));

    // Epoch Control Message
    class Epoch: public Control
    {
    public:
        Epoch(const Region & dst, const Time & ep, const Global_Coordinates & coordinates)
        //: Control(EPOCH, 0, 0, now(), here(), here()), _destination(dst), _epoch(ep), _coordinates(coordinates) { }
        : Control(EPOCH, 0, 0, 0, Coordinates(0,0,0), Coordinates(0,0,0)), _destination(dst), _epoch(ep), _coordinates(coordinates) { }

        const Region & destination() const { return _destination; }
        const Time epoch() const { return _epoch; }
        const Global_Coordinates & coordinates() const { return _coordinates; }

        friend OStream & operator<<(OStream & db, const Epoch & e) {
            db << reinterpret_cast<const Control &>(e) << ",e=" << e._epoch << ",c=" << e._coordinates;
            return db;
        }

    private:
        Region _destination;
        Time _epoch;
        Global_Coordinates _coordinates;
        CRC _crc;
    } __attribute__((packed));

    // TSTP Smart Data bindings
    // Interested (binder between Interest messages and Smart Data)
    class Interested: public Interest
    {
    public:
        template<typename T>
        Interested(T * data, const Region & region, const Unit & unit, const Mode & mode, const Precision & precision, const Microsecond & expiry, const Microsecond & period = 0)
        : Interest(region, unit, mode, precision, expiry, period), _link(this, T::UNIT) {
            //db<TSTP>(TRC) << "TSTP::Interested(d=" << data << ",r=" << region << ",p=" << period << ") => " << reinterpret_cast<const Interest &>(*this) << endl;
//            _interested.insert(&_link);
            advertise();
        }
        ~Interested() {
            //db<TSTP>(TRC) << "TSTP::~Interested(this=" << this << ")" << endl;
//            _interested.remove(&_link);
            revoke();
        }

        void advertise() { send(); }
        void revoke() { _mode = REVOKE; send(); }

        template<typename Value>
        void command(const Value & v) {
//            //db<TSTP>(TRC) << "TSTP::Interested::command(v=" << v << ")" << endl;
//            Buffer * buf = alloc(sizeof(Command));
//            Command * command = new (buf->frame()->data<Command>()) Command(unit(), region());
//            memcpy(command->command<Value>(), &v, sizeof(Value));
//            marshal(buf);
//            //db<TSTP>(INF) << "TSTP::Interested::command:command=" << command << " => " << (*command) << endl;
//            _nic->send(buf);
        }

    private:
        void send() {
//            //db<TSTP>(TRC) << "TSTP::Interested::send() => " << reinterpret_cast<const Interest &>(*this) << endl;
//            Buffer * buf = alloc(sizeof(Interest));
//            memcpy(buf->frame()->data<Interest>(), this, sizeof(Interest));
//            marshal(buf);
//            _nic->send(buf);
        }

    private:
        Interests::Element _link;
    };

    // Responsive (binder between Smart Data (Sensors) and Response messages)
    class Responsive: public Response
    {
    public:
        template<typename T>
        Responsive(T * data, const Unit & unit, const Error & error, const Time & expiry, bool epoch_request = false)
        : Response(unit, error, expiry), _size(sizeof(Response) - sizeof(Response::Data) + sizeof(typename T::Value)), _t0(0), _t1(0), _link(this, T::UNIT) {
            //db<TSTP>(TRC) << "TSTP::Responsive(d=" << data << ",s=" << _size << ") => " << this << endl;
            //db<TSTP>(INF) << "TSTP::Responsive() => " << reinterpret_cast<const Response &>(*this) << endl;
//            _responsives.insert(&_link);
            advertise(epoch_request);
        }
        ~Responsive() {
            //db<TSTP>(TRC) << "TSTP::~Responsive(this=" << this << ")" << endl;
//            _responsives.remove(&_link);
        }

        using Header::time;
        using Header::origin;

        void t0(const Time & t) { _t0 = t; }
        void t1(const Time & t) { _t1 = t; }
        Time t0() const { return _t0; }
        Time t1() const { return _t1; }

        void respond(const Time & expiry) { send(expiry); }
        void advertise(bool epoch_request = false) {
//            //db<TSTP>(TRC) << "TSTP::Responsive::advertise()" << endl;
//            Buffer * buf = alloc(sizeof(Report));
//            //Report * report = new (buf->frame()->data<Report>()) Report(unit(), error(), epoch_request);
//            new (buf->frame()->data<Report>()) Report(unit(), error(), epoch_request);
//            marshal(buf);
//            //db<TSTP>(INF) << "TSTP::Responsive::advertise:report=" << report << " => " << (*report) << endl;
//            _nic->send(buf);
        }

    private:
        void send(const Time & expiry) {
            if((_time >= _t0) && (_time <= _t1)) {
//                assert(expiry > now());
//                //db<TSTP>(TRC) << "TSTP::Responsive::send(x=" << expiry << ")" << endl;
//                Buffer * buf = alloc(_size);
//                Response * response = buf->frame()->data<Response>();
//                memcpy(response, this, _size);
//                response->expiry(expiry);
//                marshal(buf);
//                //db<TSTP>(INF) << "TSTP::Responsive::send:response=" << response << " => " << (*response) << endl;
//                _nic->send(buf);
            }
        }

    private:
        unsigned int _size;
        Time _t0;
        Time _t1;
        Responsives::Element _link;
    };


    class Router;

    // TSTP Locator
    class Locator: private NIC::Observer
    {
        friend class TSTP::Router;

        typedef char RSSI;

        struct Peer {
            Coordinates coordinates;
            Percent confidence;
            RSSI rssi;
        };

    public:
        Locator(TSTP * tstp) : _tstp(tstp), _bootstrapped(false) {
            //db<TSTP>(TRC) << "TSTP::Locator()" << endl;
            _n_peers = 0;
            _confidence = 0;
            _here = Coordinates(Random::random(), Random::random(), Random::random());
        }
        virtual ~Locator();

        Coordinates here() { return _here; }

        void bootstrap();

        bool synchronized() { return _confidence >= 80; }

        bool bootstrapped() { return _bootstrapped; }

        void marshal(Buffer * buf);

        void update(NIC::Observed * obs, NIC::Protocol prot, NIC::Buffer * buf);

    private:
        void add_peer(Coordinates coord, Percent conf, RSSI r) {
            //db<TSTP>(INF) << "TSTP::Locator::add_peer(c=" << coord << ",conf=" << conf << ",r=" << static_cast<int>(r) << ")" << endl;

            unsigned int idx = -1u;

            for(unsigned int i = 0; i < _n_peers; i++) {
                if(_peers[i].coordinates == coord) {
                    if(_peers[i].confidence > conf)
                       return;
                    else {
                        idx = i;
                        break;
                    }
                }
            }

            if(idx == -1u) {
                if(_n_peers < 3)
                    idx = _n_peers++;
                else
                    for(unsigned int i = 0; i < _n_peers; i++)
                        if((_peers[i].confidence <= conf) && ((idx == -1u) || (conf >= _peers[i].confidence)))
                            idx = i;
            }

            if(idx != -1u) {
                _peers[idx].coordinates = coord;
                _peers[idx].confidence = conf;
                _peers[idx].rssi = r;

                if(_n_peers == 3) {
                    Coordinates new_here = _here.trilaterate(_peers[0].coordinates, _peers[0].rssi + 128, _peers[1].coordinates, _peers[1].rssi + 128, _peers[2].coordinates, _peers[2].rssi + 128);
                    if(new_here != TSTP::sink()) {
                        _here = new_here;
                        _confidence = (_peers[0].confidence + _peers[1].confidence + _peers[2].confidence) * 80 / 100 / 3;
                        //db<TSTP>(INF) << "TSTP::Locator: Location updated: " << _here << ", confidence = " << _confidence << "%" << endl;
                    } else {
                        //db<TSTP>(INF) << "TSTP::Locator: Dropped trilateration that resulted in here == sink" << endl;
                    }
                }
            }
        }

    private:
        Coordinates _here;
        unsigned int _n_peers;
        Percent _confidence;
        Peer _peers[3];
        TSTP * _tstp;
        bool _bootstrapped;
    };


    // TSTP Timekeeper
    class Timekeeper: private NIC::Observer
    {
        friend class TSTP;
        typedef Radio::Timer::Time_Stamp Time_Stamp;
        typedef Radio::Timer::Offset Offset;

        unsigned int SYNC_PERIOD;

    public:
        Timekeeper(TSTP * tstp) : _tstp(tstp), _disable_explicit_sync(false), _total_error(0), _max_error(0), _n_adjusts(0), _first_pass(true), _adjusts(0), _bootstrapped(false) {
            //db<TSTP>(TRC) << "TSTP::Timekeeper()" << endl;
            _t0 = 0;
            _t1 = 0;
            _next_sync = 0;
        }
        virtual ~Timekeeper();

        Time now() {
            return Radio::Timer::count2us(_tstp->_nic->read());
        }

        bool synchronized() {
            if(_disable_explicit_sync)
                return true;
            bool ret = now() <= _next_sync;
            if((!ret) && (now() > _next_sync + SYNC_PERIOD))
                _t1 = 0; // Find a new peer
            return ret;
        }

        void bootstrap();
        bool bootstrapped() { return _bootstrapped; }

        void marshal(Buffer * buf);

        void update(NIC::Observed * obs, NIC::Protocol prot, NIC::Buffer * buf);

    private:
        Time_Stamp _t0;
        Time_Stamp _t1;
        Time_Stamp _next_sync;
        Coordinates _peer;
        TSTP * _tstp;
        bool _disable_explicit_sync;
        Time_Stamp _total_error;
        Offset _max_error;
        unsigned int _n_adjusts;
        bool _first_pass;
        cOutVector * _adjusts;
        bool _bootstrapped;
    };


    // TSTP Router
    class Router: private NIC::Observer
    {
    public:
        bool old_expiry_metric;
        bool effort_metric;
        bool expiry_metric;
        bool random_metric;
        double EXPIRY_BETHA;

    private:
        static const unsigned int CCA_TX_GAP = IEEE802_15_4::CCA_TX_GAP;
        static unsigned int RADIO_RANGE;

    public:
        Router(TSTP * tstp) : _tstp(tstp) {
            //db<TSTP>(TRC) << "TSTP::Router()" << endl;
        }
        virtual ~Router();

        void bootstrap();

        bool synchronized() { return true; }

        void marshal(Buffer * buf);

        void update(NIC::Observed * obs, NIC::Protocol prot, NIC::Buffer * buf);

    private:
        bool forward(Buffer * buf) {
            if(!buf->destined_to_me && (buf->my_distance >= buf->sender_distance)) {
                _tstp->trace() << "not forwarding message from node closer to destination";
                return false;
            }

            // Do not forward messages that come from too far away, to avoid radio range asymmetry
            unsigned int aux = _tstp->here() - buf->frame()->data<Header>()->last_hop();

            if(aux > RADIO_RANGE) {
                _tstp->trace() << "not forwarding message that comes from too far away";
                return false;
            }

            if(expiry_metric) {
                TSTP::Time expiry = buf->deadline;
                const TSTP::Time infinite = -1;

                if(expiry == infinite)
                    return true;

                if(expiry < _tstp->now()) {
                    _tstp->trace() << "not forwarding expired message";
                    return false;
                }

                TSTP::Time best_case_delivery_time = ((buf->my_distance + RADIO_RANGE - 1) / RADIO_RANGE * _tstp->_nic->period()) / 1000000;
                TSTP::Time relative_expiry = expiry - _tstp->now();

                if(best_case_delivery_time > relative_expiry) {
                    _tstp->trace() << "not forwarding message that will expire soon";
                    return false;
                }

                buf->deadline -= best_case_delivery_time;
            }

            buf->progress_bits = 1;
            return true;
        }

        void offset(Buffer * buf) {

            if(random_metric || buf->is_new) {
                buf->offset = Random::random() % buf->offset;
            } else {
                if(buf->my_distance == 0)
                    buf->offset = 0;
                else {
                    buf->offset *= RADIO_RANGE + buf->my_distance - buf->sender_distance;
                    buf->offset /= RADIO_RANGE;
                }
            }

            if(old_expiry_metric) {
                TSTP::Time expiry = _tstp->destination(buf).t1;
                TSTP::Time elapsed;
                if(_tstp->now() > buf->frame()->data<Header>()->time())
                    elapsed = _tstp->now() - buf->frame()->data<Header>()->time();
                else
                    elapsed = expiry;

                buf->offset *= expiry - elapsed;
                buf->offset /= expiry;
            } if(expiry_metric) {
                TSTP::Time expiry = _tstp->destination(buf).t1;
                TSTP::Time elapsed;
                const TSTP::Time infinite = -1;

                if((expiry != infinite) && (_tstp->now() > buf->frame()->data<Header>()->time())) {
                    elapsed = _tstp->now() - buf->frame()->data<Header>()->time();
                    buf->offset *= expiry - elapsed;
                    buf->offset /= expiry;
                }
                else
                    elapsed = 0;

            } if(effort_metric) {
                if(_tstp->_nic->statistics().tx_packets && _tstp->_nic->statistics().tx_relayed) {
                    buf->offset *= _tstp->_nic->statistics().tx_relayed;
                    buf->offset /= _tstp->_nic->statistics().tx_packets;
                }
            }
        }

        TSTP * _tstp;
    };


    // TSTP Security
    class Security: private NIC::Observer
    {
        friend class TSTP;

        unsigned int KEY_MANAGER_PERIOD;
        unsigned long long KEY_EXPIRY;
        unsigned int DHREQUEST_AUTH_DELAY;
        unsigned int POLY_TIME_WINDOW;

    public:
        typedef Diffie_Hellman::Shared_Key Master_Secret;

    private:
        class Peer;
        typedef _UTIL::Simple_List<Peer> Peers;
        class Peer
        {
        public:
            Peer(const Node_ID & id, const Region & v, TSTP * tstp)
               : _id(id), _valid(v), _el(this), _auth_time(0), _tstp(tstp) {
               _tstp->_security->_cipher.encrypt(_id, _id, _auth);
            }

            void valid(const Region & r) { _valid = r; }
            const Region & valid() const { return _valid; }

            bool valid_deploy(const Coordinates & where, const Time & when) {
                return _valid.contains(where, when);
            }

            bool valid_request(const Node_Auth & auth, const Coordinates & where, const Time & when) {
                return !memcmp(auth, _auth, sizeof(Node_Auth)) && _valid.contains(where, when);
            }

            const Time & authentication_time() { return _auth_time; }

            Peers::Element * link() { return &_el; }

            const Master_Secret & master_secret() const { return _master_secret; }
            void master_secret(const Master_Secret & ms) {
                _master_secret = ms;
                _auth_time = _tstp->now();
            }

            const Node_Auth & auth() const { return _auth; }
            const Node_ID & id() const { return _id; }

            friend OStream & operator<<(OStream & db, const Peer & p) {
                db << "{id=" << p._id << ",au=" << p._auth << ",v=" << p._valid << ",ms=" << p._master_secret << ",el=" << &p._el << "}";
                return db;
            }

        private:
            Node_ID _id;
            Node_Auth _auth;
            Region _valid;
            Master_Secret _master_secret;
            Peers::Element _el;
            Time _auth_time;
            TSTP * _tstp;
        };

        struct Pending_Key;
        typedef _UTIL::Simple_List<Pending_Key> Pending_Keys;
        class Pending_Key
        {
        public:
            Pending_Key(const Public_Key & pk, TSTP * tstp) : _tstp(tstp), _master_secret_calculated(false), _creation(_tstp->now()), _public_key(pk), _el(this) {}

            bool expired() { return _tstp->now() - _creation > _tstp->_security->KEY_EXPIRY; }

            const Master_Secret & master_secret() {
                if(_master_secret_calculated)
                    return _master_secret;
                _master_secret = _tstp->_security->_dh.shared_key(_public_key);
                _master_secret_calculated = true;
                //db<TSTP>(INF) << "TSTP::Security::Pending_Key: Master Secret set: " << _master_secret << endl;
                return _master_secret;
            }

            Pending_Keys::Element * link() { return &_el; };

            friend OStream & operator<<(OStream & db, const Pending_Key & p) {
                db << "{msc=" << p._master_secret_calculated << ",c=" << p._creation << ",pk=" << p._public_key << ",ms=" << p._master_secret << ",el=" << &p._el << "}";
                return db;
            }

        private:
            TSTP * _tstp;
            bool _master_secret_calculated;
            Time _creation;
            Public_Key _public_key;
            Master_Secret _master_secret;
            Pending_Keys::Element _el;
        };

    public:
        Security(TSTP * tstp) : _key_manager(false), _dh_requests_open(0), _tstp(tstp), _bootstrapped(false) {
            //db<TSTP>(TRC) << "TSTP::Security()" << endl;
            Bignum<sizeof(Node_ID)> id(_tstp->_unit);
            new (&_id) Node_ID(&id, sizeof(id));
            assert(Cipher::KEY_SIZE == sizeof(Node_ID));
            _cipher.encrypt(_id, _id, _auth);
        }
        virtual ~Security();

        void add_peer(const unsigned char * peer_id, unsigned int id_len, const Region & valid_region) {
            Node_ID id(peer_id, id_len);
            Peer * peer = new Peer(id, valid_region, _tstp);
            //while(CPU::tsl(_peers_lock));
            _pending_peers.insert(peer->link());
            //_peers_lock = false;
            if(!_key_manager) {
                _tstp->setTimer(_tstp->KEY_MANAGER_TIMER, simtime_t(KEY_MANAGER_PERIOD, SIMTIME_US));
                _key_manager = true;
            }
        }

        void bootstrap();
        bool bootstrapped() { return _bootstrapped; }

        bool synchronized() { return true; }

        void marshal(Buffer * buf);

        void update(NIC::Observed * obs, NIC::Protocol prot, NIC::Buffer * buf);

    private:
        void auth_request() {
            _tstp->trace() << "TSTP::Security::update(): Sending Auth_Request" << endl;
            // Send Authentication Request
            assert(_pending_keys.size() > 0);
            Buffer * resp = _tstp->alloc(sizeof(Auth_Request));
            new (resp->frame()) Auth_Request(_auth, otp(_pending_keys.head()->object()->master_secret(), _id));
            _tstp->marshal(resp);
            _tstp->_nic->send(resp);
            _tstp->trace() << "Sent" << endl;
        }

        void pack(unsigned char * msg, const Peer * peer) {
            Time t = _tstp->now() / POLY_TIME_WINDOW;

            const unsigned char * ms = reinterpret_cast<const unsigned char *>(&peer->master_secret());
            const unsigned char * id = reinterpret_cast<const unsigned char *>(&peer->id());

            unsigned char nonce[16];
            memset(nonce, 0, 16);
            memcpy(nonce, &t, sizeof(Time) < 16u ? sizeof(Time) : 16u);

            Poly1305 poly(id, ms);

            poly.stamp(&msg[sizeof(Master_Secret)], nonce, reinterpret_cast<const unsigned char *>(msg), sizeof(Master_Secret));

            if(_tstp->_encrypt) {
                // mi = ms ^ _id
                static const unsigned int MI_SIZE = sizeof(Node_ID) > sizeof(Master_Secret) ? sizeof(Node_ID) : sizeof(Master_Secret);
                unsigned char mi[MI_SIZE];
                unsigned int i;
                for(i = 0; (i < sizeof(Node_ID)) && (i < sizeof(Master_Secret)); i++)
                    mi[i] = id[i] ^ ms[i];
                for(; i < sizeof(Node_ID); i++)
                    mi[i] = id[i];
                for(; i < sizeof(Master_Secret); i++)
                    mi[i] = ms[i];

                OTP key;
                poly.stamp(key, nonce, mi, MI_SIZE);
                _cipher.encrypt(msg, key, msg);
            }
        }

        bool unpack(const Peer * peer, unsigned char * msg, const unsigned char * mac, Time reception_time) {
            unsigned char original_msg[sizeof(Master_Secret)];
            memcpy(original_msg, msg, sizeof(Master_Secret));

            const unsigned char * ms = reinterpret_cast<const unsigned char *>(&peer->master_secret());
            const unsigned char * id = reinterpret_cast<const unsigned char *>(&peer->id());

            // mi = ms ^ _id
            static const unsigned int MI_SIZE = sizeof(Node_ID) > sizeof(Master_Secret) ? sizeof(Node_ID) : sizeof(Master_Secret);
            unsigned char mi[MI_SIZE];
            unsigned int i;
            for(i = 0; (i < sizeof(Node_ID)) && (i < sizeof(Master_Secret)); i++)
                mi[i] = id[i] ^ ms[i];
            for(; i < sizeof(Node_ID); i++)
                mi[i] = id[i];
            for(; i < sizeof(Master_Secret); i++)
                mi[i] = ms[i];

            reception_time /= POLY_TIME_WINDOW;
            _tstp->trace() << "t = " << reception_time << std::endl;

            OTP key;
            unsigned char nonce[16];
            Poly1305 poly(id, ms);

            memset(nonce, 0, 16);
            memcpy(nonce, &reception_time, sizeof(Time) < 16u ? sizeof(Time) : 16u);
            poly.stamp(key, nonce, mi, MI_SIZE);
            if(_tstp->_encrypt)
                _cipher.decrypt(original_msg, key, msg);
            if(poly.verify(mac, nonce, msg, sizeof(Master_Secret)))
                return true;

            reception_time--;
            memset(nonce, 0, 16);
            memcpy(nonce, &reception_time, sizeof(Time) < 16u ? sizeof(Time) : 16u);
            poly.stamp(key, nonce, mi, MI_SIZE);
            if(_tstp->_encrypt)
                _cipher.decrypt(original_msg, key, msg);
            if(poly.verify(mac, nonce, msg, sizeof(Master_Secret)))
                return true;

            reception_time += 2;
            memset(nonce, 0, 16);
            memcpy(nonce, &reception_time, sizeof(Time) < 16u ? sizeof(Time) : 16u);
            poly.stamp(key, nonce, mi, MI_SIZE);
            if(_tstp->_encrypt)
                _cipher.decrypt(original_msg, key, msg);
            if(poly.verify(mac, nonce, msg, sizeof(Master_Secret)))
                return true;

            memcpy(msg, original_msg, sizeof(Master_Secret));
            return false;
        }

        void encrypt(const void * msg, const Peer * peer, unsigned char * out) {
            OTP key = otp(peer->master_secret(), peer->id());
            _cipher.encrypt(msg, key, out);
        }

        OTP otp(const Master_Secret & master_secret, const Node_ID & id) {
            const unsigned char * ms = reinterpret_cast<const unsigned char *>(&master_secret);

            // mi = ms ^ _id
            static const unsigned int MI_SIZE = sizeof(Node_ID) > sizeof(Master_Secret) ? sizeof(Node_ID) : sizeof(Master_Secret);
            unsigned char mi[MI_SIZE];
            unsigned int i;
            for(i = 0; (i < sizeof(Node_ID)) && (i < sizeof(Master_Secret)); i++)
                mi[i] = id[i] ^ ms[i];
            for(; i < sizeof(Node_ID); i++)
                mi[i] = id[i];
            for(; i < sizeof(Master_Secret); i++)
                mi[i] = ms[i];

            Time t = _tstp->now() / POLY_TIME_WINDOW;

            unsigned char nonce[16];
            memset(nonce, 0, 16);
            memcpy(nonce, &t, sizeof(Time) < 16u ? sizeof(Time) : 16u);

            OTP out;
            Poly1305(id, ms).stamp(out, nonce, mi, MI_SIZE);
            return out;
        }

        bool verify_auth_request(const Master_Secret & master_secret, const Node_ID & id, const OTP & otp) {
            const unsigned char * ms = reinterpret_cast<const unsigned char *>(&master_secret);

            // mi = ms ^ _id
            static const unsigned int MI_SIZE = sizeof(Node_ID) > sizeof(Master_Secret) ? sizeof(Node_ID) : sizeof(Master_Secret);
            unsigned char mi[MI_SIZE];
            unsigned int i;
            for(i = 0; (i < sizeof(Node_ID)) && (i < sizeof(Master_Secret)); i++)
                mi[i] = id[i] ^ ms[i];
            for(; i < sizeof(Node_ID); i++)
                mi[i] = id[i];
            for(; i < sizeof(Master_Secret); i++)
                mi[i] = ms[i];

            unsigned char nonce[16];
            Time t = _tstp->now() / POLY_TIME_WINDOW;

            Poly1305 poly(id, ms);

            memset(nonce, 0, 16);
            memcpy(nonce, &t, sizeof(Time) < 16u ? sizeof(Time) : 16u);
            if(poly.verify(otp, nonce, mi, MI_SIZE))
                return true;

            t--;
            memset(nonce, 0, 16);
            memcpy(nonce, &t, sizeof(Time) < 16u ? sizeof(Time) : 16u);
            if(poly.verify(otp, nonce, mi, MI_SIZE))
                return true;

            t += 2;
            memset(nonce, 0, 16);
            memcpy(nonce, &t, sizeof(Time) < 16u ? sizeof(Time) : 16u);
            if(poly.verify(otp, nonce, mi, MI_SIZE))
                return true;

            return false;
        }

        int key_manager() {

            Peers::Element * last_dh_request = 0;

            db<TSTP>(TRC) << "TSTP::Security::key_manager()" << endl;
            CPU::int_disable();
            //while(CPU::tsl(_peers_lock));

            // Cleanup expired pending keys
            Pending_Keys::Element * next_key;
            for(Pending_Keys::Element * el = _pending_keys.head(); el; el = next_key) {
                next_key = el->next();
                Pending_Key * p = el->object();
                if(p->expired()) {
                    _pending_keys.remove(el);
                    delete p;
                    db<TSTP>(INF) << "TSTP::Security::key_manager(): removed pending key" << endl;
                }
            }

            // Cleanup expired peers
            Peers::Element * next;
            for(Peers::Element * el = _trusted_peers.head(); el; el = next) {
                next = el->next();
                Peer * p = el->object();
                if(!p->valid_deploy(p->valid().center, _tstp->now())) {
                    _trusted_peers.remove(el);
                    delete p;
                    db<TSTP>(INF) << "TSTP::Security::key_manager(): permanently removed trusted peer" << endl;
                }
            }
            for(Peers::Element * el = _pending_peers.head(); el; el = next) {
                next = el->next();
                Peer * p = el->object();
                if(!p->valid_deploy(p->valid().center, _tstp->now())) {
                    _pending_peers.remove(el);
                    delete p;
                    db<TSTP>(INF) << "TSTP::Security::key_manager(): permanently removed pending peer" << endl;
                }
            }

            // Cleanup expired established keys
            for(Peers::Element * el = _trusted_peers.head(); el; el = next) {
                next = el->next();
                Peer * p = el->object();
                if(_tstp->now() - p->authentication_time() > KEY_EXPIRY) {
                    _trusted_peers.remove(el);
                    _pending_peers.insert(el);
                    db<TSTP>(INF) << "TSTP::Security::key_manager(): trusted peer's key expired" << endl;
                }
            }

            // Send DH Request to at most one peer
            Peers::Element * el;
            if(last_dh_request && last_dh_request->next())
                el = last_dh_request->next();
            else
                el = _pending_peers.head();

            last_dh_request = 0;

            for(; el; el = el->next()) {
                Peer * p = el->object();
                if(p->valid_deploy(p->valid().center, _tstp->now())) {
                    last_dh_request = el;
                    Buffer * buf = _tstp->alloc(sizeof(DH_Request));
                    new (buf->frame()->data<DH_Request>()) DH_Request(Region::Space(p->valid().center, p->valid().radius), _dh.public_key());
                    _tstp->marshal(buf);
                    _dh_requests_open++;
                    _tstp->_nic->send(buf);
                    db<TSTP>(INF) << "TSTP::Security::key_manager(): Sent DH_Request: "  << *buf->frame()->data<DH_Request>() << endl;
                    break;
                }
            }

            //_peers_lock = false;
            CPU::int_enable();

            _tstp->setTimer(_tstp->KEY_MANAGER_TIMER, simtime_t(KEY_MANAGER_PERIOD, SIMTIME_US));

            return 0;
        }

    private:
        Cipher _cipher;
        Node_ID _id;
        Node_Auth _auth;
        Diffie_Hellman _dh;
        Pending_Keys _pending_keys;
        Peers _pending_peers;
        Peers _trusted_peers;
        volatile bool _peers_lock;
        bool _key_manager;
        unsigned int _dh_requests_open;
        TSTP * _tstp;
        bool _bootstrapped;
    };

    // TSTP Life Keeper explicitly asks for Metadata whenever it's needed
    class Life_Keeper
    {
        friend class TSTP;

        static unsigned int PERIOD;

        Life_Keeper(TSTP * tstp) : _tstp(tstp) {
            PERIOD = _tstp->par("keepAlivePeriod").doubleValue();
            if(PERIOD)
                _tstp->setTimer(_tstp->LIFEKEEPER_TIMER, simtime_t(PERIOD, SIMTIME_US));
        }

    private:
        int life_keeper() {
            _tstp->setTimer(_tstp->LIFEKEEPER_TIMER, simtime_t(PERIOD, SIMTIME_US));
            if(_tstp->here() != _tstp->sink()) {
                _tstp->trace()
                    << "TSTP::Life_Keeper: sending keep alive message" << endl;
                _tstp->keep_alive();
            }
            return 0;
        }

        TSTP * _tstp;
    };

public:
    TSTP();
    ~TSTP();

    // Local network Space-Time
    Coordinates here() { return _locator->here(); }
    Time now() { return _timekeeper->now(); }
    static Coordinates sink(unsigned int unit = 0) { return *_sinks[unit]->object(); }
    Time local(const Time & global) { return global - _epoch; }
    Coordinates local(Global_Coordinates global) {
        global -= _global_coordinates;
        return Coordinates(global.x, global.y, global.z);
    }

    void reset_lifekeeper();

    // Global Space-Time
    Global_Coordinates absolute(const Coordinates & coordinates) { return _global_coordinates + coordinates; }
    Time absolute(const Time & t) {
        if((t == static_cast<Time>(-1)) || (t == 0))
           return t;
        return _epoch + t;
    }
    void epoch(const Time & t) { _epoch = t - now(); }
    void coordinates(const Global_Coordinates & c) { _global_coordinates = c; }

    void attach(Observer * obs, void * subject) { _observed.attach(obs, int64(subject)); }
    void detach(Observer * obs, void * subject) { _observed.detach(obs, int64(subject)); }
    bool notify(void * subject, Buffer * buf);

    template<unsigned int UNIT>
    static void init(const NIC & nic);

protected:
    void bootstrap();

private:
    Region destination(Buffer * buf) {
        switch(buf->frame()->data<Frame>()->type()) {
            case INTEREST:
                return buf->frame()->data<Interest>()->region();
            case RESPONSE:
                return Region(sink(), 0, buf->frame()->data<Response>()->time(), buf->frame()->data<Response>()->expiry());
            case COMMAND:
                return buf->frame()->data<Command>()->region();
            case CONTROL:
                switch(buf->frame()->data<Control>()->subtype()) {
                    default:
                    case DH_RESPONSE:
                    case AUTH_REQUEST: {
                        Time origin = buf->frame()->data<Header>()->time();
                        Time deadline = origin + min(static_cast<unsigned long long>(_security->KEY_MANAGER_PERIOD), _security->KEY_EXPIRY) / 2;
                        return Region(sink(), 0, origin, deadline);
                    }
                    case DH_REQUEST: {
                        Time origin = buf->frame()->data<Header>()->time();
                        Time deadline = origin + min(static_cast<unsigned long long>(_security->KEY_MANAGER_PERIOD), _security->KEY_EXPIRY) / 2;
                        return Region(buf->frame()->data<DH_Request>()->destination().center, buf->frame()->data<DH_Request>()->destination().radius, origin, deadline);
                    }
                    case AUTH_GRANTED: {
                        Time origin = buf->frame()->data<Header>()->time();
                        Time deadline = origin + min(static_cast<unsigned long long>(_security->KEY_MANAGER_PERIOD), _security->KEY_EXPIRY) / 2;
                        return Region(buf->frame()->data<Auth_Granted>()->destination().center, buf->frame()->data<Auth_Granted>()->destination().radius, origin, deadline);
                    }
                    case REPORT: {
                        return Region(sink(), 0, buf->frame()->data<Report>()->time(), -1/*TODO*/);
                    }
                    case KEEP_ALIVE: {
                        while(true) {
                            Coordinates fake(here().x + (Random::random() % (RADIO_RANGE / 3)), here().y + (Random::random() % (RADIO_RANGE / 3)), (here().z + Random::random() % (RADIO_RANGE / 3)));
                            if(fake != here())
                                return Region(fake, 0, 0, -1); // Should never be destined_to_me
                        }
                    }
                    case EPOCH: {
                        return buf->frame()->data<Epoch>()->destination();
                    }
                }
            default:
                //db<TSTP>(ERR) << "TSTP::destination(): ERROR: unrecognized frame type " << buf->frame()->data<Frame>()->type() << endl;
                return Region(TSTP::here(), 0, TSTP::now() - 2, TSTP::now() - 1);
        }
    }

    void keep_alive() {
        trace() << "TSTP::keep_alive()" << endl;
        Buffer * buf = alloc(sizeof(Keep_Alive));
        Keep_Alive * keep_alive = new (buf->frame()->data<Keep_Alive>()) Keep_Alive;
//        id++;
//        id = id & Microframe::ID_MAX;
//        keep_alive->_crc = id;
        marshal(buf);
        buf->deadline = now() + Life_Keeper::PERIOD;
        trace() << "TSTP::keep_alive():keep_alive = " << keep_alive << " => " << (*keep_alive) << endl;
        _nic->send(buf);
    }

    void marshal(Buffer * buf) {
        _locator->marshal(buf);
        _timekeeper->marshal(buf);
        _router->marshal(buf);
        _security->marshal(buf);
    }

    Buffer * alloc(unsigned int size) {
        _allocated_buffers++;
        return _nic->alloc(0, NIC::Address::BROADCAST, NIC::TSTP, 0, 0, size - sizeof(Header));
    }

    void update(NIC::Observed * obs, NIC::Protocol prot, NIC::Buffer * buf);

    typedef _UTIL::Vector<Coordinates, MAX_SINKS> Sinks;

    void schedule_auth_request() {
        // Castalia workaround
        Enter_Method("schedule_auth_request()");
        setTimer(AUTH_REQUEST_TIMER, simtime_t(_security->DHREQUEST_AUTH_DELAY, SIMTIME_US));
    }

private:
    NIC * _nic;
    Interests _interested;
    Responsives _responsives;
    Observed _observed; // Channel protocols are singletons
    Time _epoch;
    Global_Coordinates _global_coordinates;

    unsigned int _unit;
    static unsigned int _units;
    static TSTP * _instances[2048];


    bool _sink;

    Locator * _locator;
    Timekeeper * _timekeeper;
    Router * _router;
    Security * _security;
    Life_Keeper * _life_keeper;

    std::vector<std::pair<Coordinates, Time>> _last_message;

    Time _expiry;

    static Sinks _sinks;
    Sinks::Element * _sinks_element;
    static unsigned int _n_sinks;
    unsigned int _allocated_buffers;
    static Frame_ID id;
    bool _encrypt;
};

#endif
