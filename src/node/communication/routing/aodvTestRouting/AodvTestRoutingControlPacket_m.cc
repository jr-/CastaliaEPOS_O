//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/aodvTestRouting/AodvTestRoutingControlPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "AodvTestRoutingControlPacket_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(PacketRREQ);

PacketRREQ::PacketRREQ(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->flagJ_var = 0;
    this->flagR_var = 0;
    this->flagG_var = 0;
    this->flagD_var = 0;
    this->flagU_var = 0;
    this->hopCount_var = 0;
    this->rreqID_var = 0;
    this->dstIP_var = 0;
    this->dstSN_var = 0;
    this->srcIP_var = 0;
    this->srcSN_var = 0;
    this->lifetime_var = 0;
}

PacketRREQ::PacketRREQ(const PacketRREQ& other) : ::RoutingPacket(other)
{
    copy(other);
}

PacketRREQ::~PacketRREQ()
{
}

PacketRREQ& PacketRREQ::operator=(const PacketRREQ& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void PacketRREQ::copy(const PacketRREQ& other)
{
    this->flagJ_var = other.flagJ_var;
    this->flagR_var = other.flagR_var;
    this->flagG_var = other.flagG_var;
    this->flagD_var = other.flagD_var;
    this->flagU_var = other.flagU_var;
    this->hopCount_var = other.hopCount_var;
    this->rreqID_var = other.rreqID_var;
    this->dstIP_var = other.dstIP_var;
    this->dstSN_var = other.dstSN_var;
    this->srcIP_var = other.srcIP_var;
    this->srcSN_var = other.srcSN_var;
    this->lifetime_var = other.lifetime_var;
}

void PacketRREQ::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->flagJ_var);
    doPacking(b,this->flagR_var);
    doPacking(b,this->flagG_var);
    doPacking(b,this->flagD_var);
    doPacking(b,this->flagU_var);
    doPacking(b,this->hopCount_var);
    doPacking(b,this->rreqID_var);
    doPacking(b,this->dstIP_var);
    doPacking(b,this->dstSN_var);
    doPacking(b,this->srcIP_var);
    doPacking(b,this->srcSN_var);
    doPacking(b,this->lifetime_var);
}

void PacketRREQ::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->flagJ_var);
    doUnpacking(b,this->flagR_var);
    doUnpacking(b,this->flagG_var);
    doUnpacking(b,this->flagD_var);
    doUnpacking(b,this->flagU_var);
    doUnpacking(b,this->hopCount_var);
    doUnpacking(b,this->rreqID_var);
    doUnpacking(b,this->dstIP_var);
    doUnpacking(b,this->dstSN_var);
    doUnpacking(b,this->srcIP_var);
    doUnpacking(b,this->srcSN_var);
    doUnpacking(b,this->lifetime_var);
}

bool PacketRREQ::getFlagJ() const
{
    return flagJ_var;
}

void PacketRREQ::setFlagJ(bool flagJ)
{
    this->flagJ_var = flagJ;
}

bool PacketRREQ::getFlagR() const
{
    return flagR_var;
}

void PacketRREQ::setFlagR(bool flagR)
{
    this->flagR_var = flagR;
}

bool PacketRREQ::getFlagG() const
{
    return flagG_var;
}

void PacketRREQ::setFlagG(bool flagG)
{
    this->flagG_var = flagG;
}

bool PacketRREQ::getFlagD() const
{
    return flagD_var;
}

void PacketRREQ::setFlagD(bool flagD)
{
    this->flagD_var = flagD;
}

bool PacketRREQ::getFlagU() const
{
    return flagU_var;
}

void PacketRREQ::setFlagU(bool flagU)
{
    this->flagU_var = flagU;
}

int PacketRREQ::getHopCount() const
{
    return hopCount_var;
}

void PacketRREQ::setHopCount(int hopCount)
{
    this->hopCount_var = hopCount;
}

int PacketRREQ::getRreqID() const
{
    return rreqID_var;
}

void PacketRREQ::setRreqID(int rreqID)
{
    this->rreqID_var = rreqID;
}

const char * PacketRREQ::getDstIP() const
{
    return dstIP_var.c_str();
}

void PacketRREQ::setDstIP(const char * dstIP)
{
    this->dstIP_var = dstIP;
}

unsigned long PacketRREQ::getDstSN() const
{
    return dstSN_var;
}

void PacketRREQ::setDstSN(unsigned long dstSN)
{
    this->dstSN_var = dstSN;
}

const char * PacketRREQ::getSrcIP() const
{
    return srcIP_var.c_str();
}

void PacketRREQ::setSrcIP(const char * srcIP)
{
    this->srcIP_var = srcIP;
}

unsigned long PacketRREQ::getSrcSN() const
{
    return srcSN_var;
}

void PacketRREQ::setSrcSN(unsigned long srcSN)
{
    this->srcSN_var = srcSN;
}

double PacketRREQ::getLifetime() const
{
    return lifetime_var;
}

void PacketRREQ::setLifetime(double lifetime)
{
    this->lifetime_var = lifetime;
}

class PacketRREQDescriptor : public cClassDescriptor
{
  public:
    PacketRREQDescriptor();
    virtual ~PacketRREQDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(PacketRREQDescriptor);

PacketRREQDescriptor::PacketRREQDescriptor() : cClassDescriptor("PacketRREQ", "RoutingPacket")
{
}

PacketRREQDescriptor::~PacketRREQDescriptor()
{
}

bool PacketRREQDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketRREQ *>(obj)!=NULL;
}

const char *PacketRREQDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketRREQDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 12+basedesc->getFieldCount(object) : 12;
}

unsigned int PacketRREQDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<12) ? fieldTypeFlags[field] : 0;
}

const char *PacketRREQDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "flagJ",
        "flagR",
        "flagG",
        "flagD",
        "flagU",
        "hopCount",
        "rreqID",
        "dstIP",
        "dstSN",
        "srcIP",
        "srcSN",
        "lifetime",
    };
    return (field>=0 && field<12) ? fieldNames[field] : NULL;
}

int PacketRREQDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagJ")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagR")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagG")==0) return base+2;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagD")==0) return base+3;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagU")==0) return base+4;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+5;
    if (fieldName[0]=='r' && strcmp(fieldName, "rreqID")==0) return base+6;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstIP")==0) return base+7;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstSN")==0) return base+8;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcIP")==0) return base+9;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcSN")==0) return base+10;
    if (fieldName[0]=='l' && strcmp(fieldName, "lifetime")==0) return base+11;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketRREQDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "int",
        "int",
        "string",
        "unsigned long",
        "string",
        "unsigned long",
        "double",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : NULL;
}

const char *PacketRREQDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int PacketRREQDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREQ *pp = (PacketRREQ *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketRREQDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREQ *pp = (PacketRREQ *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getFlagJ());
        case 1: return bool2string(pp->getFlagR());
        case 2: return bool2string(pp->getFlagG());
        case 3: return bool2string(pp->getFlagD());
        case 4: return bool2string(pp->getFlagU());
        case 5: return long2string(pp->getHopCount());
        case 6: return long2string(pp->getRreqID());
        case 7: return oppstring2string(pp->getDstIP());
        case 8: return ulong2string(pp->getDstSN());
        case 9: return oppstring2string(pp->getSrcIP());
        case 10: return ulong2string(pp->getSrcSN());
        case 11: return double2string(pp->getLifetime());
        default: return "";
    }
}

bool PacketRREQDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREQ *pp = (PacketRREQ *)object; (void)pp;
    switch (field) {
        case 0: pp->setFlagJ(string2bool(value)); return true;
        case 1: pp->setFlagR(string2bool(value)); return true;
        case 2: pp->setFlagG(string2bool(value)); return true;
        case 3: pp->setFlagD(string2bool(value)); return true;
        case 4: pp->setFlagU(string2bool(value)); return true;
        case 5: pp->setHopCount(string2long(value)); return true;
        case 6: pp->setRreqID(string2long(value)); return true;
        case 7: pp->setDstIP((value)); return true;
        case 8: pp->setDstSN(string2ulong(value)); return true;
        case 9: pp->setSrcIP((value)); return true;
        case 10: pp->setSrcSN(string2ulong(value)); return true;
        case 11: pp->setLifetime(string2double(value)); return true;
        default: return false;
    }
}

const char *PacketRREQDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *PacketRREQDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREQ *pp = (PacketRREQ *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(PacketRREP);

PacketRREP::PacketRREP(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->flagR_var = 0;
    this->flagA_var = 0;
    this->prefixSz_var = 0;
    this->hopCount_var = 0;
    this->dstIP_var = 0;
    this->dstSN_var = 0;
    this->originIP_var = 0;
    this->lifetime_var = 0;
    this->rreqID_var = 0;
}

PacketRREP::PacketRREP(const PacketRREP& other) : ::RoutingPacket(other)
{
    copy(other);
}

PacketRREP::~PacketRREP()
{
}

PacketRREP& PacketRREP::operator=(const PacketRREP& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void PacketRREP::copy(const PacketRREP& other)
{
    this->flagR_var = other.flagR_var;
    this->flagA_var = other.flagA_var;
    this->prefixSz_var = other.prefixSz_var;
    this->hopCount_var = other.hopCount_var;
    this->dstIP_var = other.dstIP_var;
    this->dstSN_var = other.dstSN_var;
    this->originIP_var = other.originIP_var;
    this->lifetime_var = other.lifetime_var;
    this->rreqID_var = other.rreqID_var;
}

void PacketRREP::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->flagR_var);
    doPacking(b,this->flagA_var);
    doPacking(b,this->prefixSz_var);
    doPacking(b,this->hopCount_var);
    doPacking(b,this->dstIP_var);
    doPacking(b,this->dstSN_var);
    doPacking(b,this->originIP_var);
    doPacking(b,this->lifetime_var);
    doPacking(b,this->rreqID_var);
}

void PacketRREP::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->flagR_var);
    doUnpacking(b,this->flagA_var);
    doUnpacking(b,this->prefixSz_var);
    doUnpacking(b,this->hopCount_var);
    doUnpacking(b,this->dstIP_var);
    doUnpacking(b,this->dstSN_var);
    doUnpacking(b,this->originIP_var);
    doUnpacking(b,this->lifetime_var);
    doUnpacking(b,this->rreqID_var);
}

bool PacketRREP::getFlagR() const
{
    return flagR_var;
}

void PacketRREP::setFlagR(bool flagR)
{
    this->flagR_var = flagR;
}

bool PacketRREP::getFlagA() const
{
    return flagA_var;
}

void PacketRREP::setFlagA(bool flagA)
{
    this->flagA_var = flagA;
}

int PacketRREP::getPrefixSz() const
{
    return prefixSz_var;
}

void PacketRREP::setPrefixSz(int prefixSz)
{
    this->prefixSz_var = prefixSz;
}

int PacketRREP::getHopCount() const
{
    return hopCount_var;
}

void PacketRREP::setHopCount(int hopCount)
{
    this->hopCount_var = hopCount;
}

const char * PacketRREP::getDstIP() const
{
    return dstIP_var.c_str();
}

void PacketRREP::setDstIP(const char * dstIP)
{
    this->dstIP_var = dstIP;
}

unsigned long PacketRREP::getDstSN() const
{
    return dstSN_var;
}

void PacketRREP::setDstSN(unsigned long dstSN)
{
    this->dstSN_var = dstSN;
}

const char * PacketRREP::getOriginIP() const
{
    return originIP_var.c_str();
}

void PacketRREP::setOriginIP(const char * originIP)
{
    this->originIP_var = originIP;
}

double PacketRREP::getLifetime() const
{
    return lifetime_var;
}

void PacketRREP::setLifetime(double lifetime)
{
    this->lifetime_var = lifetime;
}

int PacketRREP::getRreqID() const
{
    return rreqID_var;
}

void PacketRREP::setRreqID(int rreqID)
{
    this->rreqID_var = rreqID;
}

class PacketRREPDescriptor : public cClassDescriptor
{
  public:
    PacketRREPDescriptor();
    virtual ~PacketRREPDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(PacketRREPDescriptor);

PacketRREPDescriptor::PacketRREPDescriptor() : cClassDescriptor("PacketRREP", "RoutingPacket")
{
}

PacketRREPDescriptor::~PacketRREPDescriptor()
{
}

bool PacketRREPDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketRREP *>(obj)!=NULL;
}

const char *PacketRREPDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketRREPDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount(object) : 9;
}

unsigned int PacketRREPDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<9) ? fieldTypeFlags[field] : 0;
}

const char *PacketRREPDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "flagR",
        "flagA",
        "prefixSz",
        "hopCount",
        "dstIP",
        "dstSN",
        "originIP",
        "lifetime",
        "rreqID",
    };
    return (field>=0 && field<9) ? fieldNames[field] : NULL;
}

int PacketRREPDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagR")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagA")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "prefixSz")==0) return base+2;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+3;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstIP")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstSN")==0) return base+5;
    if (fieldName[0]=='o' && strcmp(fieldName, "originIP")==0) return base+6;
    if (fieldName[0]=='l' && strcmp(fieldName, "lifetime")==0) return base+7;
    if (fieldName[0]=='r' && strcmp(fieldName, "rreqID")==0) return base+8;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketRREPDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "bool",
        "int",
        "int",
        "string",
        "unsigned long",
        "string",
        "double",
        "int",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : NULL;
}

const char *PacketRREPDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int PacketRREPDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREP *pp = (PacketRREP *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketRREPDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREP *pp = (PacketRREP *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getFlagR());
        case 1: return bool2string(pp->getFlagA());
        case 2: return long2string(pp->getPrefixSz());
        case 3: return long2string(pp->getHopCount());
        case 4: return oppstring2string(pp->getDstIP());
        case 5: return ulong2string(pp->getDstSN());
        case 6: return oppstring2string(pp->getOriginIP());
        case 7: return double2string(pp->getLifetime());
        case 8: return long2string(pp->getRreqID());
        default: return "";
    }
}

bool PacketRREPDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREP *pp = (PacketRREP *)object; (void)pp;
    switch (field) {
        case 0: pp->setFlagR(string2bool(value)); return true;
        case 1: pp->setFlagA(string2bool(value)); return true;
        case 2: pp->setPrefixSz(string2long(value)); return true;
        case 3: pp->setHopCount(string2long(value)); return true;
        case 4: pp->setDstIP((value)); return true;
        case 5: pp->setDstSN(string2ulong(value)); return true;
        case 6: pp->setOriginIP((value)); return true;
        case 7: pp->setLifetime(string2double(value)); return true;
        case 8: pp->setRreqID(string2long(value)); return true;
        default: return false;
    }
}

const char *PacketRREPDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *PacketRREPDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREP *pp = (PacketRREP *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(PacketHELLO);

PacketHELLO::PacketHELLO(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->lifetime_var = 0;
}

PacketHELLO::PacketHELLO(const PacketHELLO& other) : ::RoutingPacket(other)
{
    copy(other);
}

PacketHELLO::~PacketHELLO()
{
}

PacketHELLO& PacketHELLO::operator=(const PacketHELLO& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void PacketHELLO::copy(const PacketHELLO& other)
{
    this->lifetime_var = other.lifetime_var;
}

void PacketHELLO::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->lifetime_var);
}

void PacketHELLO::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->lifetime_var);
}

double PacketHELLO::getLifetime() const
{
    return lifetime_var;
}

void PacketHELLO::setLifetime(double lifetime)
{
    this->lifetime_var = lifetime;
}

class PacketHELLODescriptor : public cClassDescriptor
{
  public:
    PacketHELLODescriptor();
    virtual ~PacketHELLODescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(PacketHELLODescriptor);

PacketHELLODescriptor::PacketHELLODescriptor() : cClassDescriptor("PacketHELLO", "RoutingPacket")
{
}

PacketHELLODescriptor::~PacketHELLODescriptor()
{
}

bool PacketHELLODescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketHELLO *>(obj)!=NULL;
}

const char *PacketHELLODescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketHELLODescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int PacketHELLODescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *PacketHELLODescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "lifetime",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int PacketHELLODescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='l' && strcmp(fieldName, "lifetime")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketHELLODescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *PacketHELLODescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int PacketHELLODescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketHELLO *pp = (PacketHELLO *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketHELLODescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketHELLO *pp = (PacketHELLO *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getLifetime());
        default: return "";
    }
}

bool PacketHELLODescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketHELLO *pp = (PacketHELLO *)object; (void)pp;
    switch (field) {
        case 0: pp->setLifetime(string2double(value)); return true;
        default: return false;
    }
}

const char *PacketHELLODescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *PacketHELLODescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketHELLO *pp = (PacketHELLO *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(PacketRREPack);

PacketRREPack::PacketRREPack(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->lifetime_var = 0;
}

PacketRREPack::PacketRREPack(const PacketRREPack& other) : ::RoutingPacket(other)
{
    copy(other);
}

PacketRREPack::~PacketRREPack()
{
}

PacketRREPack& PacketRREPack::operator=(const PacketRREPack& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void PacketRREPack::copy(const PacketRREPack& other)
{
    this->lifetime_var = other.lifetime_var;
}

void PacketRREPack::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->lifetime_var);
}

void PacketRREPack::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->lifetime_var);
}

double PacketRREPack::getLifetime() const
{
    return lifetime_var;
}

void PacketRREPack::setLifetime(double lifetime)
{
    this->lifetime_var = lifetime;
}

class PacketRREPackDescriptor : public cClassDescriptor
{
  public:
    PacketRREPackDescriptor();
    virtual ~PacketRREPackDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(PacketRREPackDescriptor);

PacketRREPackDescriptor::PacketRREPackDescriptor() : cClassDescriptor("PacketRREPack", "RoutingPacket")
{
}

PacketRREPackDescriptor::~PacketRREPackDescriptor()
{
}

bool PacketRREPackDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketRREPack *>(obj)!=NULL;
}

const char *PacketRREPackDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketRREPackDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int PacketRREPackDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *PacketRREPackDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "lifetime",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int PacketRREPackDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='l' && strcmp(fieldName, "lifetime")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketRREPackDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *PacketRREPackDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int PacketRREPackDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREPack *pp = (PacketRREPack *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketRREPackDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREPack *pp = (PacketRREPack *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getLifetime());
        default: return "";
    }
}

bool PacketRREPackDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREPack *pp = (PacketRREPack *)object; (void)pp;
    switch (field) {
        case 0: pp->setLifetime(string2double(value)); return true;
        default: return false;
    }
}

const char *PacketRREPackDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *PacketRREPackDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketRREPack *pp = (PacketRREPack *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(PacketRERR);

PacketRERR::PacketRERR(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->flagN_var = 0;
    this->destCount_var = 0;
    this->lifetime_var = 0;
    unreachableDstIP_arraysize = 0;
    this->unreachableDstIP_var = 0;
    unreachableDstSN_arraysize = 0;
    this->unreachableDstSN_var = 0;
}

PacketRERR::PacketRERR(const PacketRERR& other) : ::RoutingPacket(other)
{
    unreachableDstIP_arraysize = 0;
    this->unreachableDstIP_var = 0;
    unreachableDstSN_arraysize = 0;
    this->unreachableDstSN_var = 0;
    copy(other);
}

PacketRERR::~PacketRERR()
{
    delete [] unreachableDstIP_var;
    delete [] unreachableDstSN_var;
}

PacketRERR& PacketRERR::operator=(const PacketRERR& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void PacketRERR::copy(const PacketRERR& other)
{
    this->flagN_var = other.flagN_var;
    this->destCount_var = other.destCount_var;
    this->lifetime_var = other.lifetime_var;
    delete [] this->unreachableDstIP_var;
    this->unreachableDstIP_var = (other.unreachableDstIP_arraysize==0) ? NULL : new opp_string[other.unreachableDstIP_arraysize];
    unreachableDstIP_arraysize = other.unreachableDstIP_arraysize;
    for (unsigned int i=0; i<unreachableDstIP_arraysize; i++)
        this->unreachableDstIP_var[i] = other.unreachableDstIP_var[i];
    delete [] this->unreachableDstSN_var;
    this->unreachableDstSN_var = (other.unreachableDstSN_arraysize==0) ? NULL : new unsigned long[other.unreachableDstSN_arraysize];
    unreachableDstSN_arraysize = other.unreachableDstSN_arraysize;
    for (unsigned int i=0; i<unreachableDstSN_arraysize; i++)
        this->unreachableDstSN_var[i] = other.unreachableDstSN_var[i];
}

void PacketRERR::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->flagN_var);
    doPacking(b,this->destCount_var);
    doPacking(b,this->lifetime_var);
    b->pack(unreachableDstIP_arraysize);
    doPacking(b,this->unreachableDstIP_var,unreachableDstIP_arraysize);
    b->pack(unreachableDstSN_arraysize);
    doPacking(b,this->unreachableDstSN_var,unreachableDstSN_arraysize);
}

void PacketRERR::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->flagN_var);
    doUnpacking(b,this->destCount_var);
    doUnpacking(b,this->lifetime_var);
    delete [] this->unreachableDstIP_var;
    b->unpack(unreachableDstIP_arraysize);
    if (unreachableDstIP_arraysize==0) {
        this->unreachableDstIP_var = 0;
    } else {
        this->unreachableDstIP_var = new opp_string[unreachableDstIP_arraysize];
        doUnpacking(b,this->unreachableDstIP_var,unreachableDstIP_arraysize);
    }
    delete [] this->unreachableDstSN_var;
    b->unpack(unreachableDstSN_arraysize);
    if (unreachableDstSN_arraysize==0) {
        this->unreachableDstSN_var = 0;
    } else {
        this->unreachableDstSN_var = new unsigned long[unreachableDstSN_arraysize];
        doUnpacking(b,this->unreachableDstSN_var,unreachableDstSN_arraysize);
    }
}

bool PacketRERR::getFlagN() const
{
    return flagN_var;
}

void PacketRERR::setFlagN(bool flagN)
{
    this->flagN_var = flagN;
}

int PacketRERR::getDestCount() const
{
    return destCount_var;
}

void PacketRERR::setDestCount(int destCount)
{
    this->destCount_var = destCount;
}

double PacketRERR::getLifetime() const
{
    return lifetime_var;
}

void PacketRERR::setLifetime(double lifetime)
{
    this->lifetime_var = lifetime;
}

void PacketRERR::setUnreachableDstIPArraySize(unsigned int size)
{
    opp_string *unreachableDstIP_var2 = (size==0) ? NULL : new opp_string[size];
    unsigned int sz = unreachableDstIP_arraysize < size ? unreachableDstIP_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        unreachableDstIP_var2[i] = this->unreachableDstIP_var[i];
    for (unsigned int i=sz; i<size; i++)
        unreachableDstIP_var2[i] = 0;
    unreachableDstIP_arraysize = size;
    delete [] this->unreachableDstIP_var;
    this->unreachableDstIP_var = unreachableDstIP_var2;
}

unsigned int PacketRERR::getUnreachableDstIPArraySize() const
{
    return unreachableDstIP_arraysize;
}

const char * PacketRERR::getUnreachableDstIP(unsigned int k) const
{
    if (k>=unreachableDstIP_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDstIP_arraysize, k);
    return unreachableDstIP_var[k].c_str();
}

void PacketRERR::setUnreachableDstIP(unsigned int k, const char * unreachableDstIP)
{
    if (k>=unreachableDstIP_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDstIP_arraysize, k);
    this->unreachableDstIP_var[k] = unreachableDstIP;
}

void PacketRERR::setUnreachableDstSNArraySize(unsigned int size)
{
    unsigned long *unreachableDstSN_var2 = (size==0) ? NULL : new unsigned long[size];
    unsigned int sz = unreachableDstSN_arraysize < size ? unreachableDstSN_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        unreachableDstSN_var2[i] = this->unreachableDstSN_var[i];
    for (unsigned int i=sz; i<size; i++)
        unreachableDstSN_var2[i] = 0;
    unreachableDstSN_arraysize = size;
    delete [] this->unreachableDstSN_var;
    this->unreachableDstSN_var = unreachableDstSN_var2;
}

unsigned int PacketRERR::getUnreachableDstSNArraySize() const
{
    return unreachableDstSN_arraysize;
}

unsigned long PacketRERR::getUnreachableDstSN(unsigned int k) const
{
    if (k>=unreachableDstSN_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDstSN_arraysize, k);
    return unreachableDstSN_var[k];
}

void PacketRERR::setUnreachableDstSN(unsigned int k, unsigned long unreachableDstSN)
{
    if (k>=unreachableDstSN_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDstSN_arraysize, k);
    this->unreachableDstSN_var[k] = unreachableDstSN;
}

class PacketRERRDescriptor : public cClassDescriptor
{
  public:
    PacketRERRDescriptor();
    virtual ~PacketRERRDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(PacketRERRDescriptor);

PacketRERRDescriptor::PacketRERRDescriptor() : cClassDescriptor("PacketRERR", "RoutingPacket")
{
}

PacketRERRDescriptor::~PacketRERRDescriptor()
{
}

bool PacketRERRDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketRERR *>(obj)!=NULL;
}

const char *PacketRERRDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketRERRDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int PacketRERRDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *PacketRERRDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "flagN",
        "destCount",
        "lifetime",
        "unreachableDstIP",
        "unreachableDstSN",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int PacketRERRDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagN")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destCount")==0) return base+1;
    if (fieldName[0]=='l' && strcmp(fieldName, "lifetime")==0) return base+2;
    if (fieldName[0]=='u' && strcmp(fieldName, "unreachableDstIP")==0) return base+3;
    if (fieldName[0]=='u' && strcmp(fieldName, "unreachableDstSN")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketRERRDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "int",
        "double",
        "string",
        "unsigned long",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *PacketRERRDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int PacketRERRDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketRERR *pp = (PacketRERR *)object; (void)pp;
    switch (field) {
        case 3: return pp->getUnreachableDstIPArraySize();
        case 4: return pp->getUnreachableDstSNArraySize();
        default: return 0;
    }
}

std::string PacketRERRDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketRERR *pp = (PacketRERR *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getFlagN());
        case 1: return long2string(pp->getDestCount());
        case 2: return double2string(pp->getLifetime());
        case 3: return oppstring2string(pp->getUnreachableDstIP(i));
        case 4: return ulong2string(pp->getUnreachableDstSN(i));
        default: return "";
    }
}

bool PacketRERRDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketRERR *pp = (PacketRERR *)object; (void)pp;
    switch (field) {
        case 0: pp->setFlagN(string2bool(value)); return true;
        case 1: pp->setDestCount(string2long(value)); return true;
        case 2: pp->setLifetime(string2double(value)); return true;
        case 3: pp->setUnreachableDstIP(i,(value)); return true;
        case 4: pp->setUnreachableDstSN(i,string2ulong(value)); return true;
        default: return false;
    }
}

const char *PacketRERRDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *PacketRERRDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketRERR *pp = (PacketRERR *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


