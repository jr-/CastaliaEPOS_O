//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/tstp/TSTPPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "TSTPPacket_m.h"

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

Register_Class(TSTPPacket);

TSTPPacket::TSTPPacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    for (unsigned int i=0; i<127; i++)
        this->data_var[i] = 0;
}

TSTPPacket::TSTPPacket(const TSTPPacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

TSTPPacket::~TSTPPacket()
{
}

TSTPPacket& TSTPPacket::operator=(const TSTPPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void TSTPPacket::copy(const TSTPPacket& other)
{
    for (unsigned int i=0; i<127; i++)
        this->data_var[i] = other.data_var[i];
}

void TSTPPacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->data_var,127);
}

void TSTPPacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->data_var,127);
}

unsigned int TSTPPacket::getDataArraySize() const
{
    return 127;
}

char TSTPPacket::getData(unsigned int k) const
{
    if (k>=127) throw cRuntimeError("Array of size 127 indexed by %lu", (unsigned long)k);
    return data_var[k];
}

void TSTPPacket::setData(unsigned int k, char data)
{
    if (k>=127) throw cRuntimeError("Array of size 127 indexed by %lu", (unsigned long)k);
    this->data_var[k] = data;
}

class TSTPPacketDescriptor : public cClassDescriptor
{
  public:
    TSTPPacketDescriptor();
    virtual ~TSTPPacketDescriptor();

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

Register_ClassDescriptor(TSTPPacketDescriptor);

TSTPPacketDescriptor::TSTPPacketDescriptor() : cClassDescriptor("TSTPPacket", "RoutingPacket")
{
}

TSTPPacketDescriptor::~TSTPPacketDescriptor()
{
}

bool TSTPPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<TSTPPacket *>(obj)!=NULL;
}

const char *TSTPPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int TSTPPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int TSTPPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *TSTPPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "data",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int TSTPPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "data")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *TSTPPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "char",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *TSTPPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int TSTPPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    TSTPPacket *pp = (TSTPPacket *)object; (void)pp;
    switch (field) {
        case 0: return 127;
        default: return 0;
    }
}

std::string TSTPPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    TSTPPacket *pp = (TSTPPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getData(i));
        default: return "";
    }
}

bool TSTPPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    TSTPPacket *pp = (TSTPPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setData(i,string2long(value)); return true;
        default: return false;
    }
}

const char *TSTPPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *TSTPPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    TSTPPacket *pp = (TSTPPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


