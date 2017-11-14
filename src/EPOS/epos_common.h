#ifndef EPOS_COMMON_H_
#define EPOS_COMMON_H_

#include <omnetpp.h>
#include <ostream>
#include <cassert>
#include <meta.h>
#include <traits.h>

typedef unsigned int Interrupt_Id;

typedef void (* Interrupt_Handler)(const Interrupt_Id &);

typedef unsigned char Percent;

enum Debug_Error {ERR = 1};
enum Debug_Warning {WRN = 2};
enum Debug_Info {INF = 3};
enum Debug_Trace {TRC = 4};

class Null_Streambuf : public std::streambuf
{
protected:
    virtual int overflow(int c) {
        setp(_dummy_buffer, _dummy_buffer + sizeof(_dummy_buffer));
        return (c == EOF) ? '\0' : c;
    }
private:
    char _dummy_buffer[64];
};

class Null_Stream : public Null_Streambuf, public std::ostream
{
public:
    Null_Stream() : std::ostream(this) { }
};

template<typename T, typename L>
inline std::ostream & db(const L & l) {
    if(Traits<T>::debugged)
        return std::cout;
    else {
        static Null_Stream _null_stream;
        return _null_stream;
    }
}

typedef std::ostream OStream;
typedef std::ostream Debug;

class Random
{
public:
    static int random() { return uniform(0, INT_MAX); }
};

class CPU
{
public:
    static void int_enable() {}
    static void int_disable() {}
};

enum Power_Mode {
    PM_FULL,
    PM_LIGHT,
    PM_SLEEP,
    PM_OFF,
};

//============================================================================
// NAMESPACES AND DEFINITIONS
//============================================================================
namespace EPOS {
    namespace S {
        namespace U {}
        using namespace U;
    }
}

#define __BEGIN_API             namespace EPOS {
#define __END_API               }
#define _API                    ::EPOS

#define __BEGIN_UTIL            namespace EPOS { namespace S { namespace U {
#define __END_UTIL              }}}
#define __USING_UTIL            using namespace S::U;
#define _UTIL                   ::EPOS::S::U

#define __BEGIN_SYS             namespace EPOS { namespace S {
#define __END_SYS               }}
#define __USING_SYS             using namespace EPOS::S;
#define _SYS                    ::EPOS::S

#endif /* EPOS_COMMON_H_ */
