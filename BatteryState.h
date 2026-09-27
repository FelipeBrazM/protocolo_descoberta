#pragma once
#include <algorithm>
#include <stdexcept>
namespace veins {
// Energy is expressed in kWh. No simulator or network dependencies.
class BatteryState {
    double capacity_ = 0, energy_ = 0;
public:
    void initialize(double capacity, double energy) {
        if (!(capacity > 0) || !(energy >= 0 && energy <= capacity))
            throw std::invalid_argument("Invalid battery capacity/initial energy");
        capacity_ = capacity; energy_ = energy;
    }
    double energy() const { return energy_; }
    double room() const { return capacity_ - energy_; }
    double consume(double amount) { double d=std::min(energy_,std::max(0.0,amount)); energy_-=d; return d; }
    double charge(double amount) { double d=std::min(room(),std::max(0.0,amount)); energy_+=d; return d; }
};
}
