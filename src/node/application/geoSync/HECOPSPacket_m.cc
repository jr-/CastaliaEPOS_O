//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/application/geoSync/HECOPSPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "HECOPSPacket_m.h"

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

Register_Class(HECOPSPacket);

HECOPSPacket::HECOPSPacket(const char *name, int kind) : ::GeoSyncPacketType(name,kind)
{
    this->someField_var = 0;
}

HECOPSPacket::HECOPSPacket(const HECOPSPacket& other) : ::GeoSyncPacketType(other)
{
    copy(other);
}

HECOPSPacket::~HECOPSPacket()
{
}

HECOPSPacket& HECOPSPacket::operator=(const HECOPSPacket& other)
{
    if (this==&other) return *this;
    ::GeoSyncPacketType::operator=(other);
    copy(other);
    return *this;
}

void HECOPSPacket::copy(const HECOPSPacket& other)
{
    this->someField_var = other.someField_var;
}

void HECOPSPacket::parsimPack(cCommBuffer *b)
{
    ::GeoSyncPacketType::parsimPack(b);
    doPacking(b,this->someField_var);
}

void HECOPSPacket::parsimUnpack(cCommBuffer *b)
{
    ::GeoSyncPacketType::parsimUnpack(b);
    doUnpacking(b,this->someField_var);
}

int HECOPSPacket::getSomeField() const
{
    return someField_var;
}

void HECOPSPacket::setSomeField(int someField)
{
    this->someField_var = someField;
}

class HECOPSPacketDescriptor : public cClassDescriptor
{
  public:
    HECOPSPacketDescriptor();
    virtual ~HECOPSPacketDescriptor();

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

Register_ClassDescriptor(HECOPSPacketDescriptor);

HECOPSPacketDescriptor::HECOPSPacketDescriptor() : cClassDescriptor("HECOPSPacket", "GeoSyncPacketType")
{
}

HECOPSPacketDescriptor::~HECOPSPacketDescriptor()
{
}

bool HECOPSPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<HECOPSPacket *>(obj)!=NULL;
}

const char *HECOPSPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int HECOPSPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int HECOPSPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *HECOPSPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "someField",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int HECOPSPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "someField")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *HECOPSPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *HECOPSPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int HECOPSPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    HECOPSPacket *pp = (HECOPSPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string HECOPSPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    HECOPSPacket *pp = (HECOPSPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSomeField());
        default: return "";
    }
}

bool HECOPSPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    HECOPSPacket *pp = (HECOPSPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSomeField(string2long(value)); return true;
        default: return false;
    }
}

const char *HECOPSPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *HECOPSPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    HECOPSPacket *pp = (HECOPSPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


