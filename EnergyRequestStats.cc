#include "EnergyRequestStats.h"

using namespace omnetpp;

namespace veins {

Define_Module(EnergyRequestStats);


void EnergyRequestStats::initialize()
{
    hop0Received = 0;
    hop1Received = 0;
    retransmissions = 0;
    duplicates = 0;

    firstTransmissionTime = -1;
    lastReceptionTime = -1;

    vehiclesHop0.clear();
    vehiclesHop1.clear();
}


void EnergyRequestStats::recordTransmission(simtime_t time)
{
    /*
     * Guarda somente o instante da primeira
     * transmissão da requisição.
     */
    if (firstTransmissionTime < SIMTIME_ZERO) {
        firstTransmissionTime = time;
    }
}


void EnergyRequestStats::recordReception(
    int vehicleId,
    int hop,
    simtime_t time)
{
    /*
     * Mantém as estatísticas de recepções
     * que já tínhamos.
     */
    if (hop == 0) {

        hop0Received++;

        /*
         * Guarda o ID do veículo.
         *
         * std::set evita duplicação.
         */
        vehiclesHop0.insert(vehicleId);
    }

    else if (hop == 1) {

        hop1Received++;

        /*
         * Guarda o ID do veículo.
         */
        vehiclesHop1.insert(vehicleId);
    }


    /*
     * Atualiza a última recepção.
     */
    if (time > lastReceptionTime) {
        lastReceptionTime = time;
    }
}


void EnergyRequestStats::recordRetransmission()
{
    retransmissions++;
}


void EnergyRequestStats::recordDuplicate()
{
    duplicates++;
}


void EnergyRequestStats::finish()
{
    /*
     * Número de veículos distintos no hop 0.
     */
    int uniqueHop0 =
        static_cast<int>(vehiclesHop0.size());


    /*
     * Número de veículos distintos no hop 1.
     */
    int uniqueHop1 =
        static_cast<int>(vehiclesHop1.size());


    /*
     * Junta os veículos dos dois hops.
     *
     * Se um mesmo veículo apareceu nos dois
     * conjuntos, será contado somente uma vez.
     */
    std::set<int> uniqueVehicles =
        vehiclesHop0;

    uniqueVehicles.insert(
        vehiclesHop1.begin(),
        vehiclesHop1.end()
    );


    int totalUniqueVehicles =
        static_cast<int>(uniqueVehicles.size());


    /*
     * Mantém também a métrica antiga.
     *
     * Ela representa recepções contabilizadas,
     * não veículos únicos.
     */
    int totalReceived =
        hop0Received + hop1Received;


    /*
     * Calcula o tempo entre a primeira transmissão
     * e a última recepção.
     */
    simtime_t propagationTime =
        SIMTIME_ZERO;

    if (firstTransmissionTime >= SIMTIME_ZERO &&
        lastReceptionTime >= SIMTIME_ZERO) {

        propagationTime =
            lastReceptionTime -
            firstTransmissionTime;
    }


    /*
     * ==================================================
     * RESULTADO GLOBAL
     * ==================================================
     */

    EV_INFO << "\n";

    EV_INFO << "========================================\n";

    EV_INFO << "ENERGY REQUEST - RESULTADO GLOBAL\n";

    EV_INFO << "========================================\n";


    /*
     * Recepções.
     */
    EV_INFO << "Recepcoes hop 0="
            << hop0Received
            << "\n";

    EV_INFO << "Recepcoes hop 1="
            << hop1Received
            << "\n";


    /*
     * Veículos únicos.
     */
    EV_INFO << "Veiculos unicos hop 0="
            << uniqueHop0
            << "\n";

    EV_INFO << "Veiculos unicos hop 1="
            << uniqueHop1
            << "\n";

    EV_INFO << "Veiculos unicos alcancados="
            << totalUniqueVehicles
            << "\n";


    /*
     * Métrica antiga.
     */
    EV_INFO << "Total de recepcoes="
            << totalReceived
            << "\n";


    /*
     * Retransmissões e duplicatas.
     */
    EV_INFO << "Retransmissoes="
            << retransmissions
            << "\n";

    EV_INFO << "Duplicatas="
            << duplicates
            << "\n";


    /*
     * Tempo.
     */
    EV_INFO << "Primeira transmissao="
            << firstTransmissionTime
            << "\n";

    EV_INFO << "Ultima recepcao="
            << lastReceptionTime
            << "\n";

    EV_INFO << "Tempo de propagacao="
            << propagationTime
            << "\n";


    EV_INFO << "========================================\n";


    /*
     * ==================================================
     * RESULTADOS DO OMNET++
     * ==================================================
     */

    recordScalar(
        "EnergyRequest - Recepcoes hop 0",
        hop0Received
    );

    recordScalar(
        "EnergyRequest - Recepcoes hop 1",
        hop1Received
    );

    recordScalar(
        "EnergyRequest - Veiculos unicos hop 0",
        uniqueHop0
    );

    recordScalar(
        "EnergyRequest - Veiculos unicos hop 1",
        uniqueHop1
    );

    recordScalar(
        "EnergyRequest - Veiculos unicos alcancados",
        totalUniqueVehicles
    );

    recordScalar(
        "EnergyRequest - Total de recepcoes",
        totalReceived
    );

    recordScalar(
        "EnergyRequest - Retransmissoes",
        retransmissions
    );

    recordScalar(
        "EnergyRequest - Duplicatas",
        duplicates
    );

    recordScalar(
        "EnergyRequest - Tempo propagacao",
        propagationTime
    );
}

} // namespace veins