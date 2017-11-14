// EPOS Trustful SpaceTime Protocol Declarations

#ifndef TSTP_COMMON_H_
#define TSTP_COMMON_H_

#include <meta.h>
#include <geometry.h>
#include <ieee802_15_4.h>
#include <climits>

#include <epos_common.h>
#include <array.h>
#include <diffie_hellman.h>

__BEGIN_SYS

class TSTP_Common: public IEEE802_15_4
{

protected:
    static unsigned int RADIO_RANGE; // Approximated radio range of nodes, in centimeters
    static bool drop_expired;

public:
    static const unsigned int PAN = 10; // Nodes
    static const unsigned int SAN = 100; // Nodes
    static const unsigned int LAN = 10000; // Nodes
    static const unsigned int NODES = Traits<Build>::NODES;
    static const unsigned int MAX_SINKS = 64;

    // Version
    // This field is packed first and matches the Frame Type field in the Frame Control in IEEE 802.15.4 MAC.
    // A version number above 4 renders TSTP into the reserved frame type zone and should avoid interference.
    enum Version {
        V0 = 4
    };

    // MAC definitions
    typedef uint16 Frame_ID;
    typedef int32 Hint;
    typedef uint16 CRC;
    typedef uint16 MF_Count;

    // Packet Types
    typedef unsigned char Type;
    enum {
        INTEREST  = 0,
        RESPONSE  = 1,
        COMMAND   = 2,
        CONTROL   = 3
    };

    // Control packet subtypes
    typedef unsigned char Subtype;
    enum {
        DH_REQUEST   = 0,
        DH_RESPONSE  = 1,
        AUTH_REQUEST = 2,
        AUTH_GRANTED = 3,
        REPORT = 4,
        KEEP_ALIVE = 5,
        EPOCH = 6,
    };

    // Scale for local network's geographic coordinates
    enum Scale {
        CMx50_8  = 0,
        CM_16    = 1,
        CMx25_16 = 2,
        CM_32    = 3
    };
    static const Scale SCALE = (NODES <= PAN) ? CMx50_8 : (NODES <= SAN) ? CM_16 : (NODES <= LAN) ? CMx25_16 : CM_32;

    // Time
    typedef uint64 Microsecond;
    typedef uint64 Time;
    typedef int Time_Offset;

    // Geographic Coordinates
    template<Scale S>
    struct _Coordinates: public Point<
                         typename IF<S == CMx50_8, char,
                         typename IF<S == CM_16, short,
                         typename IF<S == CMx25_16, short,
                         typename IF<S == CM_32, int, void
                         >::Result>::Result>::Result>::Result, 3>
    {
        typedef typename IF<S == CMx50_8, char,
                typename IF<S == CM_16, short,
                typename IF<S == CMx25_16, short,
                typename IF<S == CM_32, int, void
                    >::Result>::Result>::Result>::Result Number;

        _Coordinates(Number x = 0, Number y = 0, Number z = 0): Point<Number, 3>(x, y, z) {}
        _Coordinates(const Point<Number, 3> & p): Point<Number, 3>(p) {}
    } __attribute__((packed));
    typedef _Coordinates<SCALE> Coordinates;
    typedef _Coordinates<CM_32> Global_Coordinates;

    // Geographic Region in a time interval (not exactly Spacetime, but ...)
    template<Scale S>
    struct _Region: public Sphere<typename _Coordinates<S>::Number>
    {
        typedef typename _Coordinates<S>::Number Number;
        typedef Sphere<Number> Space;
        typedef typename Space::Center Center;

        _Region(const Center & c, const Number & r, const Time & _t0, const Time & _t1)
        : Space(c, r), t0(_t0), t1(_t1) {}

        bool contains(const Center & c, const Time & t) const {
            return ((t >= t0) && (t <= t1)) && Space::contains(c);
        }

        friend std::ostream & operator<<(std::ostream & db, const _Region & r) {
            db << "{" << reinterpret_cast<const Space &>(r) << ",t0=" << r.t0 << ",t1=" << r.t1 << "}";
            return db;
        }

        Time t0;
        Time t1;
    } __attribute__((packed));
    typedef _Region<SCALE> Region;

    // MAC Preamble Microframe
    class Microframe
    {
        // Format
        // Bit 0            1      12   24     56    72
        //     +------------+-------+----+------+-----+
        //     | all listen | count | id | hint | crc |
        //     +------------+-------+----+------+-----+
        // Bits       1        11     12    32    16
    public:
        static const unsigned int ID_MAX = 0x0fff;
        static const unsigned int COUNT_MAX = 0x07ff;

        Microframe() {}

        Microframe(bool all_listen, const Frame_ID & id, const MF_Count & count, const Hint & hint = 0)
        : _al_count_id_hintl(htolel(all_listen | ((count & 0x07ff) << 1) | ((id & 0x0fff) << 12) | ((hint & 0x0ff) << 24))), _hinth_crcl(htolel((hint & 0xffffff00) >> 8)), _crch(0) {}

        MF_Count count() const { return (letohl(_al_count_id_hintl) & 0x0ffe) >> 1; }

        MF_Count dec_count() {
            MF_Count c = count();
            count(c - 1);
            return c;
        }

        void count(const MF_Count & c) {
            _al_count_id_hintl = htolel((letohl(_al_count_id_hintl) & ~0x0ffe) | (c << 1));
        }

        Frame_ID id() const { return ((letohl(_al_count_id_hintl) & 0x00fff000) >> 12); }

        void id(const Frame_ID & id) {
            _al_count_id_hintl = htolel((letohl(_al_count_id_hintl) & ~0x00fff000) | ((id & 0x00000fff) << 12));
        }

        void all_listen(bool all_listen) {
            if(all_listen)
                _al_count_id_hintl = htolel((letohl(_al_count_id_hintl) | 0x01));
            else
                _al_count_id_hintl = htolel((letohl(_al_count_id_hintl) & ~0x01));
        }

        bool all_listen() const { return letohl(_al_count_id_hintl & 0x01); }

        Hint hint() const { return ((letohl(_al_count_id_hintl) & 0xff000000) >> 24) + ((letohl(_hinth_crcl) & 0x00ffffff) << 8); }
        void hint(const Hint & h) {
            _al_count_id_hintl = htolel((letohl(_al_count_id_hintl) & ~0xff000000) | ((h & 0x0ff) << 24));
            _hinth_crcl = htolel((letohl(_hinth_crcl) & ~0x00ffffff) | ((static_cast<unsigned int>(h & 0xffffff00)) >> 8));
        }

        friend std::ostream & operator<<(std::ostream & db, const Microframe & m) {
            db << "{al=" << m.all_listen() << ",c=" << m.count() << ",id=" << m.id() << ",h=" << m.hint()<< "}";
            return db;
        }

    private:
        unsigned int _al_count_id_hintl; // All listen, Count, ID, Hint LSB
        unsigned int _hinth_crcl; // Hint MSBs, CRC LSB
        unsigned char _crch; // CRC MSB
    } __attribute__((packed));

    // Security types
    typedef _UTIL::Array<unsigned char, 16> Node_ID;
    typedef _UTIL::Array<unsigned char, 16> Node_Auth;
    typedef _UTIL::Array<unsigned char, 16> OTP;
    typedef _UTIL::Array<unsigned char, 16> Message_Auth;
    typedef Diffie_Hellman::Public_Key Public_Key;

    // Packet Header
    template<Scale S>
    class _Header
    {
        // Format
        // Bit 0      3    5  6    0                0         0         0         0         0         0         0         0
        //     +------+----+--+----+----------------+--- ~ ---+--- ~ ---+--- ~ ---+--- ~ ---+--- ~ ---+--- ~ ---+--- ~ ---+--- ~ ---+
        //     | ver  |type|tr|scal|   confidence   |   o.t   |   o.x   |   o.y   |   o.z   |   l.t   |   l.x   |   l.y   |   l.z   |
        //     +------+----+--+----+----------------+--- ~ ---+--- ~ ---+--- ~ ---+--- ~ ---+--- ~ ---+--- ~ ---+--- ~ ---+--- ~ ---+
        // Bits          8                  8            64     8/16/32   8/16/32   8/16/32      64     8/16/32   8/16/32   8/16/32

    public:
        _Header(const Type & t, bool tr = false, unsigned char c = 0, const Time & ot = 0, const Coordinates & o = 0, const Coordinates & l = 0, const Version & v = V0)
        : _config((S & 0x03) << 6 | tr << 5 | (t & 0x03) << 3 | (v & 0x07)), _confidence(c), _time(ot), _origin(o), _last_hop_time(0), _last_hop(l) {}

        Version version() const { return static_cast<Version>(_config & 0x07); }
        void version(const Version & v) { _config = (_config & 0xf8) | (v & 0x07); }

        Type type() const { return static_cast<Type>((_config >> 3) & 0x03); }
        void type(const Type & t) { _config = (_config & 0xe7) | ((t & 0x03) << 3); }

        bool time_request() const { return (_config >> 5) & 0x01; }
        void time_request(bool tr) { _config = (_config & 0xdf) | (tr << 5); }

        Scale scale() const { return static_cast<Scale>((_config >> 6) & 0x03); }
        void scale(const Scale & s) { _config = (_config & 0x3f) | (s & 0x03) << 6; }

        const Coordinates & origin() const { return _origin; }
        void origin(const Coordinates & c) { _origin = c; }

        const Coordinates & last_hop() const { return _last_hop; }
        void last_hop(const Coordinates & c) { _last_hop = c; }

        Time time() const { return _time; }
        void time(const Time & t) { _time = t; }

        Time last_hop_time() const { return _last_hop_time; }
        void last_hop_time(const Time & t) { _last_hop_time = t; }

        unsigned char confidence() const { return _confidence; }
        void confidence(unsigned char c) { _confidence = c; }

        friend std::ostream & operator<<(std::ostream & db, const _Header & h) {
            db << "{v=" << h.version() - V0 << ",t=" << ((h.type() == INTEREST) ? 'I' :  (h.type() == RESPONSE) ? 'R' : (h.type() == COMMAND) ? 'C' : 'T') << ",co=" << h._confidence << ",tr=" << h.time_request() << ",s=" << h.scale() << ",ot=" << h._time << ",o=" << h._origin << ",lt=" << h._last_hop_time << ",l=" << h._last_hop << "}";
            return db;
        }

    protected:
        unsigned char _config;
        unsigned char _confidence;
        Time _time;
        Coordinates _origin;
        Time _last_hop_time; // TODO: change to Time_Offset
        Coordinates _last_hop;
    } __attribute__((packed));
    typedef _Header<SCALE> Header;

    static const unsigned int MTU = IEEE802_15_4::MTU - sizeof(Header);

    // Control Message extended Header
    class Control: public Header
    {
    protected:
        Control(const Subtype & st, bool tr = false, unsigned char c = 0, const Time & ot = 0, const Coordinates & o = 0, const Coordinates & l = 0, const Version & v = V0)
        : Header(CONTROL, tr, c, ot, o, l, v), _subtype(st) {}

    public:
        const Subtype & subtype() const { return _subtype; }

        friend OStream & operator<<(OStream & db, const Control & c) {
            db << reinterpret_cast<const Header &>(c) << ",s=" << (unsigned int)c._subtype;
            return db;
        }

    private:
        Subtype _subtype;
    } __attribute__((packed));

    // Frame
    template<Scale S>
    class _Frame: public Header
    {
    public:
        static const unsigned int MTU = TSTP_Common::MTU;
        typedef unsigned char Data[MTU];

    public:
        _Frame() {}
        _Frame(const Type & type, const Address & src, const Address & dst): Header(type) {} // Just for NIC compatibility
        _Frame(const Type & type, const Address & src, const Address & dst, const void * data, unsigned int size): Header(type) { memcpy(_data, data, size); }

        Header * header() { return this; }

        uint8 length() const { return MTU; } // Fixme: placeholder

        template<typename T>
        T * data() { return reinterpret_cast<T *>(&_data); }

        friend std::ostream & operator<<(std::ostream & db, const _Frame & p) {
            db << "{h=" << reinterpret_cast<const Header &>(p) << ",d=" << p._data << "}";
            return db;
        }

    private:
        Data _data;
    } __attribute__((packed, may_alias));
    typedef _Frame<SCALE> Frame;

    typedef Frame PDU;


    // TSTP encodes SI Units similarly to IEEE 1451 TEDs
    class Unit
    {
    public:
        // Formats
        // Bit       31                                 16                                     0
        //         +--+----------------------------------+-------------------------------------+
        // Digital |0 | type                             | dev                                 |
        //         +--+----------------------------------+-------------------------------------+

        // Bit       31   29   27     24     21     18     15     12      9      6      3      0
        //         +--+----+----+------+------+------+------+------+------+------+------+------+
        // SI      |1 |NUM |MOD |sr+4  |rad+4 |m+4   |kg+4  |s+4   |A+4   |K+4   |mol+4 |cd+4  |
        //         +--+----+----+------+------+------+------+------+------+------+------+------+
        // Bits     1   2    2     3      3      3      3      3      3      3      3      3


        // Valid values for field SI
        enum {
            DIGITAL = 0 << 31, // The Unit is plain digital data. Subsequent 15 bits designate the data type. Lower 16 bits are application-specific, usually a device selector.
            SI      = 1 << 31  // The Unit is SI. Remaining bits are interpreted as specified here.
        };

        // Valid values for field NUM
        enum {
            I32 = 0 << 29, // Value is an integral number stored in the 32 last significant bits of a 32-bit big-endian integer.
            I64 = 1 << 29, // Value is an integral number stored in the 64 last significant bits of a 64-bit big-endian integer.
            F32 = 2 << 29, // Value is a real number stored as an IEEE 754 binary32 big-endian floating point.
            D64 = 3 << 29, // Value is a real number stored as an IEEE 754 binary64 big-endian double precision floating point.
            NUM = D64      // AND mask to select NUM bits
        };

        // Valid values for field MOD
        enum {
            DIR     = 0 << 27, // Unit is described by the product of SI base units raised to the powers recorded in the remaining fields.
            DIV     = 1 << 27, // Unit is U/U, where U is described by the product SI base units raised to the powers recorded in the remaining fields.
            LOG     = 2 << 27, // Unit is log_e(U), where U is described by the product of SI base units raised to the powers recorded in the remaining fields.
            LOG_DIV = 3 << 27, // Unit is log_e(U/U), where U is described by the product of SI base units raised to the powers recorded in the remaining fields.
            MOD = D64          // AND mask to select MOD bits
        };

        // Masks to select the SI units
        enum {
            SR      = 7 << 24,
            RAD     = 7 << 21,
            M       = 7 << 18,
            KG      = 7 << 15,
            S       = 7 << 12,
            A       = 7 <<  9,
            K       = 7 <<  6,
            MOL     = 7 <<  3,
            CD      = 7 <<  0
        };

        // Typical SI Quantities
        enum Quantity {
             //                        si      | mod     | sr            | rad           |  m            |  kg           |  s            |  A            |  K            |  mol          |  cd
             Length                  = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 1) << 18 | (4 + 0) << 15 | (4 + 0) << 12 | (4 + 0) << 9  | (4 + 0) << 6  | (4 + 0) << 3  | (4 + 0),
             Mass                    = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 0) << 18 | (4 + 1) << 15 | (4 + 0) << 12 | (4 + 0) << 9  | (4 + 0) << 6  | (4 + 0) << 3  | (4 + 0),
             Time                    = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 0) << 18 | (4 + 0) << 15 | (4 + 1) << 12 | (4 + 0) << 9  | (4 + 0) << 6  | (4 + 0) << 3  | (4 + 0),
             Current                 = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 0) << 18 | (4 + 0) << 15 | (4 + 0) << 12 | (4 + 1) << 9  | (4 + 0) << 6  | (4 + 0) << 3  | (4 + 0),
             Electric_Current        = Current,
             Temperature             = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 0) << 18 | (4 + 0) << 15 | (4 + 0) << 12 | (4 + 0) << 9  | (4 + 1) << 6  | (4 + 0) << 3  | (4 + 0),
             Amount_of_Substance     = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 0) << 18 | (4 + 0) << 15 | (4 + 0) << 12 | (4 + 0) << 9  | (4 + 0) << 6  | (4 + 1) << 3  | (4 + 0),
             Luminous_Intensity      = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 0) << 18 | (4 + 0) << 15 | (4 + 0) << 12 | (4 + 0) << 9  | (4 + 0) << 6  | (4 + 0) << 3  | (4 + 1),
             Area                    = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 2) << 18 | (4 + 0) << 15 | (4 + 0) << 12 | (4 + 0) << 9  | (4 + 0) << 6  | (4 + 0) << 3  | (4 + 0),
             Volume                  = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 3) << 18 | (4 + 0) << 15 | (4 + 0) << 12 | (4 + 0) << 9  | (4 + 0) << 6  | (4 + 0) << 3  | (4 + 0),
             Speed                   = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 1) << 18 | (4 + 0) << 15 | (4 - 1) << 12 | (4 + 0) << 9  | (4 + 0) << 6  | (4 + 0) << 3  | (4 + 0),
             Velocity                = Speed,
             Acceleration            = 1 << 31 | 0 << 27 | (4 + 0) << 24 | (4 + 0) << 21 | (4 + 1) << 18 | (4 + 0) << 15 | (4 - 2) << 12 | (4 + 0) << 9  | (4 + 0) << 6  | (4 + 0) << 3  | (4 + 0)
         };

        // SI Factors
        typedef char Factor;
        enum {
         // Name           Code         Symbol    Factor
            ATTO        = (8 - 8), //     a       0.000000000000000001
            FEMTO       = (8 - 7), //     f       0.000000000000001
            PICO        = (8 - 6), //     p       0.000000000001
            NANO        = (8 - 5), //     n       0.000000001
            MICRO       = (8 - 4), //     μ       0.000001
            MILI        = (8 - 3), //     m       0.001
            CENTI       = (8 - 2), //     c       0.01
            DECI        = (8 - 1), //     d       0.1
            NONE        = (8    ), //     -       1
            DECA        = (8 + 1), //     da      10
            HECTO       = (8 + 2), //     h       100
            KILO        = (8 + 3), //     k       1000
            MEGA        = (8 + 4), //     M       1000000
            GIGA        = (8 + 5), //     G       1000000000
            TERA        = (8 + 6), //     T       1000000000000
            PETA        = (8 + 7)  //     P       1000000000000000
        };


        template<int N>
        struct Get { typedef typename SWITCH<N, CASE<I32, int, CASE<I64, long long, CASE<DEFAULT, int>>>>::Result Type; };

        template<typename T>
        struct GET { enum { NUM = I32 }; };

    public:
        Unit(unsigned int u) { _unit = u; }

        operator unsigned int() const { return _unit; }

        int sr()  const { return ((_unit & SR)  >> 24) - 4 ; }
        int rad() const { return ((_unit & RAD) >> 21) - 4 ; }
        int m()   const { return ((_unit & M)   >> 18) - 4 ; }
        int kg()  const { return ((_unit & KG)  >> 15) - 4 ; }
        int s()   const { return ((_unit & S)   >> 12) - 4 ; }
        int a()   const { return ((_unit & A)   >>  9) - 4 ; }
        int k()   const { return ((_unit & K)   >>  6) - 4 ; }
        int mol() const { return ((_unit & MOL) >>  3) - 4 ; }
        int cd()  const { return ((_unit & CD)  >>  0) - 4 ; }

        friend std::ostream & operator<<(std::ostream & db, const Unit & u) {
            if(u & SI) {
                db << "{SI";
                switch(u & MOD) {
                case DIR: break;
                case DIV: db << "[U/U]"; break;
                case LOG: db << "[log(U)]"; break;
                case LOG_DIV: db << "[log(U/U)]";
                };
                switch(u & NUM) {
                case I32: db << ".I32"; break;
                case I64: db << ".I64"; break;
                case F32: db << ".F32"; break;
                case D64: db << ".D64";
                }
                db << ':';
                if(u.sr())
                    db << "sr^" << u.sr();
                if(u.rad())
                    db << "rad^" << u.rad();
                if(u.m())
                    db << "m^" << u.m();
                if(u.kg())
                    db << "kg^" << u.kg();
                if(u.s())
                    db << "s^" << u.s();
                if(u.a())
                    db << "A^" << u.a();
                if(u.k())
                    db << "K^" << u.k();
                if(u.mol())
                    db << "mol^" << u.mol();
                if(u.cd())
                    db << "cdr^" << u.cd();
            } else
                db << "{D:" << "l=" <<  (u >> 16);
            db << "}";
            return db;
        }

    private:
        unsigned int _unit;
    } __attribute__((packed));

    // SI values (either integer32, integer64, float32, double64)
    template<int NUM>
    class Value
    {
        typedef typename IF<NUM == TSTP_Common::Unit::I64, long long int,
                typename IF<NUM == TSTP_Common::Unit::F32, float,
                typename IF<NUM == TSTP_Common::Unit::D64, double, int
                    >::Result>::Result>::Result Number;
    public:
        Value(Number v): _value(v) {}

        operator Number() { return _value; }

    private:
        Number _value;
    };

    // Precision or Error in SI values, expressed as 10^Error
    typedef char Precision;
    typedef char Error;



    // Response (Data) Message
    class Response: public Header
    {
        friend class TSTP;
    public:
        typedef unsigned char Data[MTU - sizeof(Unit) - sizeof(Error) - sizeof(Time_Offset) - sizeof(Message_Auth) - sizeof(CRC)];

    public:
        Response(const Unit & unit, const Error & error = 0, const Time & expiry = 0)
        //: Header(RESPONSE, 0, 0, now(), here(), here()), _unit(unit), _error(error), _expiry(expiry) {}
        : Header(RESPONSE, 0, 0, 0, Coordinates(0,0,0), Coordinates(0,0,0)), _unit(unit), _error(error), _expiry(expiry) {}

        const Unit & unit() const { return _unit; }
        Error error() const { return _error; }
        Time expiry() const { return _time + _expiry; }
        void expiry(const Time & e) { _expiry = e - _time; }

        template<typename T>
        void value(const T & v) { *reinterpret_cast<Value<Unit::GET<T>::NUM> *>(&_data) = v; }

        template<typename T>
        T value() { return *reinterpret_cast<Value<Unit::GET<T>::NUM> *>(&_data); }

        template<typename T>
        T * data() { return reinterpret_cast<T *>(&_data); }

        friend std::ostream & operator<<(std::ostream & db, const Response & m) {
            db << reinterpret_cast<const Header &>(m) << ",u=" << m._unit << ",e=" << int(m._error) << ",x=" << m._expiry << ",d=" << std::hex << *const_cast<Response &>(m).data<unsigned>() << std::dec;
            return db;
        }

    protected:
        Unit _unit;
        Error _error;
        Time_Offset _expiry;
        Data _data;
        Message_Auth _auth; // Data size is variable, so _auth is not directly accessible
        CRC _crc;
    } __attribute__((packed));


    // Keep Alive Control Message
    class Keep_Alive: public Control
    {
    public:
        Keep_Alive()
        //: Control(KEEP_ALIVE, 0, 0, now(), here(), here()) { }
        : Control(KEEP_ALIVE, 0, 0, 0, Coordinates(0,0,0), Coordinates(0,0,0)) { }

        friend std::ostream & operator<<(std::ostream & db, const Keep_Alive & k) {
            db << reinterpret_cast<const Control &>(k);
            return db;
        }

    private:
    public:
        CRC _crc;
    } __attribute__((packed));

    /*
     * Copied from a GCC implementation[1] of MurmurHash[2]
     * [1] https://gcc.gnu.org/git/?p=gcc.git;a=blob_plain;f=libstdc%2b%2b-v3/libsupc%2b%2b/hash_bytes.cc;hb=HEAD
     * [2] https://github.com/aappleby/smhasher
     * See also: https://stackoverflow.com/a/19411888
     */
    unsigned int murmur_hash(const void * data, unsigned int len) {
        assert(len >= 4);

        const unsigned long seed = 0xc70f6907UL;
        const unsigned long m = 0x5bd1e995;
        const unsigned char * buf = static_cast<const unsigned char*>(data);

        unsigned int hash = seed ^ len;

        // Mix 4 bytes at a time into the hash.
        while(len >= 4) {
            unsigned int k = reinterpret_cast<const unsigned int *>(buf)[0];
            k *= m;
            k ^= k >> 24;
            k *= m;
            hash *= m;
            hash ^= k;
            buf += 4;
            len -= 4;
        }

        // Handle the last few bytes of the input array.
        switch(len) {
        case 3:
            hash ^= static_cast<unsigned char>(buf[2]) << 16;
        case 2:
            hash ^= static_cast<unsigned char>(buf[1]) << 8;
        case 1:
            hash ^= static_cast<unsigned char>(buf[0]);
        }

        // Do a few final mixes of the hash.
        hash ^= hash >> 13;
        hash *= m;
        hash ^= hash >> 15;
        return hash;
    }
};

__END_SYS

#endif /* TSTP_COMMON_H_ */
