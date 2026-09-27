import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;
simple EnergyProtocolApp extends DemoBaseApplLayer
{
    parameters:
        double batteryCapacity = default(80); // kWh
        double initialBattery = default(-1); // -1 draws from the range below
        double initialBatteryMin = default(16); // kWh
        double initialBatteryMax = default(40); // kWh
        double lowBatteryThreshold = default(28); // kWh
        double targetBatteryPercent = default(50);
        double consumptionRate = default(0.001); // kWh/s, abstract load
        double tickInterval @unit(s) = default(1s);
        double requestInterval @unit(s) = default(30s);
        double requestLifetime @unit(s) = default(600s); // discovery and travel, excludes charging
        double messageTtl @unit(s) = default(5s);
        int maxHops = default(8);
        double forwardJitter @unit(s) = default(20ms);
        double meetingDistance @unit(m) = default(20m);
        double responseTimeout @unit(s) = default(5s);
        double chargingPowerKW = default(100);
        bool logNetworkDetails = default(false);
        sendBeacons = default(false);
        dataLengthBits = default(2048bit);
        @signal[batteryState](type=long);
        @signal[energyReceived](type=double);
        @signal[energySupplied](type=double);
        @signal[meetingDistanceObserved](type=double);
        @statistic[batteryState](source=batteryState;record=vector);
        @statistic[energyReceived](source=energyReceived;record=vector,sum);
        @statistic[energySupplied](source=energySupplied;record=vector,sum);
        @statistic[meetingDistanceObserved](source=meetingDistanceObserved;record=vector,stats);
}
