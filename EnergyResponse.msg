import veins.modules.messages.BaseFrame1609_4;

namespace veins;

packet EnergyResponse extends BaseFrame1609_4 {
    int requestId;
    int originAddress;
    int responderAddress;

    int nextHopAddress;

    int hopCount = 0;

    double energyAvailable;
    double batteryLevel;
}