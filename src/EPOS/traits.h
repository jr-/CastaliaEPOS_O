#ifndef __traits_h
#define __traits_h

template <typename T>
struct Traits
{
    static const bool hysterically_debugged = false;
    static const bool debugged = false;
    static const bool error = false;
    static const bool warning = false;
    static const bool info = false;
    static const bool trace = false;
};

class Build;
template<> struct Traits<Build> : public Traits<void>
{
    static const unsigned int NODES = 10001;
};

#endif
