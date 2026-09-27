#pragma once
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "EnergyRequest_m.h"
#include "BatteryState.h"
#include "MessageCache.h"
#include <map>
#include <set>
#include <string>
namespace veins {
enum class EnergyMessage { Request=1, TruckResponse, Meeting, Transfer, Confirmation };
class EnergyProtocolApp : public DemoBaseApplLayer {
public:
    void initialize(int stage) override;
    ~EnergyProtocolApp() override;
protected:
    BatteryState battery;
    cMessage* tick = nullptr;
    long sequence = 0;
    MessageCache<simtime_t> cache;
    std::map<std::string,double> counters;
    std::map<std::string,cStdDev> samples;
    simtime_t lastTick;
    std::set<cMessage*> pending;
    virtual void protocolTick(double dt) = 0;
    virtual void receiveProtocol(const EnergyRequest& frame) = 0;
    void handleSelfMsg(cMessage* msg) override;
    void onWSM(BaseFrame1609_4* wsm) override;
    void finish() override;
    EnergyRequest* frame(EnergyMessage type, int destination=-1);
    void transmit(EnergyRequest* msg);
    void count(const char* name, double amount=1);
    void sample(const char* name, double value);
    void logEvent(const char* event, int requester, int request, int truck,
                  double energy, const Coord& destination, double duration = 0) const;
};
}
