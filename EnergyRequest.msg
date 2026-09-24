import veins.modules.messages.BaseFrame1609_4;

namespace veins;

packet EnergyRequest extends BaseFrame1609_4 {
    int requestId;
    int originAddress;

    int previousHopAddress;

    int hopCount = 0;

    double energyRequired;
    double minimumBattery;
}