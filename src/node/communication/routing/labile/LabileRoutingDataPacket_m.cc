//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/labile/LabileRoutingDataPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "LabileRoutingDataPacket_m.h"

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

Register_Class(LabileDataPacket);

LabileDataPacket::LabileDataPacket(const char *name, int kind) : ::PacketId(name,kind)
{
    this->destinationLABILE_var = 0;
}

LabileDataPacket::LabileDataPacket(const LabileDataPacket& other) : ::PacketId(other)
{
    copy(other);
}

LabileDataPacket::~LabileDataPacket()
{
}

LabileDataPacket& LabileDataPacket::operator=(const LabileDataPacket& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void LabileDataPacket::copy(const LabileDataPacket& other)
{
    this->destinationLABILE_var = other.destinationLABILE_var;
}

void LabileDataPacket::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
    doPacking(b,this->destinationLABILE_var);
}

void LabileDataPacket::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
    doUnpacking(b,this->destinationLABILE_var);
}

const char * LabileDataPacket::getDestinationLABILE() const
{
    return destinationLABILE_var.c_str();
}

void LabileDataPacket::setDestinationLABILE(const char * destinationLABILE)
{
    this->destinationLABILE_var = destinationLABILE;
}

class LabileDataPacketDescriptor : public cClassDescriptor
{
  public:
    LabileDataPacketDescriptor();
    virtual ~LabileDataPacketDescriptor();

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

Register_ClassDescriptor(LabileDataPacketDescriptor);

LabileDataPacketDescriptor::LabileDataPacketDescriptor() : cClassDescriptor("LabileDataPacket", "PacketId")
{
}

LabileDataPacketDescriptor::~LabileDataPacketDescriptor()
{
}

bool LabileDataPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<LabileDataPacket *>(obj)!=NULL;
}

const char *LabileDataPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int LabileDataPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int LabileDataPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *LabileDataPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "destinationLABILE",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int LabileDataPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationLABILE")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *LabileDataPacketDescriptor::getFieldTypeString(void *object, int field) const
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

const char *LabileDataPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int LabileDataPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    LabileDataPacket *pp = (LabileDataPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string LabileDataPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    LabileDataPacket *pp = (LabileDataPacket *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getDestinationLABILE());
        default: return "";
    }
}

bool LabileDataPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    LabileDataPacket *pp = (LabileDataPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setDestinationLABILE((value)); return true;
        default: return false;
    }
}

const char *LabileDataPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *LabileDataPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    LabileDataPacket *pp = (LabileDataPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


