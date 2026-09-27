#pragma once
#include "EnergyProtocolApp.h"
#include "FixedDestinationMobility.h"
namespace veins {
class BatteryTruckApp : public EnergyProtocolApp {
public:
    ~BatteryTruckApp() override;
protected:
    enum class State { AVAILABLE, WAITING_CONFIRMATION, GOING_TO_REQUEST, MEETING, CHARGING };
    State state = State::AVAILABLE;
    FixedDestinationMobility driving;
    std::map<std::pair<int,int>,simtime_t> requests;
    int currentRequester = -1, currentRequest = 0, requestHops = 0;
    simtime_t requestStarted, deadline, confirmationDeadline, acceptedAt, metAt, chargingEnd;
    double requestedEnergy = 0;
    Coord destination;
    cMessage* chargeDone = nullptr;
    cMessage* responseExpired = nullptr;
    void initialize(int stage) override;
    void handleSelfMsg(cMessage* msg) override;
    void transition(State next);
    void protocolTick(double dt) override;
    void receiveProtocol(const EnergyRequest& m) override;
    EnergyRequest* serviceFrame(EnergyMessage type);
    void release(bool resume);
    void logService(const char* event);
    void finish() override;
};
}
