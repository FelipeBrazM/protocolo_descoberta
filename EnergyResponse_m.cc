//
// Generated file, do not edit! Created by opp_msgtool 6.3 from EnergyResponse.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "EnergyResponse_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace veins {

Register_Class(EnergyResponse)

EnergyResponse::EnergyResponse(const char *name, short kind) : ::veins::BaseFrame1609_4(name, kind)
{
}

EnergyResponse::EnergyResponse(const EnergyResponse& other) : ::veins::BaseFrame1609_4(other)
{
    copy(other);
}

EnergyResponse::~EnergyResponse()
{
}

EnergyResponse& EnergyResponse::operator=(const EnergyResponse& other)
{
    if (this == &other) return *this;
    ::veins::BaseFrame1609_4::operator=(other);
    copy(other);
    return *this;
}

void EnergyResponse::copy(const EnergyResponse& other)
{
    this->requestId = other.requestId;
    this->originAddress = other.originAddress;
    this->responderAddress = other.responderAddress;
    this->nextHopAddress = other.nextHopAddress;
    this->hopCount = other.hopCount;
    this->energyAvailable = other.energyAvailable;
    this->batteryLevel = other.batteryLevel;
}

void EnergyResponse::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::veins::BaseFrame1609_4::parsimPack(b);
    doParsimPacking(b,this->requestId);
    doParsimPacking(b,this->originAddress);
    doParsimPacking(b,this->responderAddress);
    doParsimPacking(b,this->nextHopAddress);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->energyAvailable);
    doParsimPacking(b,this->batteryLevel);
}

void EnergyResponse::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::veins::BaseFrame1609_4::parsimUnpack(b);
    doParsimUnpacking(b,this->requestId);
    doParsimUnpacking(b,this->originAddress);
    doParsimUnpacking(b,this->responderAddress);
    doParsimUnpacking(b,this->nextHopAddress);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->energyAvailable);
    doParsimUnpacking(b,this->batteryLevel);
}

int EnergyResponse::getRequestId() const
{
    return this->requestId;
}

void EnergyResponse::setRequestId(int requestId)
{
    this->requestId = requestId;
}

int EnergyResponse::getOriginAddress() const
{
    return this->originAddress;
}

void EnergyResponse::setOriginAddress(int originAddress)
{
    this->originAddress = originAddress;
}

int EnergyResponse::getResponderAddress() const
{
    return this->responderAddress;
}

void EnergyResponse::setResponderAddress(int responderAddress)
{
    this->responderAddress = responderAddress;
}

int EnergyResponse::getNextHopAddress() const
{
    return this->nextHopAddress;
}

void EnergyResponse::setNextHopAddress(int nextHopAddress)
{
    this->nextHopAddress = nextHopAddress;
}

int EnergyResponse::getHopCount() const
{
    return this->hopCount;
}

void EnergyResponse::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

double EnergyResponse::getEnergyAvailable() const
{
    return this->energyAvailable;
}

void EnergyResponse::setEnergyAvailable(double energyAvailable)
{
    this->energyAvailable = energyAvailable;
}

double EnergyResponse::getBatteryLevel() const
{
    return this->batteryLevel;
}

void EnergyResponse::setBatteryLevel(double batteryLevel)
{
    this->batteryLevel = batteryLevel;
}

class EnergyResponseDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_requestId,
        FIELD_originAddress,
        FIELD_responderAddress,
        FIELD_nextHopAddress,
        FIELD_hopCount,
        FIELD_energyAvailable,
        FIELD_batteryLevel,
    };
  public:
    EnergyResponseDescriptor();
    virtual ~EnergyResponseDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(EnergyResponseDescriptor)

EnergyResponseDescriptor::EnergyResponseDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(veins::EnergyResponse)), "veins::BaseFrame1609_4")
{
    propertyNames = nullptr;
}

EnergyResponseDescriptor::~EnergyResponseDescriptor()
{
    delete[] propertyNames;
}

bool EnergyResponseDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<EnergyResponse *>(obj)!=nullptr;
}

const char **EnergyResponseDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *EnergyResponseDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int EnergyResponseDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 7+base->getFieldCount() : 7;
}

unsigned int EnergyResponseDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_requestId
        FD_ISEDITABLE,    // FIELD_originAddress
        FD_ISEDITABLE,    // FIELD_responderAddress
        FD_ISEDITABLE,    // FIELD_nextHopAddress
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_energyAvailable
        FD_ISEDITABLE,    // FIELD_batteryLevel
    };
    return (field >= 0 && field < 7) ? fieldTypeFlags[field] : 0;
}

const char *EnergyResponseDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "requestId",
        "originAddress",
        "responderAddress",
        "nextHopAddress",
        "hopCount",
        "energyAvailable",
        "batteryLevel",
    };
    return (field >= 0 && field < 7) ? fieldNames[field] : nullptr;
}

int EnergyResponseDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "requestId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "originAddress") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "responderAddress") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "nextHopAddress") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "energyAvailable") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "batteryLevel") == 0) return baseIndex + 6;
    return base ? base->findField(fieldName) : -1;
}

const char *EnergyResponseDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_requestId
        "int",    // FIELD_originAddress
        "int",    // FIELD_responderAddress
        "int",    // FIELD_nextHopAddress
        "int",    // FIELD_hopCount
        "double",    // FIELD_energyAvailable
        "double",    // FIELD_batteryLevel
    };
    return (field >= 0 && field < 7) ? fieldTypeStrings[field] : nullptr;
}

const char **EnergyResponseDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *EnergyResponseDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int EnergyResponseDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    EnergyResponse *pp = omnetpp::fromAnyPtr<EnergyResponse>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void EnergyResponseDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    EnergyResponse *pp = omnetpp::fromAnyPtr<EnergyResponse>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'EnergyResponse'", field);
    }
}

const char *EnergyResponseDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    EnergyResponse *pp = omnetpp::fromAnyPtr<EnergyResponse>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string EnergyResponseDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    EnergyResponse *pp = omnetpp::fromAnyPtr<EnergyResponse>(object); (void)pp;
    switch (field) {
        case FIELD_requestId: return long2string(pp->getRequestId());
        case FIELD_originAddress: return long2string(pp->getOriginAddress());
        case FIELD_responderAddress: return long2string(pp->getResponderAddress());
        case FIELD_nextHopAddress: return long2string(pp->getNextHopAddress());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_energyAvailable: return double2string(pp->getEnergyAvailable());
        case FIELD_batteryLevel: return double2string(pp->getBatteryLevel());
        default: return "";
    }
}

void EnergyResponseDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    EnergyResponse *pp = omnetpp::fromAnyPtr<EnergyResponse>(object); (void)pp;
    switch (field) {
        case FIELD_requestId: pp->setRequestId(string2long(value)); break;
        case FIELD_originAddress: pp->setOriginAddress(string2long(value)); break;
        case FIELD_responderAddress: pp->setResponderAddress(string2long(value)); break;
        case FIELD_nextHopAddress: pp->setNextHopAddress(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_energyAvailable: pp->setEnergyAvailable(string2double(value)); break;
        case FIELD_batteryLevel: pp->setBatteryLevel(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EnergyResponse'", field);
    }
}

omnetpp::cValue EnergyResponseDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    EnergyResponse *pp = omnetpp::fromAnyPtr<EnergyResponse>(object); (void)pp;
    switch (field) {
        case FIELD_requestId: return pp->getRequestId();
        case FIELD_originAddress: return pp->getOriginAddress();
        case FIELD_responderAddress: return pp->getResponderAddress();
        case FIELD_nextHopAddress: return pp->getNextHopAddress();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_energyAvailable: return pp->getEnergyAvailable();
        case FIELD_batteryLevel: return pp->getBatteryLevel();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'EnergyResponse' as cValue -- field index out of range?", field);
    }
}

void EnergyResponseDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    EnergyResponse *pp = omnetpp::fromAnyPtr<EnergyResponse>(object); (void)pp;
    switch (field) {
        case FIELD_requestId: pp->setRequestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_originAddress: pp->setOriginAddress(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_responderAddress: pp->setResponderAddress(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_nextHopAddress: pp->setNextHopAddress(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_energyAvailable: pp->setEnergyAvailable(value.doubleValue()); break;
        case FIELD_batteryLevel: pp->setBatteryLevel(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EnergyResponse'", field);
    }
}

const char *EnergyResponseDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr EnergyResponseDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    EnergyResponse *pp = omnetpp::fromAnyPtr<EnergyResponse>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void EnergyResponseDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    EnergyResponse *pp = omnetpp::fromAnyPtr<EnergyResponse>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EnergyResponse'", field);
    }
}

}  // namespace veins

namespace omnetpp {

}  // namespace omnetpp

