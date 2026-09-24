#include "EnergyRequestApp.h"
#include "EnergyRequest_m.h"
#include "EnergyRequestStats.h"

using namespace veins;

Define_Module(EnergyRequestApp);


EnergyRequestApp::~EnergyRequestApp()
{
    cancelAndDelete(sendRequestEvt);
}


// ============================================================
// INITIALIZE
// ============================================================

void EnergyRequestApp::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);

    if (stage != 0) {
        return;
    }

    sendRequestEvt =
        new cMessage("sendEnergyRequest");


    int nodeIndex =
        getParentModule()->getIndex();


    EV_INFO
        << "EnergyRequestApp inicializada"
        << " | node=" << nodeIndex
        << " | myId=" << myId
        << endl;


    /*
     * Somente o node[0] cria a request inicial.
     */

    if (nodeIndex == 0) {

        scheduleAt(
            5,
            sendRequestEvt
        );
    }
}


// ============================================================
// SELF MESSAGE
// ============================================================

void EnergyRequestApp::handleSelfMsg(cMessage* msg)
{
    if (msg != sendRequestEvt) {

        DemoBaseApplLayer::handleSelfMsg(msg);

        return;
    }


    /*
     * ========================================================
     * REQUEST ORIGINAL
     * ========================================================
     */

    EnergyRequest* request =
        new EnergyRequest("EnergyRequest");


    populateWSM(request);


    /*
     * Identificação da requisição.
     */

    request->setRequestId(1);

    request->setOriginAddress(myId);


    /*
     * A transmissão original começa em hop 0.
     */

    request->setHopCount(0);


    /*
     * A própria origem também conhece essa request.
     *
     * Isso evita que ela processe novamente a mesma
     * requisição quando ela retornar por outro caminho.
     */

    receivedRequests.insert(
        std::make_pair(
            myId,
            request->getRequestId()
        )
    );


    /*
     * Estatística global.
     */

    getStats()->recordTransmission(
        simTime()
    );


    /*
     * Log.
     */

    EV_INFO
        << "REQUEST_SENT"
        << " | node="
        << getParentModule()->getIndex()
        << " | vehicleId="
        << myId
        << " | origin="
        << request->getOriginAddress()
        << " | requestId="
        << request->getRequestId()
        << " | hop="
        << request->getHopCount()
        << " | t="
        << simTime()
        << endl;


    /*
     * Broadcast através do Veins.
     */

    sendDown(request);
}


// ============================================================
// RECEBIMENTO DE WSM
// ============================================================

void EnergyRequestApp::onWSM(BaseFrame1609_4* wsm)
{
    /*
     * A infraestrutura agora trabalha somente
     * com EnergyRequest.
     */

    EnergyRequest* request =
        dynamic_cast<EnergyRequest*>(wsm);


    /*
     * Se não for uma EnergyRequest, ignoramos.
     *
     * Isso deixa a aplicação preparada para futuras
     * mensagens sem fazer cast inválido.
     */

    if (request == nullptr) {

        EV_INFO
            << "WSM_IGNORADO"
            << " | node="
            << getParentModule()->getIndex()
            << " | myId="
            << myId
            << " | t="
            << simTime()
            << endl;

        return;
    }


    int nodeIndex =
        getParentModule()->getIndex();


    int requestId =
        request->getRequestId();


    int originAddress =
        request->getOriginAddress();


    int hopCount =
        request->getHopCount();


    /*
     * ========================================================
     * IDENTIFICAÇÃO
     * ========================================================
     */

    std::pair<int, int> requestKey =
        std::make_pair(
            originAddress,
            requestId
        );


    /*
     * ========================================================
     * LOG DE RECEPÇÃO
     * ========================================================
     */

    EV_INFO
        << "REQUEST_RECEIVED"
        << " | node="
        << nodeIndex
        << " | vehicleId="
        << myId
        << " | origin="
        << originAddress
        << " | requestId="
        << requestId
        << " | hop="
        << hopCount
        << " | t="
        << simTime()
        << endl;


    /*
     * ========================================================
     * DUPLICATA
     * ========================================================
     */

    if (alreadyReceived(
            originAddress,
            requestId)) {

        duplicateRequests++;

        getStats()->recordDuplicate();


        EV_INFO
            << "REQUEST_DROPPED_DUPLICATE"
            << " | node="
            << nodeIndex
            << " | vehicleId="
            << myId
            << " | origin="
            << originAddress
            << " | requestId="
            << requestId
            << " | hop="
            << hopCount
            << " | t="
            << simTime()
            << endl;


        return;
    }


    /*
     * Marca a request como processada ANTES
     * de qualquer retransmissão.
     */

    receivedRequests.insert(
        requestKey
    );


    receivedRequestsCount++;


    /*
     * Estatística global.
     */

    getStats()->recordReception(
        myId,
        hopCount,
        simTime()
    );


    /*
     * ========================================================
     * LIMITE DE HOPS
     * ========================================================
     *
     * hop 0 -> pode virar hop 1
     * hop 1 -> pode virar hop 2
     * hop 2 -> NÃO retransmite
     */

    if (!canForward(hopCount)) {

        hopLimitDrops++;


        EV_INFO
            << "REQUEST_DROPPED_HOP_LIMIT"
            << " | node="
            << nodeIndex
            << " | vehicleId="
            << myId
            << " | origin="
            << originAddress
            << " | requestId="
            << requestId
            << " | hop="
            << hopCount
            << " | t="
            << simTime()
            << endl;


        return;
    }


    /*
     * ========================================================
     * RETRANSMISSÃO
     * ========================================================
     */

    forwardRequest(request);
}


// ============================================================
// VERIFICA DUPLICATA
// ============================================================

bool EnergyRequestApp::alreadyReceived(
    int originAddress,
    int requestId
) const
{
    return receivedRequests.find(
        std::make_pair(
            originAddress,
            requestId
        )
    ) != receivedRequests.end();
}


// ============================================================
// VERIFICA SE PODE RETRANSMITIR
// ============================================================

bool EnergyRequestApp::canForward(
    int hopCount
) const
{
    /*
     * O máximo é hop 2.
     *
     * Portanto:
     *
     * hop 0 -> encaminha
     * hop 1 -> encaminha
     * hop 2 -> para
     */

    return hopCount < 2;
}


// ============================================================
// FORWARD
// ============================================================

void EnergyRequestApp::forwardRequest(
    const EnergyRequest* request
)
{
    int nodeIndex =
        getParentModule()->getIndex();


    int oldHop =
        request->getHopCount();


    int newHop =
        oldHop + 1;


    EnergyRequest* forwarded =
        request->dup();


    forwarded->setHopCount(
        newHop
    );


    forwardedRequests++;


    getStats()->recordRetransmission();


    EV_INFO
        << "REQUEST_FORWARDED"
        << " | node="
        << nodeIndex
        << " | vehicleId="
        << myId
        << " | origin="
        << request->getOriginAddress()
        << " | requestId="
        << request->getRequestId()
        << " | hop="
        << newHop
        << " | t="
        << simTime()
        << endl;


    sendDown(forwarded);
}


// ============================================================
// FINISH
// ============================================================

void EnergyRequestApp::finish()
{
    DemoBaseApplLayer::finish();


    int nodeIndex =
        getParentModule()->getIndex();


    /*
     * Somente o node[0] mostra as estatísticas locais.
     */

    if (nodeIndex != 0) {
        return;
    }


    EV_INFO << "\n";
    EV_INFO
        << "========================================\n";

    EV_INFO
        << "ENERGY REQUEST - RESULTADO\n";

    EV_INFO
        << "========================================\n";


    EV_INFO
        << "Requests recebidas="
        << receivedRequestsCount
        << "\n";


    EV_INFO
        << "Requests encaminhadas="
        << forwardedRequests
        << "\n";


    EV_INFO
        << "Duplicatas="
        << duplicateRequests
        << "\n";


    EV_INFO
        << "Drops por limite de hop="
        << hopLimitDrops
        << "\n";


    EV_INFO
        << "========================================\n";


    recordScalar(
        "EnergyRequest - Requests recebidas",
        receivedRequestsCount
    );


    recordScalar(
        "EnergyRequest - Requests encaminhadas",
        forwardedRequests
    );


    recordScalar(
        "EnergyRequest - Duplicatas",
        duplicateRequests
    );


    recordScalar(
        "EnergyRequest - Drops por limite de hop",
        hopLimitDrops
    );
}


// ============================================================
// STATS
// ============================================================

EnergyRequestStats*
EnergyRequestApp::getStats()
{
    return check_and_cast<EnergyRequestStats*>(
        getParentModule()
            ->getParentModule()
            ->getSubmodule("stats")
    );
}