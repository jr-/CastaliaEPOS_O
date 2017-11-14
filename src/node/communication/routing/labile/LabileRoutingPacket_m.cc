//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/labile/LabileRoutingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "LabileRoutingPacket_m.h"

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

Register_Class(LabileRREQPacket);

LabileRREQPacket::LabileRREQPacket(const char *name, int kind) : ::PacketId(name,kind)
{
    this->flagJ_var = 0;
    this->flagR_var = 0;
    this->flagG_var = 0;
    this->flagD_var = 0;
    this->flagU_var = 0;
    this->hopCount_var = 0;
    this->weakLinks_var = 0;
    this->rreqID_var = 0;
    this->sourceLABILE_var = 0;
    this->sourceSeqNum_var = 0;
    this->destinationSeqNum_var = 0;
    this->destinationLABILE_var = 0;
}

LabileRREQPacket::LabileRREQPacket(const LabileRREQPacket& other) : ::PacketId(other)
{
    copy(other);
}

LabileRREQPacket::~LabileRREQPacket()
{
}

LabileRREQPacket& LabileRREQPacket::operator=(const LabileRREQPacket& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void LabileRREQPacket::copy(const LabileRREQPacket& other)
{
    this->flagJ_var = other.flagJ_var;
    this->flagR_var = other.flagR_var;
    this->flagG_var = other.flagG_var;
    this->flagD_var = other.flagD_var;
    this->flagU_var = other.flagU_var;
    this->hopCount_var = other.hopCount_var;
    this->weakLinks_var = other.weakLinks_var;
    this->rreqID_var = other.rreqID_var;
    this->sourceLABILE_var = other.sourceLABILE_var;
    this->sourceSeqNum_var = other.sourceSeqNum_var;
    this->destinationSeqNum_var = other.destinationSeqNum_var;
    this->destinationLABILE_var = other.destinationLABILE_var;
}

void LabileRREQPacket::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
    doPacking(b,this->flagJ_var);
    doPacking(b,this->flagR_var);
    doPacking(b,this->flagG_var);
    doPacking(b,this->flagD_var);
    doPacking(b,this->flagU_var);
    doPacking(b,this->hopCount_var);
    doPacking(b,this->weakLinks_var);
    doPacking(b,this->rreqID_var);
    doPacking(b,this->sourceLABILE_var);
    doPacking(b,this->sourceSeqNum_var);
    doPacking(b,this->destinationSeqNum_var);
    doPacking(b,this->destinationLABILE_var);
}

void LabileRREQPacket::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
    doUnpacking(b,this->flagJ_var);
    doUnpacking(b,this->flagR_var);
    doUnpacking(b,this->flagG_var);
    doUnpacking(b,this->flagD_var);
    doUnpacking(b,this->flagU_var);
    doUnpacking(b,this->hopCount_var);
    doUnpacking(b,this->weakLinks_var);
    doUnpacking(b,this->rreqID_var);
    doUnpacking(b,this->sourceLABILE_var);
    doUnpacking(b,this->sourceSeqNum_var);
    doUnpacking(b,this->destinationSeqNum_var);
    doUnpacking(b,this->destinationLABILE_var);
}

bool LabileRREQPacket::getFlagJ() const
{
    return flagJ_var;
}

void LabileRREQPacket::setFlagJ(bool flagJ)
{
    this->flagJ_var = flagJ;
}

bool LabileRREQPacket::getFlagR() const
{
    return flagR_var;
}

void LabileRREQPacket::setFlagR(bool flagR)
{
    this->flagR_var = flagR;
}

bool LabileRREQPacket::getFlagG() const
{
    return flagG_var;
}

void LabileRREQPacket::setFlagG(bool flagG)
{
    this->flagG_var = flagG;
}

bool LabileRREQPacket::getFlagD() const
{
    return flagD_var;
}

void LabileRREQPacket::setFlagD(bool flagD)
{
    this->flagD_var = flagD;
}

bool LabileRREQPacket::getFlagU() const
{
    return flagU_var;
}

void LabileRREQPacket::setFlagU(bool flagU)
{
    this->flagU_var = flagU;
}

int LabileRREQPacket::getHopCount() const
{
    return hopCount_var;
}

void LabileRREQPacket::setHopCount(int hopCount)
{
    this->hopCount_var = hopCount;
}

int LabileRREQPacket::getWeakLinks() const
{
    return weakLinks_var;
}

void LabileRREQPacket::setWeakLinks(int weakLinks)
{
    this->weakLinks_var = weakLinks;
}

int LabileRREQPacket::getRreqID() const
{
    return rreqID_var;
}

void LabileRREQPacket::setRreqID(int rreqID)
{
    this->rreqID_var = rreqID;
}

const char * LabileRREQPacket::getSourceLABILE() const
{
    return sourceLABILE_var.c_str();
}

void LabileRREQPacket::setSourceLABILE(const char * sourceLABILE)
{
    this->sourceLABILE_var = sourceLABILE;
}

unsigned long LabileRREQPacket::getSourceSeqNum() const
{
    return sourceSeqNum_var;
}

void LabileRREQPacket::setSourceSeqNum(unsigned long sourceSeqNum)
{
    this->sourceSeqNum_var = sourceSeqNum;
}

unsigned long LabileRREQPacket::getDestinationSeqNum() const
{
    return destinationSeqNum_var;
}

void LabileRREQPacket::setDestinationSeqNum(unsigned long destinationSeqNum)
{
    this->destinationSeqNum_var = destinationSeqNum;
}

const char * LabileRREQPacket::getDestinationLABILE() const
{
    return destinationLABILE_var.c_str();
}

void LabileRREQPacket::setDestinationLABILE(const char * destinationLABILE)
{
    this->destinationLABILE_var = destinationLABILE;
}

class LabileRREQPacketDescriptor : public cClassDescriptor
{
  public:
    LabileRREQPacketDescriptor();
    virtual ~LabileRREQPacketDescriptor();

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

Register_ClassDescriptor(LabileRREQPacketDescriptor);

LabileRREQPacketDescriptor::LabileRREQPacketDescriptor() : cClassDescriptor("LabileRREQPacket", "PacketId")
{
}

LabileRREQPacketDescriptor::~LabileRREQPacketDescriptor()
{
}

bool LabileRREQPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<LabileRREQPacket *>(obj)!=NULL;
}

const char *LabileRREQPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int LabileRREQPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 12+basedesc->getFieldCount(object) : 12;
}

unsigned int LabileRREQPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *LabileRREQPacketDescriptor::getFieldName(void *object, int field) const
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
        "weakLinks",
        "rreqID",
        "sourceLABILE",
        "sourceSeqNum",
        "destinationSeqNum",
        "destinationLABILE",
    };
    return (field>=0 && field<12) ? fieldNames[field] : NULL;
}

int LabileRREQPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagJ")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagR")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagG")==0) return base+2;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagD")==0) return base+3;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagU")==0) return base+4;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+5;
    if (fieldName[0]=='w' && strcmp(fieldName, "weakLinks")==0) return base+6;
    if (fieldName[0]=='r' && strcmp(fieldName, "rreqID")==0) return base+7;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceLABILE")==0) return base+8;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceSeqNum")==0) return base+9;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationSeqNum")==0) return base+10;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationLABILE")==0) return base+11;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *LabileRREQPacketDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
        "string",
        "unsigned long",
        "unsigned long",
        "string",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : NULL;
}

const char *LabileRREQPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int LabileRREQPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREQPacket *pp = (LabileRREQPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string LabileRREQPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREQPacket *pp = (LabileRREQPacket *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getFlagJ());
        case 1: return bool2string(pp->getFlagR());
        case 2: return bool2string(pp->getFlagG());
        case 3: return bool2string(pp->getFlagD());
        case 4: return bool2string(pp->getFlagU());
        case 5: return long2string(pp->getHopCount());
        case 6: return long2string(pp->getWeakLinks());
        case 7: return long2string(pp->getRreqID());
        case 8: return oppstring2string(pp->getSourceLABILE());
        case 9: return ulong2string(pp->getSourceSeqNum());
        case 10: return ulong2string(pp->getDestinationSeqNum());
        case 11: return oppstring2string(pp->getDestinationLABILE());
        default: return "";
    }
}

bool LabileRREQPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREQPacket *pp = (LabileRREQPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setFlagJ(string2bool(value)); return true;
        case 1: pp->setFlagR(string2bool(value)); return true;
        case 2: pp->setFlagG(string2bool(value)); return true;
        case 3: pp->setFlagD(string2bool(value)); return true;
        case 4: pp->setFlagU(string2bool(value)); return true;
        case 5: pp->setHopCount(string2long(value)); return true;
        case 6: pp->setWeakLinks(string2long(value)); return true;
        case 7: pp->setRreqID(string2long(value)); return true;
        case 8: pp->setSourceLABILE((value)); return true;
        case 9: pp->setSourceSeqNum(string2ulong(value)); return true;
        case 10: pp->setDestinationSeqNum(string2ulong(value)); return true;
        case 11: pp->setDestinationLABILE((value)); return true;
        default: return false;
    }
}

const char *LabileRREQPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *LabileRREQPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREQPacket *pp = (LabileRREQPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(LabileHelloMessage);

LabileHelloMessage::LabileHelloMessage(const char *name, int kind) : ::PacketId(name,kind)
{
}

LabileHelloMessage::LabileHelloMessage(const LabileHelloMessage& other) : ::PacketId(other)
{
    copy(other);
}

LabileHelloMessage::~LabileHelloMessage()
{
}

LabileHelloMessage& LabileHelloMessage::operator=(const LabileHelloMessage& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void LabileHelloMessage::copy(const LabileHelloMessage& other)
{
}

void LabileHelloMessage::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
}

void LabileHelloMessage::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
}

class LabileHelloMessageDescriptor : public cClassDescriptor
{
  public:
    LabileHelloMessageDescriptor();
    virtual ~LabileHelloMessageDescriptor();

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

Register_ClassDescriptor(LabileHelloMessageDescriptor);

LabileHelloMessageDescriptor::LabileHelloMessageDescriptor() : cClassDescriptor("LabileHelloMessage", "PacketId")
{
}

LabileHelloMessageDescriptor::~LabileHelloMessageDescriptor()
{
}

bool LabileHelloMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<LabileHelloMessage *>(obj)!=NULL;
}

const char *LabileHelloMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int LabileHelloMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int LabileHelloMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *LabileHelloMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int LabileHelloMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *LabileHelloMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *LabileHelloMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int LabileHelloMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    LabileHelloMessage *pp = (LabileHelloMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string LabileHelloMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    LabileHelloMessage *pp = (LabileHelloMessage *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool LabileHelloMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    LabileHelloMessage *pp = (LabileHelloMessage *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *LabileHelloMessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *LabileHelloMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    LabileHelloMessage *pp = (LabileHelloMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(LabileRREPAckPacket);

LabileRREPAckPacket::LabileRREPAckPacket(const char *name, int kind) : ::PacketId(name,kind)
{
}

LabileRREPAckPacket::LabileRREPAckPacket(const LabileRREPAckPacket& other) : ::PacketId(other)
{
    copy(other);
}

LabileRREPAckPacket::~LabileRREPAckPacket()
{
}

LabileRREPAckPacket& LabileRREPAckPacket::operator=(const LabileRREPAckPacket& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void LabileRREPAckPacket::copy(const LabileRREPAckPacket& other)
{
}

void LabileRREPAckPacket::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
}

void LabileRREPAckPacket::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
}

class LabileRREPAckPacketDescriptor : public cClassDescriptor
{
  public:
    LabileRREPAckPacketDescriptor();
    virtual ~LabileRREPAckPacketDescriptor();

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

Register_ClassDescriptor(LabileRREPAckPacketDescriptor);

LabileRREPAckPacketDescriptor::LabileRREPAckPacketDescriptor() : cClassDescriptor("LabileRREPAckPacket", "PacketId")
{
}

LabileRREPAckPacketDescriptor::~LabileRREPAckPacketDescriptor()
{
}

bool LabileRREPAckPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<LabileRREPAckPacket *>(obj)!=NULL;
}

const char *LabileRREPAckPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int LabileRREPAckPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int LabileRREPAckPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *LabileRREPAckPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int LabileRREPAckPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *LabileRREPAckPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *LabileRREPAckPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int LabileRREPAckPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREPAckPacket *pp = (LabileRREPAckPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string LabileRREPAckPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREPAckPacket *pp = (LabileRREPAckPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool LabileRREPAckPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREPAckPacket *pp = (LabileRREPAckPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *LabileRREPAckPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *LabileRREPAckPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    LabileRREPAckPacket *pp = (LabileRREPAckPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(LabileRERRPacket);

LabileRERRPacket::LabileRERRPacket(const char *name, int kind) : ::PacketId(name,kind)
{
    this->flagN_var = 0;
    this->destCount_var = 0;
    unreachableDestAddr_arraysize = 0;
    this->unreachableDestAddr_var = 0;
    unreachableDestSeqNum_arraysize = 0;
    this->unreachableDestSeqNum_var = 0;
}

LabileRERRPacket::LabileRERRPacket(const LabileRERRPacket& other) : ::PacketId(other)
{
    unreachableDestAddr_arraysize = 0;
    this->unreachableDestAddr_var = 0;
    unreachableDestSeqNum_arraysize = 0;
    this->unreachableDestSeqNum_var = 0;
    copy(other);
}

LabileRERRPacket::~LabileRERRPacket()
{
    delete [] unreachableDestAddr_var;
    delete [] unreachableDestSeqNum_var;
}

LabileRERRPacket& LabileRERRPacket::operator=(const LabileRERRPacket& other)
{
    if (this==&other) return *this;
    ::PacketId::operator=(other);
    copy(other);
    return *this;
}

void LabileRERRPacket::copy(const LabileRERRPacket& other)
{
    this->flagN_var = other.flagN_var;
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

void LabileRERRPacket::parsimPack(cCommBuffer *b)
{
    ::PacketId::parsimPack(b);
    doPacking(b,this->flagN_var);
    doPacking(b,this->destCount_var);
    b->pack(unreachableDestAddr_arraysize);
    doPacking(b,this->unreachableDestAddr_var,unreachableDestAddr_arraysize);
    b->pack(unreachableDestSeqNum_arraysize);
    doPacking(b,this->unreachableDestSeqNum_var,unreachableDestSeqNum_arraysize);
}

void LabileRERRPacket::parsimUnpack(cCommBuffer *b)
{
    ::PacketId::parsimUnpack(b);
    doUnpacking(b,this->flagN_var);
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

bool LabileRERRPacket::getFlagN() const
{
    return flagN_var;
}

void LabileRERRPacket::setFlagN(bool flagN)
{
    this->flagN_var = flagN;
}

int LabileRERRPacket::getDestCount() const
{
    return destCount_var;
}

void LabileRERRPacket::setDestCount(int destCount)
{
    this->destCount_var = destCount;
}

void LabileRERRPacket::setUnreachableDestAddrArraySize(unsigned int size)
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

unsigned int LabileRERRPacket::getUnreachableDestAddrArraySize() const
{
    return unreachableDestAddr_arraysize;
}

const char * LabileRERRPacket::getUnreachableDestAddr(unsigned int k) const
{
    if (k>=unreachableDestAddr_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDestAddr_arraysize, k);
    return unreachableDestAddr_var[k].c_str();
}

void LabileRERRPacket::setUnreachableDestAddr(unsigned int k, const char * unreachableDestAddr)
{
    if (k>=unreachableDestAddr_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDestAddr_arraysize, k);
    this->unreachableDestAddr_var[k] = unreachableDestAddr;
}

void LabileRERRPacket::setUnreachableDestSeqNumArraySize(unsigned int size)
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

unsigned int LabileRERRPacket::getUnreachableDestSeqNumArraySize() const
{
    return unreachableDestSeqNum_arraysize;
}

unsigned long LabileRERRPacket::getUnreachableDestSeqNum(unsigned int k) const
{
    if (k>=unreachableDestSeqNum_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDestSeqNum_arraysize, k);
    return unreachableDestSeqNum_var[k];
}

void LabileRERRPacket::setUnreachableDestSeqNum(unsigned int k, unsigned long unreachableDestSeqNum)
{
    if (k>=unreachableDestSeqNum_arraysize) throw cRuntimeError("Array of size %d indexed by %d", unreachableDestSeqNum_arraysize, k);
    this->unreachableDestSeqNum_var[k] = unreachableDestSeqNum;
}

class LabileRERRPacketDescriptor : public cClassDescriptor
{
  public:
    LabileRERRPacketDescriptor();
    virtual ~LabileRERRPacketDescriptor();

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

Register_ClassDescriptor(LabileRERRPacketDescriptor);

LabileRERRPacketDescriptor::LabileRERRPacketDescriptor() : cClassDescriptor("LabileRERRPacket", "PacketId")
{
}

LabileRERRPacketDescriptor::~LabileRERRPacketDescriptor()
{
}

bool LabileRERRPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<LabileRERRPacket *>(obj)!=NULL;
}

const char *LabileRERRPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int LabileRERRPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int LabileRERRPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *LabileRERRPacketDescriptor::getFieldName(void *object, int field) const
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
        "unreachableDestAddr",
        "unreachableDestSeqNum",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int LabileRERRPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flagN")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destCount")==0) return base+1;
    if (fieldName[0]=='u' && strcmp(fieldName, "unreachableDestAddr")==0) return base+2;
    if (fieldName[0]=='u' && strcmp(fieldName, "unreachableDestSeqNum")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *LabileRERRPacketDescriptor::getFieldTypeString(void *object, int field) const
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
        "string",
        "unsigned long",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *LabileRERRPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int LabileRERRPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    LabileRERRPacket *pp = (LabileRERRPacket *)object; (void)pp;
    switch (field) {
        case 2: return pp->getUnreachableDestAddrArraySize();
        case 3: return pp->getUnreachableDestSeqNumArraySize();
        default: return 0;
    }
}

std::string LabileRERRPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    LabileRERRPacket *pp = (LabileRERRPacket *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getFlagN());
        case 1: return long2string(pp->getDestCount());
        case 2: return oppstring2string(pp->getUnreachableDestAddr(i));
        case 3: return ulong2string(pp->getUnreachableDestSeqNum(i));
        default: return "";
    }
}

bool LabileRERRPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    LabileRERRPacket *pp = (LabileRERRPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setFlagN(string2bool(value)); return true;
        case 1: pp->setDestCount(string2long(value)); return true;
        case 2: pp->setUnreachableDestAddr(i,(value)); return true;
        case 3: pp->setUnreachableDestSeqNum(i,string2ulong(value)); return true;
        default: return false;
    }
}

const char *LabileRERRPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *LabileRERRPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    LabileRERRPacket *pp = (LabileRERRPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


