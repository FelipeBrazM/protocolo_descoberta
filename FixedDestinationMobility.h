#pragma once
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include <algorithm>
#include <list>
#include <string>

namespace veins {
// Mobility adapter: only our own vehicle and the coordinate in REQUEST.
// Routing is delegated to SUMO; no other vehicle is queried.
class FixedDestinationMobility {
    TraCICommandInterface* traci = nullptr;
    TraCICommandInterface::Vehicle* vehicle = nullptr;
    std::list<std::string> patrol;
    std::string destinationEdge;
    double destinationPosition = 0;
    bool routePending = false;
    bool returningToPatrol = false;
public:
    void initialize(TraCICommandInterface* api, TraCICommandInterface::Vehicle* ownVehicle) {
        traci = api;
        vehicle = ownVehicle;
        patrol = vehicle->getPlannedRoadIds();
    }
    void goTo(const Coord& destination) {
        const auto road = traci->getRoadMapPos(destination);
        destinationEdge = std::get<0>(road);
        destinationPosition = std::get<1>(road);
        routePending = true;
        returningToPatrol = false;
        vehicle->setSpeed(-1);
        update();
    }
    void update() {
        if (!routePending && !returningToPatrol) return;
        const auto road = vehicle->getRoadId();
        if (road.empty() || road.front() == ':') return;
        if (routePending) {
            // A point behind us on a one-way road requires leaving that road
            // before SUMO can compute a route back. Never teleport backwards.
            if (road == destinationEdge && vehicle->getLanePosition() > destinationPosition) return;
            vehicle->changeTarget(destinationEdge);
            routePending = false;
        }
        if (returningToPatrol) {
            const auto it = std::find(patrol.begin(), patrol.end(), road);
            if (it != patrol.end()) {
                vehicle->changeVehicleRoute(std::list<std::string>(it, patrol.end()));
                returningToPatrol = false;
            }
        }
    }
    void hold() {
        routePending = false;
        vehicle->setSpeed(0); // no lane selection or parking manoeuvre
    }
    void resumePatrol() {
        routePending = false;
        vehicle->setSpeed(-1);
        returningToPatrol = true;
        const auto road = vehicle->getRoadId();
        if (std::find(patrol.begin(), patrol.end(), road) == patrol.end())
            vehicle->changeTarget(patrol.front());
        update();
    }
};
}
