#pragma once

#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "EnergyRequest_m.h"

#include <set>
#include <utility>

namespace veins {

class EnergyRequestStats;

class EnergyRequestApp : public DemoBaseApplLayer
{
public:

    void initialize(int stage) override;

    ~EnergyRequestApp() override;


protected:

    void onWSM(
        BaseFrame1609_4* wsm
    ) override;


    void handleSelfMsg(
        cMessage* msg
    ) override;


    void finish() override;


private:

    // ========================================================
    // EVENTO DE ENVIO
    // ========================================================

    cMessage* sendRequestEvt = nullptr;


    // ========================================================
    // REQUESTS JÁ PROCESSADAS
    //
    // Chave:
    //
    // (originAddress, requestId)
    // ========================================================

    std::set<std::pair<int, int>> receivedRequests;


    // ========================================================
    // ESTATÍSTICAS LOCAIS
    // ========================================================

    int receivedRequestsCount = 0;

    int forwardedRequests = 0;

    int duplicateRequests = 0;

    int hopLimitDrops = 0;


    // ========================================================
    // ESTATÍSTICAS GLOBAIS
    // ========================================================

    EnergyRequestStats* getStats();


    // ========================================================
    // CONTROLE DE DUPLICATAS
    // ========================================================

    bool alreadyReceived(
        int originAddress,
        int requestId
    ) const;


    // ========================================================
    // CONTROLE DE HOPS
    // ========================================================

    bool canForward(
        int hopCount
    ) const;


    // ========================================================
    // RETRANSMISSÃO
    // ========================================================

    void forwardRequest(
        const EnergyRequest* request
    );
};

} // namespace veins