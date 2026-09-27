#include "BatteryState.h"
#include "MessageCache.h"
#include <cassert>
#include <cmath>
int main() {
    veins::BatteryState truck,car;truck.initialize(500,10);car.initialize(60,55);
    double amount=truck.consume(car.charge(8));
    assert(amount==5 && car.energy()==60 && truck.energy()==5);
    assert(truck.consume(100)==5 && truck.energy()==0);
    assert(car.consume(-1)==0 && car.charge(-1)==0);
    bool invalid=false;try{car.initialize(10,11);}catch(const std::invalid_argument&){invalid=true;}assert(invalid);
    veins::MessageCache<double> cache;
    assert(cache.insert(1,1,5));assert(!cache.insert(1,1,5));
    assert(cache.insert(2,1,5));assert(cache.insert(1,2,6));
    cache.expire(5);assert(cache.insert(1,1,9));assert(!cache.insert(1,2,6));
}
