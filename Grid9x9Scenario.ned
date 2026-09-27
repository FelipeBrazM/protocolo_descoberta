import org.car2x.veins.nodes.Scenario;
import org.car2x.veins.nodes.Car;

simple EnergyRequestStats
{
    @class(veins::EnergyRequestStats);
}

network Grid9x9Scenario extends Scenario
{
    submodules:
        truck[0]: Car;
        stats: EnergyRequestStats;
}