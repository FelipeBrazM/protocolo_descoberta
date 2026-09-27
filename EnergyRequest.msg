import veins.modules.messages.BaseFrame1609_4;
namespace veins;
packet EnergyRequest extends BaseFrame1609_4 {
    // Shared protocol envelope. originAddress identifies this frame's creator.
    int messageType = 1;
    long messageId;
    int destination = -1;
    int requesterId = -1;
    int truckId = -1;
    int hopLimit = 8;
    simtime_t createdAt;
    simtime_t expiresAt;
    simtime_t requestStarted;
    simtime_t requestDeadline;
    simtime_t chargingStartedAt;
    simtime_t chargingEndsAt;
    double energyAmount = 0;
    double velocityX = 0;
    double velocityY = 0;
    int requestHops = 0;
    int requestId;
    int originAddress;
    int previousHopAddress;
    int hopCount = 0;
    double energyRequired;
    double positionX;
    double positionY;
    double speed;
    double heading;
}
