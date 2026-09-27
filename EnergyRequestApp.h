#pragma once
#include "EnergyProtocolApp.h"
namespace veins {
class EnergyRequestApp : public EnergyProtocolApp {
protected:
    enum class State { NORMAL, LOW_BATTERY, REQUESTING, SEARCHING_TRUCK, MEETING_TRUCK, CHARGING, RECOVERED };
    State state = State::NORMAL;
    int requestId = 0, selectedTruck = -1;
    simtime_t requestStarted, deadline, nextRequest = 0, metAt, chargingEnd;
    double requested = 0, received = 0;
    bool met = false;
    Coord requestPosition;
    void initialize(int stage) override;
    void transition(State next);
    void protocolTick(double dt) override;
    void receiveProtocol(const EnergyRequest& m) override;
    void sendRequest();
    void logRequest(const char* event);
    void finish() override;
};
}
