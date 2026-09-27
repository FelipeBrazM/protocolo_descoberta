//
// Generated file, do not edit! Created by opp_msgtool 6.3 from EnergyRequest.msg.
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
#include "EnergyRequest_m.h"

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

Register_Class(EnergyRequest)

EnergyRequest::EnergyRequest(const char *name, short kind) : ::veins::BaseFrame1609_4(name, kind)
{
}

EnergyRequest::EnergyRequest(const EnergyRequest& other) : ::veins::BaseFrame1609_4(other)
{
    copy(other);
}

EnergyRequest::~EnergyRequest()
{
}

EnergyRequest& EnergyRequest::operator=(const EnergyRequest& other)
{
    if (this == &other) return *this;
    ::veins::BaseFrame1609_4::operator=(other);
    copy(other);
    return *this;
}

void EnergyRequest::copy(const EnergyRequest& other)
{
    this->messageType = other.messageType;
    this->messageId = other.messageId;
    this->destination = other.destination;
    this->requesterId = other.requesterId;
    this->truckId = other.truckId;
    this->hopLimit = other.hopLimit;
    this->createdAt = other.createdAt;
    this->expiresAt = other.expiresAt;
    this->requestStarted = other.requestStarted;
    this->requestDeadline = other.requestDeadline;
    this->chargingStartedAt = other.chargingStartedAt;
    this->chargingEndsAt = other.chargingEndsAt;
    this->energyAmount = other.energyAmount;
    this->velocityX = other.velocityX;
    this->velocityY = other.velocityY;
    this->requestHops = other.requestHops;
    this->requestId = other.requestId;
    this->originAddress = other.originAddress;
    this->previousHopAddress = other.previousHopAddress;
    this->hopCount = other.hopCount;
    this->energyRequired = other.energyRequired;
    this->positionX = other.positionX;
    this->positionY = other.positionY;
    this->speed = other.speed;
    this->heading = other.heading;
}

void EnergyRequest::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::veins::BaseFrame1609_4::parsimPack(b);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->messageId);
    doParsimPacking(b,this->destination);
    doParsimPacking(b,this->requesterId);
    doParsimPacking(b,this->truckId);
    doParsimPacking(b,this->hopLimit);
    doParsimPacking(b,this->createdAt);
    doParsimPacking(b,this->expiresAt);
    doParsimPacking(b,this->requestStarted);
    doParsimPacking(b,this->requestDeadline);
    doParsimPacking(b,this->chargingStartedAt);
    doParsimPacking(b,this->chargingEndsAt);
    doParsimPacking(b,this->energyAmount);
    doParsimPacking(b,this->velocityX);
    doParsimPacking(b,this->velocityY);
    doParsimPacking(b,this->requestHops);
    doParsimPacking(b,this->requestId);
    doParsimPacking(b,this->originAddress);
    doParsimPacking(b,this->previousHopAddress);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->energyRequired);
    doParsimPacking(b,this->positionX);
    doParsimPacking(b,this->positionY);
    doParsimPacking(b,this->speed);
    doParsimPacking(b,this->heading);
}

void EnergyRequest::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::veins::BaseFrame1609_4::parsimUnpack(b);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->messageId);
    doParsimUnpacking(b,this->destination);
    doParsimUnpacking(b,this->requesterId);
    doParsimUnpacking(b,this->truckId);
    doParsimUnpacking(b,this->hopLimit);
    doParsimUnpacking(b,this->createdAt);
    doParsimUnpacking(b,this->expiresAt);
    doParsimUnpacking(b,this->requestStarted);
    doParsimUnpacking(b,this->requestDeadline);
    doParsimUnpacking(b,this->chargingStartedAt);
    doParsimUnpacking(b,this->chargingEndsAt);
    doParsimUnpacking(b,this->energyAmount);
    doParsimUnpacking(b,this->velocityX);
    doParsimUnpacking(b,this->velocityY);
    doParsimUnpacking(b,this->requestHops);
    doParsimUnpacking(b,this->requestId);
    doParsimUnpacking(b,this->originAddress);
    doParsimUnpacking(b,this->previousHopAddress);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->energyRequired);
    doParsimUnpacking(b,this->positionX);
    doParsimUnpacking(b,this->positionY);
    doParsimUnpacking(b,this->speed);
    doParsimUnpacking(b,this->heading);
}

int EnergyRequest::getMessageType() const
{
    return this->messageType;
}

void EnergyRequest::setMessageType(int messageType)
{
    this->messageType = messageType;
}

long EnergyRequest::getMessageId() const
{
    return this->messageId;
}

void EnergyRequest::setMessageId(long messageId)
{
    this->messageId = messageId;
}

int EnergyRequest::getDestination() const
{
    return this->destination;
}

void EnergyRequest::setDestination(int destination)
{
    this->destination = destination;
}

int EnergyRequest::getRequesterId() const
{
    return this->requesterId;
}

void EnergyRequest::setRequesterId(int requesterId)
{
    this->requesterId = requesterId;
}

int EnergyRequest::getTruckId() const
{
    return this->truckId;
}

void EnergyRequest::setTruckId(int truckId)
{
    this->truckId = truckId;
}

int EnergyRequest::getHopLimit() const
{
    return this->hopLimit;
}

void EnergyRequest::setHopLimit(int hopLimit)
{
    this->hopLimit = hopLimit;
}

::omnetpp::simtime_t EnergyRequest::getCreatedAt() const
{
    return this->createdAt;
}

void EnergyRequest::setCreatedAt(::omnetpp::simtime_t createdAt)
{
    this->createdAt = createdAt;
}

::omnetpp::simtime_t EnergyRequest::getExpiresAt() const
{
    return this->expiresAt;
}

void EnergyRequest::setExpiresAt(::omnetpp::simtime_t expiresAt)
{
    this->expiresAt = expiresAt;
}

::omnetpp::simtime_t EnergyRequest::getRequestStarted() const
{
    return this->requestStarted;
}

void EnergyRequest::setRequestStarted(::omnetpp::simtime_t requestStarted)
{
    this->requestStarted = requestStarted;
}

::omnetpp::simtime_t EnergyRequest::getRequestDeadline() const
{
    return this->requestDeadline;
}

void EnergyRequest::setRequestDeadline(::omnetpp::simtime_t requestDeadline)
{
    this->requestDeadline = requestDeadline;
}

::omnetpp::simtime_t EnergyRequest::getChargingStartedAt() const
{
    return this->chargingStartedAt;
}

void EnergyRequest::setChargingStartedAt(::omnetpp::simtime_t chargingStartedAt)
{
    this->chargingStartedAt = chargingStartedAt;
}

::omnetpp::simtime_t EnergyRequest::getChargingEndsAt() const
{
    return this->chargingEndsAt;
}

void EnergyRequest::setChargingEndsAt(::omnetpp::simtime_t chargingEndsAt)
{
    this->chargingEndsAt = chargingEndsAt;
}

double EnergyRequest::getEnergyAmount() const
{
    return this->energyAmount;
}

void EnergyRequest::setEnergyAmount(double energyAmount)
{
    this->energyAmount = energyAmount;
}

double EnergyRequest::getVelocityX() const
{
    return this->velocityX;
}

void EnergyRequest::setVelocityX(double velocityX)
{
    this->velocityX = velocityX;
}

double EnergyRequest::getVelocityY() const
{
    return this->velocityY;
}

void EnergyRequest::setVelocityY(double velocityY)
{
    this->velocityY = velocityY;
}

int EnergyRequest::getRequestHops() const
{
    return this->requestHops;
}

void EnergyRequest::setRequestHops(int requestHops)
{
    this->requestHops = requestHops;
}

int EnergyRequest::getRequestId() const
{
    return this->requestId;
}

void EnergyRequest::setRequestId(int requestId)
{
    this->requestId = requestId;
}

int EnergyRequest::getOriginAddress() const
{
    return this->originAddress;
}

void EnergyRequest::setOriginAddress(int originAddress)
{
    this->originAddress = originAddress;
}

int EnergyRequest::getPreviousHopAddress() const
{
    return this->previousHopAddress;
}

void EnergyRequest::setPreviousHopAddress(int previousHopAddress)
{
    this->previousHopAddress = previousHopAddress;
}

int EnergyRequest::getHopCount() const
{
    return this->hopCount;
}

void EnergyRequest::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

double EnergyRequest::getEnergyRequired() const
{
    return this->energyRequired;
}

void EnergyRequest::setEnergyRequired(double energyRequired)
{
    this->energyRequired = energyRequired;
}

double EnergyRequest::getPositionX() const
{
    return this->positionX;
}

void EnergyRequest::setPositionX(double positionX)
{
    this->positionX = positionX;
}

double EnergyRequest::getPositionY() const
{
    return this->positionY;
}

void EnergyRequest::setPositionY(double positionY)
{
    this->positionY = positionY;
}

double EnergyRequest::getSpeed() const
{
    return this->speed;
}

void EnergyRequest::setSpeed(double speed)
{
    this->speed = speed;
}

double EnergyRequest::getHeading() const
{
    return this->heading;
}

void EnergyRequest::setHeading(double heading)
{
    this->heading = heading;
}

class EnergyRequestDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_messageType,
        FIELD_messageId,
        FIELD_destination,
        FIELD_requesterId,
        FIELD_truckId,
        FIELD_hopLimit,
        FIELD_createdAt,
        FIELD_expiresAt,
        FIELD_requestStarted,
        FIELD_requestDeadline,
        FIELD_chargingStartedAt,
        FIELD_chargingEndsAt,
        FIELD_energyAmount,
        FIELD_velocityX,
        FIELD_velocityY,
        FIELD_requestHops,
        FIELD_requestId,
        FIELD_originAddress,
        FIELD_previousHopAddress,
        FIELD_hopCount,
        FIELD_energyRequired,
        FIELD_positionX,
        FIELD_positionY,
        FIELD_speed,
        FIELD_heading,
    };
  public:
    EnergyRequestDescriptor();
    virtual ~EnergyRequestDescriptor();

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

Register_ClassDescriptor(EnergyRequestDescriptor)

EnergyRequestDescriptor::EnergyRequestDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(veins::EnergyRequest)), "veins::BaseFrame1609_4")
{
    propertyNames = nullptr;
}

EnergyRequestDescriptor::~EnergyRequestDescriptor()
{
    delete[] propertyNames;
}

bool EnergyRequestDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<EnergyRequest *>(obj)!=nullptr;
}

const char **EnergyRequestDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *EnergyRequestDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int EnergyRequestDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 25+base->getFieldCount() : 25;
}

unsigned int EnergyRequestDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_messageId
        FD_ISEDITABLE,    // FIELD_destination
        FD_ISEDITABLE,    // FIELD_requesterId
        FD_ISEDITABLE,    // FIELD_truckId
        FD_ISEDITABLE,    // FIELD_hopLimit
        FD_ISEDITABLE,    // FIELD_createdAt
        FD_ISEDITABLE,    // FIELD_expiresAt
        FD_ISEDITABLE,    // FIELD_requestStarted
        FD_ISEDITABLE,    // FIELD_requestDeadline
        FD_ISEDITABLE,    // FIELD_chargingStartedAt
        FD_ISEDITABLE,    // FIELD_chargingEndsAt
        FD_ISEDITABLE,    // FIELD_energyAmount
        FD_ISEDITABLE,    // FIELD_velocityX
        FD_ISEDITABLE,    // FIELD_velocityY
        FD_ISEDITABLE,    // FIELD_requestHops
        FD_ISEDITABLE,    // FIELD_requestId
        FD_ISEDITABLE,    // FIELD_originAddress
        FD_ISEDITABLE,    // FIELD_previousHopAddress
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_energyRequired
        FD_ISEDITABLE,    // FIELD_positionX
        FD_ISEDITABLE,    // FIELD_positionY
        FD_ISEDITABLE,    // FIELD_speed
        FD_ISEDITABLE,    // FIELD_heading
    };
    return (field >= 0 && field < 25) ? fieldTypeFlags[field] : 0;
}

const char *EnergyRequestDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "messageType",
        "messageId",
        "destination",
        "requesterId",
        "truckId",
        "hopLimit",
        "createdAt",
        "expiresAt",
        "requestStarted",
        "requestDeadline",
        "chargingStartedAt",
        "chargingEndsAt",
        "energyAmount",
        "velocityX",
        "velocityY",
        "requestHops",
        "requestId",
        "originAddress",
        "previousHopAddress",
        "hopCount",
        "energyRequired",
        "positionX",
        "positionY",
        "speed",
        "heading",
    };
    return (field >= 0 && field < 25) ? fieldNames[field] : nullptr;
}

int EnergyRequestDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "messageId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "destination") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "requesterId") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "truckId") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "hopLimit") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "createdAt") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "expiresAt") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "requestStarted") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "requestDeadline") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "chargingStartedAt") == 0) return baseIndex + 10;
    if (strcmp(fieldName, "chargingEndsAt") == 0) return baseIndex + 11;
    if (strcmp(fieldName, "energyAmount") == 0) return baseIndex + 12;
    if (strcmp(fieldName, "velocityX") == 0) return baseIndex + 13;
    if (strcmp(fieldName, "velocityY") == 0) return baseIndex + 14;
    if (strcmp(fieldName, "requestHops") == 0) return baseIndex + 15;
    if (strcmp(fieldName, "requestId") == 0) return baseIndex + 16;
    if (strcmp(fieldName, "originAddress") == 0) return baseIndex + 17;
    if (strcmp(fieldName, "previousHopAddress") == 0) return baseIndex + 18;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 19;
    if (strcmp(fieldName, "energyRequired") == 0) return baseIndex + 20;
    if (strcmp(fieldName, "positionX") == 0) return baseIndex + 21;
    if (strcmp(fieldName, "positionY") == 0) return baseIndex + 22;
    if (strcmp(fieldName, "speed") == 0) return baseIndex + 23;
    if (strcmp(fieldName, "heading") == 0) return baseIndex + 24;
    return base ? base->findField(fieldName) : -1;
}

const char *EnergyRequestDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_messageType
        "long",    // FIELD_messageId
        "int",    // FIELD_destination
        "int",    // FIELD_requesterId
        "int",    // FIELD_truckId
        "int",    // FIELD_hopLimit
        "omnetpp::simtime_t",    // FIELD_createdAt
        "omnetpp::simtime_t",    // FIELD_expiresAt
        "omnetpp::simtime_t",    // FIELD_requestStarted
        "omnetpp::simtime_t",    // FIELD_requestDeadline
        "omnetpp::simtime_t",    // FIELD_chargingStartedAt
        "omnetpp::simtime_t",    // FIELD_chargingEndsAt
        "double",    // FIELD_energyAmount
        "double",    // FIELD_velocityX
        "double",    // FIELD_velocityY
        "int",    // FIELD_requestHops
        "int",    // FIELD_requestId
        "int",    // FIELD_originAddress
        "int",    // FIELD_previousHopAddress
        "int",    // FIELD_hopCount
        "double",    // FIELD_energyRequired
        "double",    // FIELD_positionX
        "double",    // FIELD_positionY
        "double",    // FIELD_speed
        "double",    // FIELD_heading
    };
    return (field >= 0 && field < 25) ? fieldTypeStrings[field] : nullptr;
}

const char **EnergyRequestDescriptor::getFieldPropertyNames(int field) const
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

const char *EnergyRequestDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int EnergyRequestDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    EnergyRequest *pp = omnetpp::fromAnyPtr<EnergyRequest>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void EnergyRequestDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    EnergyRequest *pp = omnetpp::fromAnyPtr<EnergyRequest>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'EnergyRequest'", field);
    }
}

const char *EnergyRequestDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    EnergyRequest *pp = omnetpp::fromAnyPtr<EnergyRequest>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string EnergyRequestDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    EnergyRequest *pp = omnetpp::fromAnyPtr<EnergyRequest>(object); (void)pp;
    switch (field) {
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_messageId: return long2string(pp->getMessageId());
        case FIELD_destination: return long2string(pp->getDestination());
        case FIELD_requesterId: return long2string(pp->getRequesterId());
        case FIELD_truckId: return long2string(pp->getTruckId());
        case FIELD_hopLimit: return long2string(pp->getHopLimit());
        case FIELD_createdAt: return simtime2string(pp->getCreatedAt());
        case FIELD_expiresAt: return simtime2string(pp->getExpiresAt());
        case FIELD_requestStarted: return simtime2string(pp->getRequestStarted());
        case FIELD_requestDeadline: return simtime2string(pp->getRequestDeadline());
        case FIELD_chargingStartedAt: return simtime2string(pp->getChargingStartedAt());
        case FIELD_chargingEndsAt: return simtime2string(pp->getChargingEndsAt());
        case FIELD_energyAmount: return double2string(pp->getEnergyAmount());
        case FIELD_velocityX: return double2string(pp->getVelocityX());
        case FIELD_velocityY: return double2string(pp->getVelocityY());
        case FIELD_requestHops: return long2string(pp->getRequestHops());
        case FIELD_requestId: return long2string(pp->getRequestId());
        case FIELD_originAddress: return long2string(pp->getOriginAddress());
        case FIELD_previousHopAddress: return long2string(pp->getPreviousHopAddress());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_energyRequired: return double2string(pp->getEnergyRequired());
        case FIELD_positionX: return double2string(pp->getPositionX());
        case FIELD_positionY: return double2string(pp->getPositionY());
        case FIELD_speed: return double2string(pp->getSpeed());
        case FIELD_heading: return double2string(pp->getHeading());
        default: return "";
    }
}

void EnergyRequestDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    EnergyRequest *pp = omnetpp::fromAnyPtr<EnergyRequest>(object); (void)pp;
    switch (field) {
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_messageId: pp->setMessageId(string2long(value)); break;
        case FIELD_destination: pp->setDestination(string2long(value)); break;
        case FIELD_requesterId: pp->setRequesterId(string2long(value)); break;
        case FIELD_truckId: pp->setTruckId(string2long(value)); break;
        case FIELD_hopLimit: pp->setHopLimit(string2long(value)); break;
        case FIELD_createdAt: pp->setCreatedAt(string2simtime(value)); break;
        case FIELD_expiresAt: pp->setExpiresAt(string2simtime(value)); break;
        case FIELD_requestStarted: pp->setRequestStarted(string2simtime(value)); break;
        case FIELD_requestDeadline: pp->setRequestDeadline(string2simtime(value)); break;
        case FIELD_chargingStartedAt: pp->setChargingStartedAt(string2simtime(value)); break;
        case FIELD_chargingEndsAt: pp->setChargingEndsAt(string2simtime(value)); break;
        case FIELD_energyAmount: pp->setEnergyAmount(string2double(value)); break;
        case FIELD_velocityX: pp->setVelocityX(string2double(value)); break;
        case FIELD_velocityY: pp->setVelocityY(string2double(value)); break;
        case FIELD_requestHops: pp->setRequestHops(string2long(value)); break;
        case FIELD_requestId: pp->setRequestId(string2long(value)); break;
        case FIELD_originAddress: pp->setOriginAddress(string2long(value)); break;
        case FIELD_previousHopAddress: pp->setPreviousHopAddress(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_energyRequired: pp->setEnergyRequired(string2double(value)); break;
        case FIELD_positionX: pp->setPositionX(string2double(value)); break;
        case FIELD_positionY: pp->setPositionY(string2double(value)); break;
        case FIELD_speed: pp->setSpeed(string2double(value)); break;
        case FIELD_heading: pp->setHeading(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EnergyRequest'", field);
    }
}

omnetpp::cValue EnergyRequestDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    EnergyRequest *pp = omnetpp::fromAnyPtr<EnergyRequest>(object); (void)pp;
    switch (field) {
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_messageId: return (omnetpp::intval_t)(pp->getMessageId());
        case FIELD_destination: return pp->getDestination();
        case FIELD_requesterId: return pp->getRequesterId();
        case FIELD_truckId: return pp->getTruckId();
        case FIELD_hopLimit: return pp->getHopLimit();
        case FIELD_createdAt: return pp->getCreatedAt().dbl();
        case FIELD_expiresAt: return pp->getExpiresAt().dbl();
        case FIELD_requestStarted: return pp->getRequestStarted().dbl();
        case FIELD_requestDeadline: return pp->getRequestDeadline().dbl();
        case FIELD_chargingStartedAt: return pp->getChargingStartedAt().dbl();
        case FIELD_chargingEndsAt: return pp->getChargingEndsAt().dbl();
        case FIELD_energyAmount: return pp->getEnergyAmount();
        case FIELD_velocityX: return pp->getVelocityX();
        case FIELD_velocityY: return pp->getVelocityY();
        case FIELD_requestHops: return pp->getRequestHops();
        case FIELD_requestId: return pp->getRequestId();
        case FIELD_originAddress: return pp->getOriginAddress();
        case FIELD_previousHopAddress: return pp->getPreviousHopAddress();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_energyRequired: return pp->getEnergyRequired();
        case FIELD_positionX: return pp->getPositionX();
        case FIELD_positionY: return pp->getPositionY();
        case FIELD_speed: return pp->getSpeed();
        case FIELD_heading: return pp->getHeading();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'EnergyRequest' as cValue -- field index out of range?", field);
    }
}

void EnergyRequestDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    EnergyRequest *pp = omnetpp::fromAnyPtr<EnergyRequest>(object); (void)pp;
    switch (field) {
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_messageId: pp->setMessageId(omnetpp::checked_int_cast<long>(value.intValue())); break;
        case FIELD_destination: pp->setDestination(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_requesterId: pp->setRequesterId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_truckId: pp->setTruckId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopLimit: pp->setHopLimit(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_createdAt: pp->setCreatedAt(value.doubleValue()); break;
        case FIELD_expiresAt: pp->setExpiresAt(value.doubleValue()); break;
        case FIELD_requestStarted: pp->setRequestStarted(value.doubleValue()); break;
        case FIELD_requestDeadline: pp->setRequestDeadline(value.doubleValue()); break;
        case FIELD_chargingStartedAt: pp->setChargingStartedAt(value.doubleValue()); break;
        case FIELD_chargingEndsAt: pp->setChargingEndsAt(value.doubleValue()); break;
        case FIELD_energyAmount: pp->setEnergyAmount(value.doubleValue()); break;
        case FIELD_velocityX: pp->setVelocityX(value.doubleValue()); break;
        case FIELD_velocityY: pp->setVelocityY(value.doubleValue()); break;
        case FIELD_requestHops: pp->setRequestHops(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_requestId: pp->setRequestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_originAddress: pp->setOriginAddress(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_previousHopAddress: pp->setPreviousHopAddress(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_energyRequired: pp->setEnergyRequired(value.doubleValue()); break;
        case FIELD_positionX: pp->setPositionX(value.doubleValue()); break;
        case FIELD_positionY: pp->setPositionY(value.doubleValue()); break;
        case FIELD_speed: pp->setSpeed(value.doubleValue()); break;
        case FIELD_heading: pp->setHeading(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EnergyRequest'", field);
    }
}

const char *EnergyRequestDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr EnergyRequestDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    EnergyRequest *pp = omnetpp::fromAnyPtr<EnergyRequest>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void EnergyRequestDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    EnergyRequest *pp = omnetpp::fromAnyPtr<EnergyRequest>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EnergyRequest'", field);
    }
}

}  // namespace veins

namespace omnetpp {

}  // namespace omnetpp

