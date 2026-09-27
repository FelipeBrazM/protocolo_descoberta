simple BatteryTruckApp extends EnergyProtocolApp
{
    parameters:
        @class(veins::BatteryTruckApp);
        batteryCapacity = default(500);
        initialBattery = default(500);
        consumptionRate = default(0);
}
