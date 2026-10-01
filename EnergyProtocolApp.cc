#include "EnergyProtocolApp.h"
#include <cmath>
using namespace veins;
void EnergyProtocolApp::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if(stage!=1) return;
    double initial = par("initialBattery");
    if (initial < 0) {
        double lo = par("initialBatteryMin"), hi = par("initialBatteryMax");
        if (lo < 0 || hi < lo || hi > par("batteryCapacity").doubleValue())
            throw cRuntimeError("Invalid initial battery range");
        initial = uniform(lo, hi);
    }
    battery.initialize(par("batteryCapacity"), initial);
    for(const char* p : {"tickInterval","messageTtl","requestLifetime","requestInterval","meetingDistance","chargingDuration","responseTimeout"})
        if(par(p).doubleValue()<=0) throw cRuntimeError("%s must be positive",p);
    if(par("maxHops").intValue()<1 || par("forwardJitter").doubleValue()<0)
        throw cRuntimeError("Invalid hop limit or jitter");
    if(par("consumptionRate").doubleValue()<0 || par("lowBatteryThreshold").doubleValue()<0 || par("lowBatteryThreshold").doubleValue()>par("batteryCapacity").doubleValue())
        throw cRuntimeError("Invalid consumption or low-battery threshold");
    for(const char* name:{"messagesTransmitted","messagesReceived","messagesExpired","duplicatesDiscarded","retransmissions","hopLimitDrops","requests","requestsServed","requestsExpired","requestsWithoutMeeting","requestsWithoutTruck","requestsPendingAtFinish","energyRequested","energyReceived","energyConsumed","requestsReceived","requestsAccepted","requestsRejected","requestsRejectedBusy","requestsRejectedEnergy","meetings","energySupplied","servicesCompleted","servicesExpired","servicesPendingAtFinish","responseTimeouts"}) counters[name]=0;
    sample("initialEnergy",battery.energy());
    lastTick=simTime(); tick=new cMessage("batteryTick");
    scheduleAt(simTime()+par("tickInterval"),tick);
}
EnergyProtocolApp::~EnergyProtocolApp() {
    cancelAndDelete(tick);
    for(auto msg:pending) cancelAndDelete(msg);
}
void EnergyProtocolApp::count(const char* name,double amount) { counters[name]+=amount; emit(registerSignal(name),amount); }
void EnergyProtocolApp::sample(const char* name,double value) { samples[name].collect(value); emit(registerSignal(name),value); }
EnergyRequest* EnergyProtocolApp::frame(EnergyMessage type,int destination) {
    auto* m=new EnergyRequest("EnergyProtocol"); populateWSM(m);
    m->setMessageType(static_cast<int>(type)); m->setMessageId(++sequence);
    m->setOriginAddress(myId); m->setPreviousHopAddress(myId);
    m->setDestination(destination); m->setHopCount(1); m->setHopLimit(par("maxHops"));
    m->setCreatedAt(simTime()); m->setExpiresAt(simTime()+par("messageTtl"));
    // Own GNSS/odometry only. Other actors' coordinates only arrive in frames.
    m->setPositionX(curPosition.x); m->setPositionY(curPosition.y);
    m->setVelocityX(curSpeed.x); m->setVelocityY(curSpeed.y);
    m->setSpeed(curSpeed.length()); m->setHeading(std::atan2(curSpeed.y,curSpeed.x));
    cache.insert(myId,m->getMessageId(),m->getExpiresAt());
    return m;
}
void EnergyProtocolApp::transmit(EnergyRequest* m) {
    if(m->getExpiresAt()<=simTime()) { count("messagesExpired"); delete m; return; }
    if (m->getMessageType()==static_cast<int>(EnergyMessage::TruckResponse) && m->getOriginAddress()==myId)
        logEvent("TRUCK_RESPONSE_SENT", m->getRequesterId(), m->getRequestId(), myId,
                 m->getEnergyAmount(), Coord(m->getPositionX(),m->getPositionY()));
    if(m->getHopCount()>1)count("retransmissions");
    if (par("logNetworkDetails").boolValue() && m->getMessageType()==static_cast<int>(EnergyMessage::Request) && m->getHopCount()>1)
        EV_INFO << "REQUEST_FORWARDED originAddress=" << m->getOriginAddress()
                << " requestId=" << m->getRequestId() << " previousHopAddress=" << myId
                << " hopCount=" << m->getHopCount() << " simTime=" << simTime() << endl;
    count("messagesTransmitted"); sendDown(m);
}
void EnergyProtocolApp::handleSelfMsg(cMessage* msg) {
    if(msg==tick) {
        cache.expire(simTime()); double dt=(simTime()-lastTick).dbl();lastTick=simTime();
        protocolTick(dt); scheduleAt(simTime()+par("tickInterval"),tick); return;
    }
    auto it=pending.find(msg);
    if(it!=pending.end()) { pending.erase(it); transmit(check_and_cast<EnergyRequest*>(msg));return; }
    DemoBaseApplLayer::handleSelfMsg(msg);
}
void EnergyProtocolApp::onWSM(BaseFrame1609_4* wsm) {
    auto* m=dynamic_cast<EnergyRequest*>(wsm); if(!m) return;
    count("messagesReceived");
    if(m->getExpiresAt()<=simTime()) { count("messagesExpired");return; }
    if(m->getHopCount()<1 || m->getHopCount()>m->getHopLimit()) {count("hopLimitDrops");return;}
    if(!cache.insert(m->getOriginAddress(),m->getMessageId(),m->getExpiresAt())) {
        count("duplicatesDiscarded");
        if (par("logNetworkDetails").boolValue() && m->getMessageType()==static_cast<int>(EnergyMessage::Request))
            EV_INFO << "REQUEST_DROPPED_DUPLICATE originAddress=" << m->getOriginAddress()
                    << " requestId=" << m->getRequestId() << " simTime=" << simTime() << endl;
        return;
    }
    if (par("logNetworkDetails").boolValue() && m->getMessageType()==static_cast<int>(EnergyMessage::Request))
        EV_INFO << "REQUEST_RECEIVED node=" << getParentModule()->getFullName()
                << " originAddress=" << m->getOriginAddress() << " requestId=" << m->getRequestId()
                << " hopCount=" << m->getHopCount() << " simTime=" << simTime() << endl;
    sample("hopCount",m->getHopCount()); sample("messageLatency",(simTime()-m->getCreatedAt()).dbl());
    receiveProtocol(*m);
    if(m->getDestination()==myId) return;
    if(m->getHopCount()>=m->getHopLimit()) {count("hopLimitDrops");return;}
    auto* f=m->dup(); f->setHopCount(m->getHopCount()+1); f->setPreviousHopAddress(myId);
    pending.insert(f);
    scheduleAt(simTime()+uniform(0,par("forwardJitter").doubleValue()),f);
}
void EnergyProtocolApp::logEvent(const char* event, int requester, int request, int truck,
                                 double energy, const Coord& destination, double duration) const {
    EV_INFO << event << " node=" << getParentModule()->getFullName()
            << " vehicleId=" << mobility->getExternalId() << " address=" << myId
            << " originAddress=" << requester << " requestId=" << request
            << " truckId=" << truck << " energyRequired=" << energy
            << " position=(" << curPosition.x << "," << curPosition.y << ")"
            << " destination=(" << destination.x << "," << destination.y << ")"
            << " battery=" << battery.energy() << " duration=" << duration
            << " simTime=" << simTime() << endl;
}
void EnergyProtocolApp::finish() {
    for(auto& c:counters) recordScalar(c.first.c_str(),c.second);
    for(auto& s:samples) s.second.recordAs(s.first.c_str());
    recordScalar("finalEnergy",battery.energy());
    DemoBaseApplLayer::finish();
}
