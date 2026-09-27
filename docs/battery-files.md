# Arquivos da etapa de destino fixo

## Criados nesta etapa

- `FixedDestinationMobility.h`: adaptação de coordenadas e controle do próprio caminhão por TraCI.
- `sumo/battery_two_trucks.rou.xml`: duas rotas independentes para o urbano.
- `sumo/battery_confirmation_trucks.rou.xml`: dois caminhões para testar resposta/timeout.
- `sumo/battery_confirmation.sumocfg`.
- `sumo/battery_confirmation.launchd.xml`.

## Alterados nesta etapa

- `EnergyProtocolApp.h`, `EnergyProtocolApp.cc`, `EnergyProtocolApp.ned`: tipos do fluxo, bateria aleatória, parâmetros e logs; broadcast/cache/hops mantidos.
- `EnergyRequestApp.h`, `EnergyRequestApp.cc`: alvo de carga, snapshot fixo, confirmação, parada e crédito final.
- `BatteryTruckApp.h`, `BatteryTruckApp.cc`: cinco estados, confirmação/timeout, deslocamento e temporizador de carregamento.
- `EnergyRequest.msg`, `EnergyRequest_m.h`, `EnergyRequest_m.cc`: horários de início/fim da carga; arquivos `_m` regenerados.
- `omnetpp.ini`, `battery.ini`: parâmetros em kWh/kW, cenários e logs.
- `scripts/prepare_battery_scenarios.py`: geração dos dois caminhões e fixtures.
- `scripts/run_battery.py`: urbano como cenário padrão.
- `scripts/test_battery.py`: verificações do fluxo novo, tempo, energia, destino e V2V.
- `sumo/battery_minimal.rou.xml`, `sumo/battery_multiple.rou.xml`: carros lentos nos testes controlados.
- `sumo/battery_approach.sumocfg`, `sumo/battery_minimal.sumocfg`, `sumo/battery_multiple.sumocfg`, `sumo/battery_multihop.sumocfg`, `sumo/battery_urban.sumocfg`: duração e desativação do teletransporte automático.
- `sumo/battery_urban.launchd.xml`: inclusão do arquivo dos dois caminhões.
- `docs/battery-protocol.md`, `docs/battery-validation.md`, `docs/battery-test-results.json`, `docs/battery-files.md`: documentação e evidências atuais.

## Preservados

- `sumo/grid9x9_50.rou.xml`: todos os 50 veículos e suas rotas; hash verificado pelo teste.
- `sumo/grid9x9.net.xml`, `sumo/grid9x9.edg.xml`: grid original.
- `Grid9x9Scenario.ned`: já continha o vetor dinâmico de caminhões.
- `BatteryState.h`, `MessageCache.h`, `EnergyRequestStats.h/.cc`: infraestrutura existente.
- `Makefile`: já compila as aplicações e regenera a mensagem utilizada.

`EnergyResponse*` havia sido removido na limpeza anterior e não foi recriado;
a resposta usa o envelope existente. Saídas atuais ficam em `results/battery/`
e objetos em `out/`, ignorados pelo Git. Nenhum commit foi criado.
