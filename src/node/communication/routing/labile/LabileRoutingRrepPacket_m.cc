//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/labile/LabileRoutingRrepPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "LabileRoutingRrepPacket_m.h"

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

Register_Class(LabileRREPPacket);

LabileRREPPacket::LabileRREPPacket(const char *name, int kind) : ::PacketId(name,kind)
{
    this->flagR_var = 0;
    this->flagA_var = 0;
    this->prefixSz_var = 0;
    this->hopCount_var = 0;
    this->weakLinks_var = 0;
    this->originator_var = 0;
    this->destinationSeqNum_var = 0;
    this->destinationLABILE_var = 0;
    this->lifetime_var = 0;
}

LabileRREPPacket::LabileRREPPacket(const LabileRREPPacket& other) : ::PacketId(other)
{
    copy(other);
}

LabileRREPPacket::~LabileRREPPacket()
{
}

LabileRREPPacket& LabileRREPPacket::operator=(const LabileRREPPacket& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void LabileRREPPacket::copy(const LabileRREPPacket& other)
{
    this->flagR_var = other.flagR_var;
    this->flagA_var = other.flagA_var;
    this->prefixSz_var = other.prefixSz_var;
    this->hopCount_var = other.hopCount_var;
    this->weakLinks_var = other.weakLinks_var;
    this->originator_var = other.originator_var;
    this->destinationSeqNum_var = other.destinationSeqNum_var;
    this->destinationLABILE_var = other.destinationLABILE_var;
    this->lifetime_var = other.lifetime_var;
}

void LabileRREPPacket::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
    doPacking(b,this->flagR_var);
    doPacking(b,this->flagA_var);
    doPacking(b,this->prefixSz_var);
    doPacking(b,this->hopCount_var);
    doPacking(b,this->weakLinks_var);
    doPacking(b,this->originator_var);
    doPacking(b,this->destinationSeqNum_var);
    doPacking(b,this->destinationLABILE_var);
    doPacking(b,this->lifetime_var);
}

void LabileRREPPacket::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
    doUnpacking(b,this->flagR_var);
    doUnpacking(b,this->flagA_var);
    doUnpacking(b,this->prefixSz_var);
    doUnpacking(b,this->hopCount_var);
    doUnpacking(b,this->weakLinks_var);
    doUnpacking(b,this->originator_var);
    doUnpacking(b,this->destinationSeqNum_var);
    doUnpacking(b,this->destinationLABILE_var);
    doUnpacking(b,this->lifetime_var);
}

bool LabileRREPPacket::getFlagR() const
{
    return flagR_var;
}

void LabileRREPPacket::setFlagR(bool flagR)
{
    this->flagR_var = flagR;
}

bool LabileRREPPacket::getFlagA() const
{
    return flagA_var;
}

void LabileRREPPacket::setFlagA(bool flagA)
{
    this->flagA_var = flagA;
}

int LabileRREPPacket::getPrefixSz() const
{
    return prefixSz_var;
}

void LabileRREPPacket::setPrefixSz(int prefixSz)
{
    this->prefixSz_var = prefixSz;
}

int LabileRREPPacket::getHopCount() const
{
    return hopCount_var;
}

void LabileRREPPacket::setHopCount(int hopCount)
{
    this->hopCount_var = hopCount;
}

int LabileRREPPacket::getWeakLinks() const
{
    return weakLinks_var;
}

void LabileRREPPacket::setWeakLinks(int weakLinks)
{
    this->weakLinks_var = weakLinks;
}

const char * LabileRREPPacket::getOriginator() const
{
    return originator_var.c_str();
}

void LabileRREPPacket::setOriginator(const char * originator)
{
    this->originator_var = originator;
}

unsigned long LabileRREPPacket::getDestinationSeqNum() const
{
    return destinationSeqNum_var;
}

void LabileRREPPacket::setDestinationSeqNum(unsigned long destinationSeqNum)
{
    this->destinationSeqNum_var = destinationSeqNum;
}

const char * LabileRREPPacket::getDestinationLABILE() const
{
    return destinationLABILE_var.c_str();
}

void LabileRREPPacket::setDestinationLABILE(const char * destinationLABILE)
{
    this->destinationLABILE_var = destinationLABILE;
}

double LabileRREPPacket::getLifetime() const
{
    return lifetime_var;
}

void LabileRREPPacket::setLifetime(double lifetime)
{
    this->lifetime_var = lifetime;
}

class LabileRREPPacketDescriptor : public cClassDescriptor
{
  public:
    LabileRREPPacketDescriptor();
    virtual ~LabileRREPPacketDescriptor();

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

Register_ClassDescriptor(LabileRREPPacketDescriptor);

LabileRREPPacketDescriptor::LabileRREPPacketDescriptor() : cClassDescriptor("LabileRREPPacket", "PacketId")
{
}

LabileRREPPacketDescriptor::~LabileRREPPacketDescriptor()
{
}

bool LabileRREPPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<LabileRREPPacket *>(obj)!=NULL;
}

const char *LabileRREPPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int LabileRREPPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount(object) : 9;
}

unsigned int LabileRREPPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *LabileRREPPacketDescriptor::getFieldName(void *object, int field) const
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
        "weakLinks",
        "originator",
        "destinationSeqNum",
        "destinationLABILE",
        "lifetime",
    };
    return (field>=0 && field<9) ? fieldNames[field] : NULL;
}

int LabileRREPPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagR")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagA")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "prefixSz")==0) return base+2;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+3;
    if (fieldName[0]=='w' && strcmp(fieldName, "weakLinks")==0) return base+4;
    if (fieldName[0]=='o' && strcmp(fieldName, "originator")==0) return base+5;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationSeqNum")==0) return base+6;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationLABILE")==0) return base+7;
    if (fieldName[0]=='l' && strcmp(fieldName, "lifetime")==0) return base+8;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *LabileRREPPacketDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
        "string",
        "unsigned long",
        "string",
        "double",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : NULL;
}

const char *LabileRREPPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int LabileRREPPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREPPacket *pp = (LabileRREPPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string LabileRREPPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREPPacket *pp = (LabileRREPPacket *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getFlagR());
        case 1: return bool2string(pp->getFlagA());
        case 2: return long2string(pp->getPrefixSz());
        case 3: return long2string(pp->getHopCount());
        case 4: return long2string(pp->getWeakLinks());
        case 5: return oppstring2string(pp->getOriginator());
        case 6: return ulong2string(pp->getDestinationSeqNum());
        case 7: return oppstring2string(pp->getDestinationLABILE());
        case 8: return double2string(pp->getLifetime());
        default: return "";
    }
}

bool LabileRREPPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREPPacket *pp = (LabileRREPPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setFlagR(string2bool(value)); return true;
        case 1: pp->setFlagA(string2bool(value)); return true;
        case 2: pp->setPrefixSz(string2long(value)); return true;
        case 3: pp->setHopCount(string2long(value)); return true;
        case 4: pp->setWeakLinks(string2long(value)); return true;
        case 5: pp->setOriginator((value)); return true;
        case 6: pp->setDestinationSeqNum(string2ulong(value)); return true;
        case 7: pp->setDestinationLABILE((value)); return true;
        case 8: pp->setLifetime(string2double(value)); return true;
        default: return false;
    }
}

const char *LabileRREPPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *LabileRREPPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREPPacket *pp = (LabileRREPPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


