//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/application/geoSync/PTPPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "PTPPacket_m.h"

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

Register_Class(PTPPacket);

PTPPacket::PTPPacket(const char *name, int kind) : ::GeoSyncPacketType(name,kind)
{
    this->ptpType_var = 0;
    this->timeStamp_var = 0;
}

PTPPacket::PTPPacket(const PTPPacket& other) : ::GeoSyncPacketType(other)
{
    copy(other);
}

PTPPacket::~PTPPacket()
{
}

PTPPacket& PTPPacket::operator=(const PTPPacket& other)
{
    if (this==&other) return *this;
    ::GeoSyncPacketType::operator=(other);
    copy(other);
    return *this;
}

void PTPPacket::copy(const PTPPacket& other)
{
    this->ptpType_var = other.ptpType_var;
    this->timeStamp_var = other.timeStamp_var;
}

void PTPPacket::parsimPack(cCommBuffer *b)
{
    ::GeoSyncPacketType::parsimPack(b);
    doPacking(b,this->ptpType_var);
    doPacking(b,this->timeStamp_var);
}

void PTPPacket::parsimUnpack(cCommBuffer *b)
{
    ::GeoSyncPacketType::parsimUnpack(b);
    doUnpacking(b,this->ptpType_var);
    doUnpacking(b,this->timeStamp_var);
}

int PTPPacket::getPtpType() const
{
    return ptpType_var;
}

void PTPPacket::setPtpType(int ptpType)
{
    this->ptpType_var = ptpType;
}

int64 PTPPacket::getTimeStamp() const
{
    return timeStamp_var;
}

void PTPPacket::setTimeStamp(int64 timeStamp)
{
    this->timeStamp_var = timeStamp;
}

class PTPPacketDescriptor : public cClassDescriptor
{
  public:
    PTPPacketDescriptor();
    virtual ~PTPPacketDescriptor();

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

Register_ClassDescriptor(PTPPacketDescriptor);

PTPPacketDescriptor::PTPPacketDescriptor() : cClassDescriptor("PTPPacket", "GeoSyncPacketType")
{
}

PTPPacketDescriptor::~PTPPacketDescriptor()
{
}

bool PTPPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PTPPacket *>(obj)!=NULL;
}

const char *PTPPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PTPPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int PTPPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *PTPPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "ptpType",
        "timeStamp",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int PTPPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "ptpType")==0) return base+0;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeStamp")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PTPPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int64",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *PTPPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int PTPPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PTPPacket *pp = (PTPPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PTPPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PTPPacket *pp = (PTPPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPtpType());
        case 1: return int642string(pp->getTimeStamp());
        default: return "";
    }
}

bool PTPPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PTPPacket *pp = (PTPPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPtpType(string2long(value)); return true;
        case 1: pp->setTimeStamp(string2int64(value)); return true;
        default: return false;
    }
}

const char *PTPPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *PTPPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PTPPacket *pp = (PTPPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


