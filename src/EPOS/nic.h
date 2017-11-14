// EPOS Network Interface Mediator Common Package

#ifndef __nic_h
#define __nic_h

#include <epos_common.h>

__BEGIN_SYS

class NIC_Common
{
protected:
    NIC_Common() {}

public:
    static uint32 htolel(uint32 v) { return htole32(v); }
    static uint16 htoles(uint16 v) { return htole16(v); }
    static uint32 letohl(uint32 v) { return le32toh(v); }
    static uint16 letohs(uint16 v) { return le16toh(v); }

    static uint32 htonl(uint32 v) { return htobe32(v); }
    static uint16 htons(uint16 v) { return htobe16(v); }
    static uint32 ntohl(uint32 v) { return be32toh(v); }
    static uint16 ntohs(uint16 v) { return be16toh(v); }

    // NIC physical address (e.g. MAC)
    template<unsigned int LENGTH>
    class Address
    {
    public:
        enum Null { _NULL = 0 };
        enum Broadcast { BROADCAST = 255 };

    public:
        Address() {}

        Address(const Null &) {
            for(unsigned int i = 0; i < LENGTH; i++)
                _address[i] =  _NULL;
        }

        Address(const Broadcast &) {
            for(unsigned int i = 0; i < LENGTH; i++)
                _address[i] = BROADCAST;
        }

        Address(const char * str) { // String formated as A.B.C.D or A:B:C:D:E:F
            static const char sep = (LENGTH == 4) ? '.' : ':';
            char * token = strchr(str, sep);
            for(unsigned int i = 0; i < LENGTH; i++, ++token, str = token, token = strchr(str, sep))
                _address[i] = atol(str);
        }

        Address(unsigned long a) {
            assert(LENGTH == sizeof(long));
            a = htonl(a);
            memcpy(this, &a, sizeof(long));
        }

        Address operator=(const Address & a) {
            for(unsigned int i = 0; i < LENGTH; ++i)
                _address[i] = a._address[i];
            return a;
        }
        Address operator=(const Address & a) volatile {
            for(unsigned int i = 0; i < LENGTH; ++i)
                _address[i] = a._address[i];
            return a;
        }

        operator bool() const {
            for(unsigned int i = 0; i < LENGTH; ++i) {
                if(_address[i])
                    return true;
            }
            return false;
        }
        operator bool() const volatile {
            for(unsigned int i = 0; i < LENGTH; ++i) {
                if(_address[i])
                    return true;
            }
            return false;
        }

        bool operator==(const Address & a) const {
            for(unsigned int i = 0; i < LENGTH; ++i) {
                if(_address[i] != a._address[i])
                    return false;
            }
            return true;
        }

        bool operator!=(const Address & a) const {
            for(unsigned int i = 0; i < LENGTH; ++i) {
                if(_address[i] != a._address[i])
                    return true;
            }
            return false;
        }

        Address operator&(const Address & a) const {
            Address ret;
            for(unsigned int i = 0; i < LENGTH; ++i)
                ret[i] = _address[i] & a._address[i];
            return ret;
        }

        Address operator|(const Address & a) const {
            Address ret;
            for(unsigned int i = 0; i < LENGTH; ++i)
                ret[i] = _address[i] | a._address[i];
            return ret;
        }

        Address operator~() const {
            Address ret;
            for(unsigned int i = 0; i < LENGTH; ++i)
                ret[i] = ~_address[i];
            return ret;
        }

        unsigned int operator%(unsigned int i) const {
            return _address[LENGTH - 1] % i;
        }

        unsigned char & operator[](const size_t i) { return _address[i]; }
        const unsigned char & operator[](const size_t i) const { return _address[i]; }

        friend OStream & operator<<(OStream & db, const Address & a) {
            db << std::hex;
            for(unsigned int i = 0; i < LENGTH; i++) {
                db << (int)a._address[i];
                if(i < LENGTH - 1)
                    db << ((LENGTH == 4) ? '.' : ':');
            }
            db << std::dec;
            return db;
        }

    private:
        unsigned char _address[LENGTH];
    } __attribute__((packed));

    // NIC protocol id
    typedef unsigned short Protocol;

    // NIC CRCs
    typedef unsigned short CRC16;
    typedef unsigned long CRC32;

    static CRC16 crc16(const void * payload, unsigned short length) {
        const char * data_p = reinterpret_cast<const char *>(payload);
        /*
         * http://www8.cs.umu.se/~isak/snippets/crc-16.c
         *
        //                                      16   12   5
        // this is the CCITT CRC 16 polynomial X  + X  + X  + 1.
        // This works out to be 0x1021, but the way the algorithm works
        // lets us use 0x8408 (the reverse of the bit pattern).  The high
        // bit is always assumed to be set, thus we only use 16 bits to
        // represent the 17 bit value.
        */
        static const unsigned int POLY = 0x8408;
        unsigned char i;
        unsigned int data;
        unsigned int crc = 0xffff;

        if (length == 0)
            return (~crc);

        do {
            for (i=0, data=(unsigned int)0xff & *data_p++;
                i < 8;
                i++, data >>= 1) {
                if ((crc & 0x0001) ^ (data & 0x0001))
                    crc = (crc >> 1) ^ POLY;
                else  crc >>= 1;
            }
        } while (--length);

        crc = ~crc;
        data = crc;
        crc = (crc << 8) | (data >> 8 & 0xff);

        return (crc);
    }

    // NIC statistics
    struct Statistics
    {
        Statistics(): rx_packets(0), tx_packets(0), rx_bytes(0), tx_bytes(0) {}

        unsigned int rx_packets;
        unsigned int tx_packets;
        unsigned int rx_bytes;
        unsigned int tx_bytes;
    };

    // Buffer Metadata added to frames by higher-level protocols
    struct Metadata
    {
        int rssi;                             // Received Signal Strength Indicator
        unsigned long long sfd_time_stamp;    // Start-of-frame reception time stamp
        unsigned int id;                      // Message identifier
        unsigned long long offset;            // MAC contention offset
        bool destined_to_me;                  // Whether this node is the final destination for this message
        bool downlink;                        // Message direction (downlink == from sink to sensor)
        unsigned long long deadline;          // Time until when this message must arrive at the final destination
        unsigned int my_distance;             // This node's distance to the message's final destination
        unsigned int sender_distance;         // Last hop's distance to the message's final destination
        bool is_new;                          // Whether this message was just created by this node
        bool is_microframe;                   // Whether this message is a Microframe
        bool relevant;                        // Whether any component is interested in this message
        bool trusted;                         // If true, this message was successfully verified by the Security Manager
        unsigned int random_backoff_exponent; // Backoff used by the MAC to avoid permanent interference
        unsigned int microframe_count;        // Number of Microframes left until data
        int hint;                             // Inserted in the Hint Microframe field
        unsigned char progress_bits;          // Bitmap for sinks towards which this node makes progress
        unsigned int times_txed;              // Number of times the MAC transmited this buffer
    };

    // NIC Timer Interface (to be implemented by NIC aiming at supporting time-synchronous protocols)
    class Timer
    {
    public:
        typedef uint64 Microsecond;
        typedef uint64 Time_Stamp;
        typedef int64 Offset;

    public:
         Timer() {}

         virtual Time_Stamp frequency() = 0;
         virtual Time_Stamp read() = 0;
         virtual Time_Stamp sfd() = 0;
         virtual double frequency_error() = 0;
         virtual void adjust(const Time_Stamp & t2, const Time_Stamp & t3) = 0;
         virtual Time_Stamp us2count(const Microsecond & us) = 0;
         virtual Microsecond count2us(const Time_Stamp & ts) = 0;
    };

    // Polymorphic NIC Base
    template<typename Family, bool polymorphic = true>
    class NIC_Base: public Family, public Family::Observed, public Timer
    {
    public:
        virtual unsigned int period() = 0; // TODO: TSTP specific
        virtual void mac2net(cPacket * pkt) = 0; // Castalia specific

        using Family::Observer;

        NIC_Base(unsigned int unit = 0) {}
        virtual ~NIC_Base() {}

        virtual int send(const typename Family::Address & dst, const typename Family::Protocol & prot, const void * data, unsigned int size) = 0;
        virtual int receive(typename Family::Address * src, typename Family::Protocol * prot, void * data, unsigned int size) = 0;

        virtual typename Family::Buffer * alloc(void * nic, const typename Family::Address & dst, const typename Family::Protocol & prot, unsigned int once, unsigned int always, unsigned int payload) = 0;
        virtual int send(typename Family::Buffer * buf) = 0;
        virtual void free(typename Family::Buffer * buf) = 0;

        virtual const typename Family::Address & address() = 0;
        virtual void address(const typename Family::Address &) = 0;

        virtual const typename Family::Statistics & statistics() = 0;

        virtual void reset() = 0;
    };

    // Monomorphic NIC Base
    template<typename Family>
    class NIC_Base<Family, false>: public Family, public Family::Observed
    {
    public:
        NIC_Base(unsigned int unit = 0) {}
    };
};

__END_SYS

#endif
