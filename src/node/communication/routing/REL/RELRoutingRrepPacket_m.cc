//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/REL/RELRoutingRrepPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "RELRoutingRrepPacket_m.h"

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

Register_Class(RELRREPPacket);

RELRREPPacket::RELRREPPacket(const char *name, int kind) : ::PacketId(name,kind)
{
    this->hopCount_var = 0;
    this->linkCost_var = 0;
    this->energy_var = 0;
    this->originator_var = 0;
    this->destinationSeqNum_var = 0;
    this->destinationREL_var = 0;
    this->lifetime_var = 0;
}

RELRREPPacket::RELRREPPacket(const RELRREPPacket& other) : ::PacketId(other)
{
    copy(other);
}

RELRREPPacket::~RELRREPPacket()
{
}

RELRREPPacket& RELRREPPacket::operator=(const RELRREPPacket& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void RELRREPPacket::copy(const RELRREPPacket& other)
{
    this->hopCount_var = other.hopCount_var;
    this->linkCost_var = other.linkCost_var;
    this->energy_var = other.energy_var;
    this->originator_var = other.originator_var;
    this->destinationSeqNum_var = other.destinationSeqNum_var;
    this->destinationREL_var = other.destinationREL_var;
    this->lifetime_var = other.lifetime_var;
}

void RELRREPPacket::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
    doPacking(b,this->hopCount_var);
    doPacking(b,this->linkCost_var);
    doPacking(b,this->energy_var);
    doPacking(b,this->originator_var);
    doPacking(b,this->destinationSeqNum_var);
    doPacking(b,this->destinationREL_var);
    doPacking(b,this->lifetime_var);
}

void RELRREPPacket::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
    doUnpacking(b,this->hopCount_var);
    doUnpacking(b,this->linkCost_var);
    doUnpacking(b,this->energy_var);
    doUnpacking(b,this->originator_var);
    doUnpacking(b,this->destinationSeqNum_var);
    doUnpacking(b,this->destinationREL_var);
    doUnpacking(b,this->lifetime_var);
}

int RELRREPPacket::getHopCount() const
{
    return hopCount_var;
}

void RELRREPPacket::setHopCount(int hopCount)
{
    this->hopCount_var = hopCount;
}

int RELRREPPacket::getLinkCost() const
{
    return linkCost_var;
}

void RELRREPPacket::setLinkCost(int linkCost)
{
    this->linkCost_var = linkCost;
}

int RELRREPPacket::getEnergy() const
{
    return energy_var;
}

void RELRREPPacket::setEnergy(int energy)
{
    this->energy_var = energy;
}

const char * RELRREPPacket::getOriginator() const
{
    return originator_var.c_str();
}

void RELRREPPacket::setOriginator(const char * originator)
{
    this->originator_var = originator;
}

unsigned long RELRREPPacket::getDestinationSeqNum() const
{
    return destinationSeqNum_var;
}

void RELRREPPacket::setDestinationSeqNum(unsigned long destinationSeqNum)
{
    this->destinationSeqNum_var = destinationSeqNum;
}

const char * RELRREPPacket::getDestinationREL() const
{
    return destinationREL_var.c_str();
}

void RELRREPPacket::setDestinationREL(const char * destinationREL)
{
    this->destinationREL_var = destinationREL;
}

double RELRREPPacket::getLifetime() const
{
    return lifetime_var;
}

void RELRREPPacket::setLifetime(double lifetime)
{
    this->lifetime_var = lifetime;
}

class RELRREPPacketDescriptor : public cClassDescriptor
{
  public:
    RELRREPPacketDescriptor();
    virtual ~RELRREPPacketDescriptor();

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

Register_ClassDescriptor(RELRREPPacketDescriptor);

RELRREPPacketDescriptor::RELRREPPacketDescriptor() : cClassDescriptor("RELRREPPacket", "PacketId")
{
}

RELRREPPacketDescriptor::~RELRREPPacketDescriptor()
{
}

bool RELRREPPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<RELRREPPacket *>(obj)!=NULL;
}

const char *RELRREPPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int RELRREPPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount(object) : 7;
}

unsigned int RELRREPPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *RELRREPPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "hopCount",
        "linkCost",
        "energy",
        "originator",
        "destinationSeqNum",
        "destinationREL",
        "lifetime",
    };
    return (field>=0 && field<7) ? fieldNames[field] : NULL;
}

int RELRREPPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+0;
    if (fieldName[0]=='l' && strcmp(fieldName, "linkCost")==0) return base+1;
    if (fieldName[0]=='e' && strcmp(fieldName, "energy")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "originator")==0) return base+3;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationSeqNum")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationREL")==0) return base+5;
    if (fieldName[0]=='l' && strcmp(fieldName, "lifetime")==0) return base+6;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *RELRREPPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "string",
        "unsigned long",
        "string",
        "double",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : NULL;
}

const char *RELRREPPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int RELRREPPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    RELRREPPacket *pp = (RELRREPPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string RELRREPPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    RELRREPPacket *pp = (RELRREPPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getHopCount());
        case 1: return long2string(pp->getLinkCost());
        case 2: return long2string(pp->getEnergy());
        case 3: return oppstring2string(pp->getOriginator());
        case 4: return ulong2string(pp->getDestinationSeqNum());
        case 5: return oppstring2string(pp->getDestinationREL());
        case 6: return double2string(pp->getLifetime());
        default: return "";
    }
}

bool RELRREPPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    RELRREPPacket *pp = (RELRREPPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setHopCount(string2long(value)); return true;
        case 1: pp->setLinkCost(string2long(value)); return true;
        case 2: pp->setEnergy(string2long(value)); return true;
        case 3: pp->setOriginator((value)); return true;
        case 4: pp->setDestinationSeqNum(string2ulong(value)); return true;
        case 5: pp->setDestinationREL((value)); return true;
        case 6: pp->setLifetime(string2double(value)); return true;
        default: return false;
    }
}

const char *RELRREPPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *RELRREPPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    RELRREPPacket *pp = (RELRREPPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


