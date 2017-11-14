//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/REL/RELRoutingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "RELRoutingPacket_m.h"

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

Register_Class(RELRREQPacket);

RELRREQPacket::RELRREQPacket(const char *name, int kind) : ::PacketId(name,kind)
{
    this->hopCount_var = 0;
    this->linkCost_var = 0;
    this->rreqID_var = 0;
    this->energy_var = 0;
    this->sourceREL_var = 0;
    this->sourceSeqNum_var = 0;
    this->destinationSeqNum_var = 0;
    this->destinationREL_var = 0;
}

RELRREQPacket::RELRREQPacket(const RELRREQPacket& other) : ::PacketId(other)
{
    copy(other);
}

RELRREQPacket::~RELRREQPacket()
{
}

RELRREQPacket& RELRREQPacket::operator=(const RELRREQPacket& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void RELRREQPacket::copy(const RELRREQPacket& other)
{
    this->hopCount_var = other.hopCount_var;
    this->linkCost_var = other.linkCost_var;
    this->rreqID_var = other.rreqID_var;
    this->energy_var = other.energy_var;
    this->sourceREL_var = other.sourceREL_var;
    this->sourceSeqNum_var = other.sourceSeqNum_var;
    this->destinationSeqNum_var = other.destinationSeqNum_var;
    this->destinationREL_var = other.destinationREL_var;
}

void RELRREQPacket::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
    doPacking(b,this->hopCount_var);
    doPacking(b,this->linkCost_var);
    doPacking(b,this->rreqID_var);
    doPacking(b,this->energy_var);
    doPacking(b,this->sourceREL_var);
    doPacking(b,this->sourceSeqNum_var);
    doPacking(b,this->destinationSeqNum_var);
    doPacking(b,this->destinationREL_var);
}

void RELRREQPacket::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
    doUnpacking(b,this->hopCount_var);
    doUnpacking(b,this->linkCost_var);
    doUnpacking(b,this->rreqID_var);
    doUnpacking(b,this->energy_var);
    doUnpacking(b,this->sourceREL_var);
    doUnpacking(b,this->sourceSeqNum_var);
    doUnpacking(b,this->destinationSeqNum_var);
    doUnpacking(b,this->destinationREL_var);
}

int RELRREQPacket::getHopCount() const
{
    return hopCount_var;
}

void RELRREQPacket::setHopCount(int hopCount)
{
    this->hopCount_var = hopCount;
}

int RELRREQPacket::getLinkCost() const
{
    return linkCost_var;
}

void RELRREQPacket::setLinkCost(int linkCost)
{
    this->linkCost_var = linkCost;
}

int RELRREQPacket::getRreqID() const
{
    return rreqID_var;
}

void RELRREQPacket::setRreqID(int rreqID)
{
    this->rreqID_var = rreqID;
}

int RELRREQPacket::getEnergy() const
{
    return energy_var;
}

void RELRREQPacket::setEnergy(int energy)
{
    this->energy_var = energy;
}

const char * RELRREQPacket::getSourceREL() const
{
    return sourceREL_var.c_str();
}

void RELRREQPacket::setSourceREL(const char * sourceREL)
{
    this->sourceREL_var = sourceREL;
}

unsigned long RELRREQPacket::getSourceSeqNum() const
{
    return sourceSeqNum_var;
}

void RELRREQPacket::setSourceSeqNum(unsigned long sourceSeqNum)
{
    this->sourceSeqNum_var = sourceSeqNum;
}

unsigned long RELRREQPacket::getDestinationSeqNum() const
{
    return destinationSeqNum_var;
}

void RELRREQPacket::setDestinationSeqNum(unsigned long destinationSeqNum)
{
    this->destinationSeqNum_var = destinationSeqNum;
}

const char * RELRREQPacket::getDestinationREL() const
{
    return destinationREL_var.c_str();
}

void RELRREQPacket::setDestinationREL(const char * destinationREL)
{
    this->destinationREL_var = destinationREL;
}

class RELRREQPacketDescriptor : public cClassDescriptor
{
  public:
    RELRREQPacketDescriptor();
    virtual ~RELRREQPacketDescriptor();

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

Register_ClassDescriptor(RELRREQPacketDescriptor);

RELRREQPacketDescriptor::RELRREQPacketDescriptor() : cClassDescriptor("RELRREQPacket", "PacketId")
{
}

RELRREQPacketDescriptor::~RELRREQPacketDescriptor()
{
}

bool RELRREQPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<RELRREQPacket *>(obj)!=NULL;
}

const char *RELRREQPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int RELRREQPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount(object) : 8;
}

unsigned int RELRREQPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *RELRREQPacketDescriptor::getFieldName(void *object, int field) const
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
        "rreqID",
        "energy",
        "sourceREL",
        "sourceSeqNum",
        "destinationSeqNum",
        "destinationREL",
    };
    return (field>=0 && field<8) ? fieldNames[field] : NULL;
}

int RELRREQPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+0;
    if (fieldName[0]=='l' && strcmp(fieldName, "linkCost")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "rreqID")==0) return base+2;
    if (fieldName[0]=='e' && strcmp(fieldName, "energy")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceREL")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceSeqNum")==0) return base+5;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationSeqNum")==0) return base+6;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationREL")==0) return base+7;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *RELRREQPacketDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
        "string",
        "unsigned long",
        "unsigned long",
        "string",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : NULL;
}

const char *RELRREQPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int RELRREQPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    RELRREQPacket *pp = (RELRREQPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string RELRREQPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    RELRREQPacket *pp = (RELRREQPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getHopCount());
        case 1: return long2string(pp->getLinkCost());
        case 2: return long2string(pp->getRreqID());
        case 3: return long2string(pp->getEnergy());
        case 4: return oppstring2string(pp->getSourceREL());
        case 5: return ulong2string(pp->getSourceSeqNum());
        case 6: return ulong2string(pp->getDestinationSeqNum());
        case 7: return oppstring2string(pp->getDestinationREL());
        default: return "";
    }
}

bool RELRREQPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    RELRREQPacket *pp = (RELRREQPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setHopCount(string2long(value)); return true;
        case 1: pp->setLinkCost(string2long(value)); return true;
        case 2: pp->setRreqID(string2long(value)); return true;
        case 3: pp->setEnergy(string2long(value)); return true;
        case 4: pp->setSourceREL((value)); return true;
        case 5: pp->setSourceSeqNum(string2ulong(value)); return true;
        case 6: pp->setDestinationSeqNum(string2ulong(value)); return true;
        case 7: pp->setDestinationREL((value)); return true;
        default: return false;
    }
}

const char *RELRREQPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *RELRREQPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    RELRREQPacket *pp = (RELRREQPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(RELHelloMessage);

RELHelloMessage::RELHelloMessage(const char *name, int kind) : ::PacketId(name,kind)
{
}

RELHelloMessage::RELHelloMessage(const RELHelloMessage& other) : ::PacketId(other)
{
    copy(other);
}

RELHelloMessage::~RELHelloMessage()
{
}

RELHelloMessage& RELHelloMessage::operator=(const RELHelloMessage& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void RELHelloMessage::copy(const RELHelloMessage& other)
{
}

void RELHelloMessage::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
}

void RELHelloMessage::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
}

class RELHelloMessageDescriptor : public cClassDescriptor
{
  public:
    RELHelloMessageDescriptor();
    virtual ~RELHelloMessageDescriptor();

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

Register_ClassDescriptor(RELHelloMessageDescriptor);

RELHelloMessageDescriptor::RELHelloMessageDescriptor() : cClassDescriptor("RELHelloMessage", "PacketId")
{
}

RELHelloMessageDescriptor::~RELHelloMessageDescriptor()
{
}

bool RELHelloMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<RELHelloMessage *>(obj)!=NULL;
}

const char *RELHelloMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int RELHelloMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int RELHelloMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *RELHelloMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int RELHelloMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *RELHelloMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *RELHelloMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int RELHelloMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    RELHelloMessage *pp = (RELHelloMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string RELHelloMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    RELHelloMessage *pp = (RELHelloMessage *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool RELHelloMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    RELHelloMessage *pp = (RELHelloMessage *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *RELHelloMessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *RELHelloMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    RELHelloMessage *pp = (RELHelloMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(RELRREPAckPacket);

RELRREPAckPacket::RELRREPAckPacket(const char *name, int kind) : ::PacketId(name,kind)
{
}

RELRREPAckPacket::RELRREPAckPacket(const RELRREPAckPacket& other) : ::PacketId(other)
{
    copy(other);
}

RELRREPAckPacket::~RELRREPAckPacket()
{
}

RELRREPAckPacket& RELRREPAckPacket::operator=(const RELRREPAckPacket& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void RELRREPAckPacket::copy(const RELRREPAckPacket& other)
{
}

void RELRREPAckPacket::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
}

void RELRREPAckPacket::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
}

class RELRREPAckPacketDescriptor : public cClassDescriptor
{
  public:
    RELRREPAckPacketDescriptor();
    virtual ~RELRREPAckPacketDescriptor();

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

Register_ClassDescriptor(RELRREPAckPacketDescriptor);

RELRREPAckPacketDescriptor::RELRREPAckPacketDescriptor() : cClassDescriptor("RELRREPAckPacket", "PacketId")
{
}

RELRREPAckPacketDescriptor::~RELRREPAckPacketDescriptor()
{
}

bool RELRREPAckPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<RELRREPAckPacket *>(obj)!=NULL;
}

const char *RELRREPAckPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int RELRREPAckPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int RELRREPAckPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *RELRREPAckPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int RELRREPAckPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *RELRREPAckPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *RELRREPAckPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int RELRREPAckPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    RELRREPAckPacket *pp = (RELRREPAckPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string RELRREPAckPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    RELRREPAckPacket *pp = (RELRREPAckPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool RELRREPAckPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    RELRREPAckPacket *pp = (RELRREPAckPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *RELRREPAckPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *RELRREPAckPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    RELRREPAckPacket *pp = (RELRREPAckPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(RELRERRPacket);

RELRERRPacket::RELRERRPacket(const char *name, int kind) : ::PacketId(name,kind)
{
    this->destCount_var = 0;
    unreachableDestAddr_arraysize = 0;
    this->unreachableDestAddr_var = 0;
    unreachableDestSeqNum_arraysize = 0;
    this->unreachableDestSeqNum_var = 0;
}

RELRERRPacket::RELRERRPacket(const RELRERRPacket& other) : ::PacketId(other)
{
    unreachableDestAddr_arraysize = 0;
    this->unreachableDestAddr_var = 0;
    unreachableDestSeqNum_arraysize = 0;
    this->unreachableDestSeqNum_var = 0;
    copy(other);
}

RELRERRPacket::~RELRERRPacket()
{
    delete [] unreachableDestAddr_var;
    delete [] unreachableDestSeqNum_var;
}

RELRERRPacket& RELRERRPacket::operator=(const RELRERRPacket& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void RELRERRPacket::copy(const RELRERRPacket& other)
{
    this->destCount_var = other.destCount_var;
    delete [] this->unreachableDestAddr_var;
    this->unreachableDestAddr_var = (other.unreachableDestAddr_arraysize==0) ? NULL : new opp_string[other.unreachableDestAddr_arraysize];
    unreachableDestAddr_arraysize = other.unreachableDestAddr_arraysize;
    for (unsigned int i=0; i<unreachableDestAddr_arraysize; i++)
        this->unreachableDestAddr_var[i] = other.unreachableDestAddr_var[i];
    delete [] this->unreachableDestSeqNum_var;
    this->unreachableDestSeqNum_var = (other.unreachableDestSeqNum_arraysize==0) ? NULL : new unsigned long[other.unreachableDestSeqNum_arraysize];
    unreachableDestSeqNum_arraysize = other.unreachableDestSeqNum_arraysize;
    for (unsigned int i=0; i<unreachableDestSeqNum_arraysize; i++)
        this->unreachableDestSeqNum_var[i] = other.unreachableDestSeqNum_var[i];
}

void RELRERRPacket::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
    doPacking(b,this->destCount_var);
    b->pack(unreachableDestAddr_arraysize);
    doPacking(b,this->unreachableDestAddr_var,unreachableDestAddr_arraysize);
    b->pack(unreachableDestSeqNum_arraysize);
    doPacking(b,this->unreachableDestSeqNum_var,unreachableDestSeqNum_arraysize);
}

void RELRERRPacket::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
    doUnpacking(b,this->destCount_var);
    delete [] this->unreachableDestAddr_var;
    b->unpack(unreachableDestAddr_arraysize);
    if (unreachableDestAddr_arraysize==0) {
        this->unreachableDestAddr_var = 0;
    } else {
        this->unreachableDestAddr_var = new opp_string[unreachableDestAddr_arraysize];
        doUnpacking(b,this->unreachableDestAddr_var,unreachableDestAddr_arraysize);
    }
    delete [] this->unreachableDestSeqNum_var;
    b->unpack(unreachableDestSeqNum_arraysize);
    if (unreachableDestSeqNum_arraysize==0) {
        this->unreachableDestSeqNum_var = 0;
    } else {
        this->unreachableDestSeqNum_var = new unsigned long[unreachableDestSeqNum_arraysize];
        doUnpacking(b,this->unreachableDestSeqNum_var,unreachableDestSeqNum_arraysize);
    }
}

int RELRERRPacket::getDestCount() const
{
    return destCount_var;
}

void RELRERRPacket::setDestCount(int destCount)
{
    this->destCount_var = destCount;
}

void RELRERRPacket::setUnreachableDestAddrArraySize(unsigned int size)
{
    opp_string *unreachableDestAddr_var2 = (size==0) ? NULL : new opp_string[size];
    unsigned int sz = unreachableDestAddr_arraysize < size ? unreachableDestAddr_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        unreachableDestAddr_var2[i] = this->unreachableDestAddr_var[i];
    for (unsigned int i=sz; i<size; i++)
        unreachableDestAddr_var2[i] = 0;
    unreachableDestAddr_arraysize = size;
    delete [] this->unreachableDestAddr_var;
    this->unreachableDestAddr_var = unreachableDestAddr_var2;
}

unsigned int RELRERRPacket::getUnreachableDestAddrArraySize() const
{
    return unreachableDestAddr_arraysize;
}

const char * RELRERRPacket::getUnreachableDestAddr(unsigned int k) const
{
    if (k>=unreachableDestAddr_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDestAddr_arraysize, k);
    return unreachableDestAddr_var[k].c_str();
}

void RELRERRPacket::setUnreachableDestAddr(unsigned int k, const char * unreachableDestAddr)
{
    if (k>=unreachableDestAddr_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDestAddr_arraysize, k);
    this->unreachableDestAddr_var[k] = unreachableDestAddr;
}

void RELRERRPacket::setUnreachableDestSeqNumArraySize(unsigned int size)
{
    unsigned long *unreachableDestSeqNum_var2 = (size==0) ? NULL : new unsigned long[size];
    unsigned int sz = unreachableDestSeqNum_arraysize < size ? unreachableDestSeqNum_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        unreachableDestSeqNum_var2[i] = this->unreachableDestSeqNum_var[i];
    for (unsigned int i=sz; i<size; i++)
        unreachableDestSeqNum_var2[i] = 0;
    unreachableDestSeqNum_arraysize = size;
    delete [] this->unreachableDestSeqNum_var;
    this->unreachableDestSeqNum_var = unreachableDestSeqNum_var2;
}

unsigned int RELRERRPacket::getUnreachableDestSeqNumArraySize() const
{
    return unreachableDestSeqNum_arraysize;
}

unsigned long RELRERRPacket::getUnreachableDestSeqNum(unsigned int k) const
{
    if (k>=unreachableDestSeqNum_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDestSeqNum_arraysize, k);
    return unreachableDestSeqNum_var[k];
}

void RELRERRPacket::setUnreachableDestSeqNum(unsigned int k, unsigned long unreachableDestSeqNum)
{
    if (k>=unreachableDestSeqNum_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDestSeqNum_arraysize, k);
    this->unreachableDestSeqNum_var[k] = unreachableDestSeqNum;
}

class RELRERRPacketDescriptor : public cClassDescriptor
{
  public:
    RELRERRPacketDescriptor();
    virtual ~RELRERRPacketDescriptor();

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

Register_ClassDescriptor(RELRERRPacketDescriptor);

RELRERRPacketDescriptor::RELRERRPacketDescriptor() : cClassDescriptor("RELRERRPacket", "PacketId")
{
}

RELRERRPacketDescriptor::~RELRERRPacketDescriptor()
{
}

bool RELRERRPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<RELRERRPacket *>(obj)!=NULL;
}

const char *RELRERRPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int RELRERRPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int RELRERRPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *RELRERRPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "destCount",
        "unreachableDestAddr",
        "unreachableDestSeqNum",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int RELRERRPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destCount")==0) return base+0;
    if (fieldName[0]=='u' && strcmp(fieldName, "unreachableDestAddr")==0) return base+1;
    if (fieldName[0]=='u' && strcmp(fieldName, "unreachableDestSeqNum")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *RELRERRPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "string",
        "unsigned long",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *RELRERRPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int RELRERRPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    RELRERRPacket *pp = (RELRERRPacket *)object; (void)pp;
    switch (field) {
        case 1: return pp->getUnreachableDestAddrArraySize();
        case 2: return pp->getUnreachableDestSeqNumArraySize();
        default: return 0;
    }
}

std::string RELRERRPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    RELRERRPacket *pp = (RELRERRPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getDestCount());
        case 1: return oppstring2string(pp->getUnreachableDestAddr(i));
        case 2: return ulong2string(pp->getUnreachableDestSeqNum(i));
        default: return "";
    }
}

bool RELRERRPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    RELRERRPacket *pp = (RELRERRPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setDestCount(string2long(value)); return true;
        case 1: pp->setUnreachableDestAddr(i,(value)); return true;
        case 2: pp->setUnreachableDestSeqNum(i,string2ulong(value)); return true;
        default: return false;
    }
}

const char *RELRERRPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *RELRERRPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    RELRERRPacket *pp = (RELRERRPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(RELRADVPacket);

RELRADVPacket::RELRADVPacket(const char *name, int kind) : ::PacketId(name,kind)
{
    this->energy_var = 0;
    this->originator_var = 0;
}

RELRADVPacket::RELRADVPacket(const RELRADVPacket& other) : ::PacketId(other)
{
    copy(other);
}

RELRADVPacket::~RELRADVPacket()
{
}

RELRADVPacket& RELRADVPacket::operator=(const RELRADVPacket& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void RELRADVPacket::copy(const RELRADVPacket& other)
{
    this->energy_var = other.energy_var;
    this->originator_var = other.originator_var;
}

void RELRADVPacket::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
    doPacking(b,this->energy_var);
    doPacking(b,this->originator_var);
}

void RELRADVPacket::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
    doUnpacking(b,this->energy_var);
    doUnpacking(b,this->originator_var);
}

int RELRADVPacket::getEnergy() const
{
    return energy_var;
}

void RELRADVPacket::setEnergy(int energy)
{
    this->energy_var = energy;
}

const char * RELRADVPacket::getOriginator() const
{
    return originator_var.c_str();
}

void RELRADVPacket::setOriginator(const char * originator)
{
    this->originator_var = originator;
}

class RELRADVPacketDescriptor : public cClassDescriptor
{
  public:
    RELRADVPacketDescriptor();
    virtual ~RELRADVPacketDescriptor();

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

Register_ClassDescriptor(RELRADVPacketDescriptor);

RELRADVPacketDescriptor::RELRADVPacketDescriptor() : cClassDescriptor("RELRADVPacket", "PacketId")
{
}

RELRADVPacketDescriptor::~RELRADVPacketDescriptor()
{
}

bool RELRADVPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<RELRADVPacket *>(obj)!=NULL;
}

const char *RELRADVPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int RELRADVPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int RELRADVPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *RELRADVPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "energy",
        "originator",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int RELRADVPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='e' && strcmp(fieldName, "energy")==0) return base+0;
    if (fieldName[0]=='o' && strcmp(fieldName, "originator")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *RELRADVPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "string",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *RELRADVPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int RELRADVPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    RELRADVPacket *pp = (RELRADVPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string RELRADVPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    RELRADVPacket *pp = (RELRADVPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getEnergy());
        case 1: return oppstring2string(pp->getOriginator());
        default: return "";
    }
}

bool RELRADVPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    RELRADVPacket *pp = (RELRADVPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setEnergy(string2long(value)); return true;
        case 1: pp->setOriginator((value)); return true;
        default: return false;
    }
}

const char *RELRADVPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *RELRADVPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    RELRADVPacket *pp = (RELRADVPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


