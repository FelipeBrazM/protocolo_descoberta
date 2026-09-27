#include "EnergyRequestApp.h"
#include <algorithm>
#include <cmath>
using namespace veins;
Define_Module(EnergyRequestApp);

void EnergyRequestApp::initialize(int stage) {
    EnergyProtocolApp::initialize(stage);
    if (stage != 1) return;
    double target = par("batteryCapacity").doubleValue() * par("targetBatteryPercent").doubleValue() / 100;
    if (par("targetBatteryPercent").doubleValue() > 100 || target <= par("lowBatteryThreshold").doubleValue())
        throw cRuntimeError("Battery target must exceed lowBatteryThreshold and not exceed capacity");
}
void EnergyRequestApp::transition(State next) {
    state = next;
    emit(registerSignal("batteryState"), static_cast<long>(next));
}
void EnergyRequestApp::logRequest(const char* event) {
    logEvent(event, myId, requestId, selectedTruck, requested, requestPosition,
             met ? (simTime()-metAt).dbl() : 0);
}
void EnergyRequestApp::sendRequest() {
    auto* m = frame(EnergyMessage::Request);
    m->setRequestId(requestId); m->setRequesterId(myId);
    m->setRequestStarted(requestStarted); m->setRequestDeadline(deadline);
    m->setEnergyRequired(requested);
    // Reannouncements retain the original snapshot, even while the car moves.
    m->setPositionX(requestPosition.x); m->setPositionY(requestPosition.y);
    transmit(m);
    nextRequest = simTime()+par("requestInterval");
    count("requestAnnouncements"); logRequest("REQUEST_SENT");
}
void EnergyRequestApp::protocolTick(double dt) {
    if (state != State::CHARGING)
        count("energyConsumed", battery.consume(par("consumptionRate").doubleValue()*dt));
    if (state == State::NORMAL || state == State::RECOVERED || state == State::LOW_BATTERY) {
        if (battery.energy() > par("lowBatteryThreshold").doubleValue() || simTime() < nextRequest) return;
        transition(State::LOW_BATTERY);
        ++requestId; selectedTruck = -1; met = false; received = 0;
        requested = par("batteryCapacity").doubleValue()*par("targetBatteryPercent").doubleValue()/100-battery.energy();
        requestStarted = simTime(); deadline = simTime()+par("requestLifetime");
        requestPosition = curPosition;
        count("requests"); count("energyRequested", requested);
        transition(State::REQUESTING); sendRequest(); transition(State::SEARCHING_TRUCK);
        return;
    }
    if (simTime() >= deadline) {
        count("requestsExpired");
        if (!met) count("requestsWithoutMeeting");
        if (selectedTruck < 0) count("requestsWithoutTruck");
        if (state == State::CHARGING) traciVehicle->setSpeed(-1);
        logRequest("REQUEST_EXPIRED");
        transition(State::LOW_BATTERY); nextRequest = simTime()+par("requestInterval");
        return;
    }
    if (state == State::SEARCHING_TRUCK && simTime() >= nextRequest) sendRequest();
}
void EnergyRequestApp::receiveProtocol(const EnergyRequest& m) {
    if (m.getDestination()!=myId || m.getRequesterId()!=myId || m.getRequestId()!=requestId) return;
    if (simTime() >= deadline || m.getOriginAddress()!=m.getTruckId()) return;
    auto type = static_cast<EnergyMessage>(m.getMessageType());
    if (type == EnergyMessage::TruckResponse && state == State::SEARCHING_TRUCK) {
        selectedTruck = m.getTruckId(); // first valid response; no ranking or distance lookup
        sample("requestHopCount", m.getRequestHops());
        auto* confirmation = frame(EnergyMessage::Confirmation, selectedTruck);
        confirmation->setRequesterId(myId); confirmation->setRequestId(requestId);
        confirmation->setTruckId(selectedTruck);
        transmit(confirmation);
        transition(State::MEETING_TRUCK); logRequest("REQUEST_CONFIRMED");
        return;
    }
    if (m.getTruckId()!=selectedTruck) return;
    if (type == EnergyMessage::Meeting && state == State::MEETING_TRUCK) {
        met = true; metAt = m.getChargingStartedAt(); chargingEnd = m.getChargingEndsAt();
        if (chargingEnd <= simTime()) return;
        // The fixed-target experiment freezes the requester at its current
        // position on MEETING. It does not chase or relocate it to the snapshot.
        traciVehicle->setSpeed(0);
        count("meetings"); sample("meetingTime", simTime().dbl());
        sample("requestToMeeting", (simTime()-requestStarted).dbl());
        sample("requesterDisplacementAtMeeting", std::hypot(curPosition.x-requestPosition.x,curPosition.y-requestPosition.y));
        sample("energyBeforeService", battery.energy());
        logRequest("MEETING_REACHED");
        transition(State::CHARGING);
        deadline = chargingEnd+par("messageTtl")+par("tickInterval");
        logRequest("ENERGY_TRANSFER_STARTED");
        return;
    }
    if (type == EnergyMessage::Transfer && state == State::CHARGING && received==0) {
        if (simTime() < chargingEnd || m.getEnergyAmount() <= 0 || m.getEnergyAmount() > requested) return;
        received = battery.charge(m.getEnergyAmount());
        count("energyReceived", received); count("requestsServed");
        sample("requestToService", (simTime()-requestStarted).dbl());
        sample("transferElapsed", (simTime()-metAt).dbl()); sample("energyAfterService", battery.energy());
        logRequest("ENERGY_TRANSFER_COMPLETED");
        transition(State::RECOVERED); traciVehicle->setSpeed(-1);
    }
}
void EnergyRequestApp::finish() {
    if (state!=State::NORMAL && state!=State::RECOVERED && state!=State::LOW_BATTERY)
        count("requestsPendingAtFinish");
    EnergyProtocolApp::finish();
}
