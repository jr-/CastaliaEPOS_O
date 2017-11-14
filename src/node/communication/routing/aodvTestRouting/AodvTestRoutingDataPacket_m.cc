//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/aodvTestRouting/AodvTestRoutingDataPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "AodvTestRoutingDataPacket_m.h"

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

Register_Class(PacketDATA);

PacketDATA::PacketDATA(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->destinationAodv_var = 0;
}

PacketDATA::PacketDATA(const PacketDATA& other) : ::RoutingPacket(other)
{
    copy(other);
}

PacketDATA::~PacketDATA()
{
}

PacketDATA& PacketDATA::operator=(const PacketDATA& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void PacketDATA::copy(const PacketDATA& other)
{
    this->destinationAodv_var = other.destinationAodv_var;
}

void PacketDATA::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->destinationAodv_var);
}

void PacketDATA::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->destinationAodv_var);
}

const char * PacketDATA::getDestinationAodv() const
{
    return destinationAodv_var.c_str();
}

void PacketDATA::setDestinationAodv(const char * destinationAodv)
{
    this->destinationAodv_var = destinationAodv;
}

class PacketDATADescriptor : public cClassDescriptor
{
  public:
    PacketDATADescriptor();
    virtual ~PacketDATADescriptor();

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

Register_ClassDescriptor(PacketDATADescriptor);

PacketDATADescriptor::PacketDATADescriptor() : cClassDescriptor("PacketDATA", "RoutingPacket")
{
}

PacketDATADescriptor::~PacketDATADescriptor()
{
}

bool PacketDATADescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketDATA *>(obj)!=NULL;
}

const char *PacketDATADescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketDATADescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int PacketDATADescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *PacketDATADescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "destinationAodv",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int PacketDATADescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationAodv")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketDATADescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *PacketDATADescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int PacketDATADescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketDATA *pp = (PacketDATA *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketDATADescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketDATA *pp = (PacketDATA *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getDestinationAodv());
        default: return "";
    }
}

bool PacketDATADescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketDATA *pp = (PacketDATA *)object; (void)pp;
    switch (field) {
        case 0: pp->setDestinationAodv((value)); return true;
        default: return false;
    }
}

const char *PacketDATADescriptor::getFieldStructName(void *object, int field) const
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

void *PacketDATADescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketDATA *pp = (PacketDATA *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


