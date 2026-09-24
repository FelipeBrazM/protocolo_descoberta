import org.car2x.veins.nodes.Scenario;

simple EnergyRequestStats
{
    @class(veins::EnergyRequestStats);
}

network Grid9x9Scenario extends Scenario
{
    submodules:
        stats: EnergyRequestStats;
}