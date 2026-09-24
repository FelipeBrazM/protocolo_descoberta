#pragma once

#include <omnetpp.h>

#include <set>

using namespace omnetpp;

namespace veins {

class EnergyRequestStats : public cSimpleModule
{
protected:
    int hop0Received = 0;
    int hop1Received = 0;
    int retransmissions = 0;
    int duplicates = 0;

    simtime_t firstTransmissionTime = -1;
    simtime_t lastReceptionTime = -1;

    // IDs dos veículos que receberam a requisição.
    std::set<int> vehiclesHop0;
    std::set<int> vehiclesHop1;

protected:
    virtual void initialize() override;
    virtual void finish() override;

public:
    void recordTransmission(simtime_t time);

    void recordReception(
        int vehicleId,
        int hop,
        simtime_t time
    );

    void recordRetransmission();

    void recordDuplicate();
};

} // namespace veins