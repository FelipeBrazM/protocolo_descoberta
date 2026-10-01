#include "BatteryTruckApp.h"
#include <cmath>
using namespace veins;
Define_Module(BatteryTruckApp);

void BatteryTruckApp::initialize(int stage) {
    EnergyProtocolApp::initialize(stage);
    if (stage != 1) return;
    driving.initialize(traci, traciVehicle);
    chargeDone = new cMessage("chargeComplete");
    responseExpired = new cMessage("responseTimeout");
    logService("TRUCK_AVAILABLE");
}
BatteryTruckApp::~BatteryTruckApp() {
    cancelAndDelete(chargeDone);
    cancelAndDelete(responseExpired);
}
void BatteryTruckApp::transition(State next) {
    state = next;
    emit(registerSignal("batteryState"), static_cast<long>(next));
}
void BatteryTruckApp::logService(const char* event) {
    logEvent(event, currentRequester, currentRequest, myId, requestedEnergy, destination,
             state == State::CHARGING ? (simTime()-metAt).dbl() : 0);
}
EnergyRequest* BatteryTruckApp::serviceFrame(EnergyMessage type) {
    auto* m = frame(type, currentRequester);
    m->setRequesterId(currentRequester); m->setRequestId(currentRequest); m->setTruckId(myId);
    m->setRequestStarted(requestStarted); m->setRequestDeadline(deadline);
    m->setRequestHops(requestHops); m->setEnergyAmount(requestedEnergy);
    m->setPositionX(destination.x); m->setPositionY(destination.y);
    m->setChargingStartedAt(metAt); m->setChargingEndsAt(chargingEnd);
    return m;
}
void BatteryTruckApp::release(bool resume) {
    if (resume) driving.resumePatrol();
    transition(State::AVAILABLE); logService("TRUCK_AVAILABLE");
    currentRequester = -1; requestedEnergy = 0;
}
void BatteryTruckApp::protocolTick(double) {
    for (auto i=requests.begin(); i!=requests.end();) {
        if (i->second<=simTime()) i=requests.erase(i); else ++i;
    }
    if (state == State::CHARGING) return;
    driving.update();
    if (state != State::GOING_TO_REQUEST) return;
    if (simTime() >= deadline) {
        count("servicesExpired"); logService("TRUCK_TRAVEL_TIMEOUT"); release(true); return;
    }
    const double distance = std::hypot(curPosition.x-destination.x, curPosition.y-destination.y);
    if (distance > par("meetingDistance").doubleValue()) return;
    driving.hold();
    transition(State::MEETING); metAt = simTime(); count("meetings");
    sample("meetingTime", metAt.dbl()); sample("meetingDistanceObserved", distance);
    sample("travelTimeToMeeting", (simTime()-acceptedAt).dbl()); logService("TRUCK_MEETING");
    chargingEnd = simTime()+par("chargingDuration");
    transmit(serviceFrame(EnergyMessage::Meeting));
    transition(State::CHARGING); logService("ENERGY_TRANSFER_STARTED");
    sample("plannedChargingSeconds", (chargingEnd-metAt).dbl());
    scheduleAt(chargingEnd, chargeDone);
}
void BatteryTruckApp::receiveProtocol(const EnergyRequest& m) {
    const auto type = static_cast<EnergyMessage>(m.getMessageType());
    if (type == EnergyMessage::Request) {
        if (m.getRequesterId()!=m.getOriginAddress() || m.getRequestDeadline()<=simTime() || m.getEnergyRequired()<=0) return;
        if (!requests.emplace(std::make_pair(m.getRequesterId(),m.getRequestId()),m.getRequestDeadline()).second) return;
        count("requestsReceived"); sample("requestHopCount", m.getHopCount());
        logEvent("REQUEST_RECEIVED", m.getRequesterId(), m.getRequestId(), myId,
                 m.getEnergyRequired(), Coord(m.getPositionX(),m.getPositionY()));
        if (state != State::AVAILABLE || battery.energy() < m.getEnergyRequired()) {
            count("requestsRejected");
            count(state != State::AVAILABLE ? "requestsRejectedBusy" : "requestsRejectedEnergy");
            return;
        }
        currentRequester = m.getRequesterId(); currentRequest = m.getRequestId(); requestHops = m.getHopCount();
        requestStarted = m.getRequestStarted(); deadline = m.getRequestDeadline();
        requestedEnergy = m.getEnergyRequired(); destination = Coord(m.getPositionX(),m.getPositionY());
        confirmationDeadline = simTime()+par("responseTimeout");
        count("responsesSent"); transition(State::WAITING_CONFIRMATION);
        scheduleAt(confirmationDeadline, responseExpired);
        logService("WAITING_CONFIRMATION");
        // Break simultaneous replies using the existing configurable radio jitter.
        auto* response = serviceFrame(EnergyMessage::TruckResponse);
        pending.insert(response);
        scheduleAt(simTime()+uniform(0,par("forwardJitter").doubleValue()), response);
        return;
    }
    if (type != EnergyMessage::Confirmation || state != State::WAITING_CONFIRMATION || simTime() >= confirmationDeadline) return;
    if (m.getDestination()!=myId || m.getOriginAddress()!=currentRequester || m.getRequesterId()!=currentRequester ||
        m.getRequestId()!=currentRequest || m.getTruckId()!=myId) return;
    cancelEvent(responseExpired);
    logService("TRUCK_CONFIRMATION_RECEIVED");
    driving.goTo(destination);
    count("requestsAccepted"); acceptedAt = simTime(); transition(State::GOING_TO_REQUEST);
    logService("TRUCK_DESTINATION_SET"); logService("TRUCK_GOING_TO_REQUEST");
}
void BatteryTruckApp::handleSelfMsg(cMessage* msg) {
    if (msg == responseExpired) {
        count("responseTimeouts"); logService("TRUCK_RESPONSE_TIMEOUT"); release(false); return;
    }
    if (msg != chargeDone) { EnergyProtocolApp::handleSelfMsg(msg); return; }
    const double supplied = battery.consume(requestedEnergy);
    count("energySupplied", supplied); count("servicesCompleted");
    sample("transferElapsed", (simTime()-metAt).dbl());
    auto* transfer = serviceFrame(EnergyMessage::Transfer); transfer->setEnergyAmount(supplied);
    transmit(transfer); logService("ENERGY_TRANSFER_COMPLETED");
    // No receipt, rollback or second transaction state in this experiment.
    release(true);
}
void BatteryTruckApp::finish() {
    if (state == State::GOING_TO_REQUEST || state == State::CHARGING) count("servicesPendingAtFinish");
    if (state == State::WAITING_CONFIRMATION) count("responsesPendingAtFinish");
    EnergyProtocolApp::finish();
}
