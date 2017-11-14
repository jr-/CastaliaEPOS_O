//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/mac/ieee802154/ieee802154Packet.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ieee802154Packet_m.h"

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

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("IEEE802154Packet_type");
    if (!e) enums.getInstance()->add(e = new cEnum("IEEE802154Packet_type"));
    e->insert(IEEE802154_BEACON_PACKET, "IEEE802154_BEACON_PACKET");
    e->insert(IEEE802154_ASSOCIATE_PACKET, "IEEE802154_ASSOCIATE_PACKET");
    e->insert(IEEE802154_DATA_PACKET, "IEEE802154_DATA_PACKET");
    e->insert(IEEE802154_ACK_PACKET, "IEEE802154_ACK_PACKET");
    e->insert(IEEE802154_GTS_REQUEST_PACKET, "IEEE802154_GTS_REQUEST_PACKET");
);

Register_Class(IEEE802154Packet);

IEEE802154Packet::IEEE802154Packet(const char *name, int kind) : ::MacPacket(name,kind)
{
    this->IEEE802154PacketType_var = 0;
    this->ackRequest_var = 0;
    this->seqNum_var = 0;
}

IEEE802154Packet::IEEE802154Packet(const IEEE802154Packet& other) : ::MacPacket(other)
{
    copy(other);
}

IEEE802154Packet::~IEEE802154Packet()
{
}

IEEE802154Packet& IEEE802154Packet::operator=(const IEEE802154Packet& other)
{
    if (this==&other) return *this;
    ::MacPacket::operator=(other);
    copy(other);
    return *this;
}

void IEEE802154Packet::copy(const IEEE802154Packet& other)
{
    this->IEEE802154PacketType_var = other.IEEE802154PacketType_var;
    this->ackRequest_var = other.ackRequest_var;
    this->seqNum_var = other.seqNum_var;
}

void IEEE802154Packet::parsimPack(cCommBuffer *b)
{
    ::MacPacket::parsimPack(b);
    doPacking(b,this->IEEE802154PacketType_var);
    doPacking(b,this->ackRequest_var);
    doPacking(b,this->seqNum_var);
}

void IEEE802154Packet::parsimUnpack(cCommBuffer *b)
{
    ::MacPacket::parsimUnpack(b);
    doUnpacking(b,this->IEEE802154PacketType_var);
    doUnpacking(b,this->ackRequest_var);
    doUnpacking(b,this->seqNum_var);
}

int IEEE802154Packet::getIEEE802154PacketType() const
{
    return IEEE802154PacketType_var;
}

void IEEE802154Packet::setIEEE802154PacketType(int IEEE802154PacketType)
{
    this->IEEE802154PacketType_var = IEEE802154PacketType;
}

bool IEEE802154Packet::getAckRequest() const
{
    return ackRequest_var;
}

void IEEE802154Packet::setAckRequest(bool ackRequest)
{
    this->ackRequest_var = ackRequest;
}

char IEEE802154Packet::getSeqNum() const
{
    return seqNum_var;
}

void IEEE802154Packet::setSeqNum(char seqNum)
{
    this->seqNum_var = seqNum;
}

class IEEE802154PacketDescriptor : public cClassDescriptor
{
  public:
    IEEE802154PacketDescriptor();
    virtual ~IEEE802154PacketDescriptor();

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

Register_ClassDescriptor(IEEE802154PacketDescriptor);

IEEE802154PacketDescriptor::IEEE802154PacketDescriptor() : cClassDescriptor("IEEE802154Packet", "MacPacket")
{
}

IEEE802154PacketDescriptor::~IEEE802154PacketDescriptor()
{
}

bool IEEE802154PacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<IEEE802154Packet *>(obj)!=NULL;
}

const char *IEEE802154PacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int IEEE802154PacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int IEEE802154PacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *IEEE802154PacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "IEEE802154PacketType",
        "ackRequest",
        "seqNum",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int IEEE802154PacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='I' && strcmp(fieldName, "IEEE802154PacketType")==0) return base+0;
    if (fieldName[0]=='a' && strcmp(fieldName, "ackRequest")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqNum")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *IEEE802154PacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "bool",
        "char",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *IEEE802154PacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "IEEE802154Packet_type";
            return NULL;
        default: return NULL;
    }
}

int IEEE802154PacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    IEEE802154Packet *pp = (IEEE802154Packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string IEEE802154PacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    IEEE802154Packet *pp = (IEEE802154Packet *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getIEEE802154PacketType());
        case 1: return bool2string(pp->getAckRequest());
        case 2: return long2string(pp->getSeqNum());
        default: return "";
    }
}

bool IEEE802154PacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    IEEE802154Packet *pp = (IEEE802154Packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setIEEE802154PacketType(string2long(value)); return true;
        case 1: pp->setAckRequest(string2bool(value)); return true;
        case 2: pp->setSeqNum(string2long(value)); return true;
        default: return false;
    }
}

const char *IEEE802154PacketDescriptor::getFieldStructName(void *object, int field) const
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

void *IEEE802154PacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    IEEE802154Packet *pp = (IEEE802154Packet *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


