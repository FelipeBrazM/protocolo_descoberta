# Validação: confirmação e destino fixo

Esta evidência substitui a matriz anterior do protocolo de encontros oportunistas.

**9 cenários integrados passaram, semente 0 para OMNeT++ e SUMO.** Compilação: `make -j$(nproc)` dentro de `opp_env`.

| Cenário | Carros | Trucks | Atendidos | Energia fornecida/recebida (kWh) | Hops máximos do pedido |
|---|---:|---:|---:|---:|---:|
| BatteryMinimal | 1 | 1 | 1 | 20.000000 / 20.000000 | 1 |
| BatteryConfirmation | 1 | 2 | 1 | 20.000000 / 20.000000 | 1 |
| BatteryResponseTimeout | 1 | 1 | 0 | 0.000000 / 0.000000 | 1 |
| BatteryInsufficient | 1 | 1 | 0 | 0.000000 / 0.000000 | 1 |
| BatteryRateLimited | 1 | 1 | 1 | 40.000000 / 40.000000 | 1 |
| BatteryMultihop | 3 | 1 | 0 | 0.000000 / 0.000000 | 3 |
| BatteryHopLimit | 3 | 1 | 0 | 0.000000 / 0.000000 | 0 |
| BatteryExpiredFrames | 3 | 1 | 0 | 0.000000 / 0.000000 | 0 |
| BatteryUrban | 50 | 2 | 3 | 53.009294 / 53.009294 | 8 |

Verificações:

- 50 carros originais e 2 caminhões no urbano; 50 valores iniciais de bateria distintos na faixa 16–40 kWh.
- SHA256 das 50 rotas originais inalterado: `708e49803f9f2058fc02d8eb148bab44b3e1b0626165d2aa9467e46dcb8900b7`.
- SUMO independente por 30 s com FCD: 52 IDs observados, incluindo batteryTruck0 e batteryTruck1.
- Destino dos reanúncios e do caminhão permanece igual ao snapshot do primeiro REQUEST.
- Nenhum destino de serviço é definido antes da confirmação correspondente.
- O carro confirma somente um truck por pedido; nenhum truck carrega dois pedidos simultaneamente.
- Timeout de resposta exatamente 5 s; sem confirmação não há deslocamento de atendimento.
- Duração observada no caminhão = energia / 100 kW × 3600 s.
- Balanço individual de energia, ausência de saldo negativo e correspondência das métricas globais de rede.
- Entrega multihop por 3 hops; TTL curto e limite de 1 hop impedem entrega ao caminhão.

## Atendimento mínimo demonstrado

Semente 0: carro `ev0`, endereço 17; caminhão `batteryTruck0`, endereço 11; pedido `(17,1)`. Esses identificadores são resultado da execução, sem hardcode no protocolo.

| Evento | Tempo simulado |
|---|---:|
| REQUEST | 2 s |
| Destino definido após confirmação | 2,015034039321 s |
| MEETING e início de carga | 12 s |
| Fim da carga no caminhão / AVAILABLE | 732 s |
| Crédito recebido pelo carro | 732,004675879769 s |

Destino fixo em coordenadas Veins: `(175,2029.8)`. Truck chega a `(163.479,2026.6)`, dentro do limite de 20 m. Duração física abstrata: **720 s**, sem contabilizar a pequena latência de entrega do resultado.

Energia: caminhão **500 → 480 kWh**; carro **20 → 40 kWh**. O caso de 40 kWh leva **1440 s**.

No teste com dois trucks, ambos respondem ao pedido; somente um recebe confirmação e realiza a transferência. O outro retorna a AVAILABLE pelo timeout. Sem respostas válidas, ambos os lados mantêm contadores coerentes e não há débito.

## Experimento urbano

Em 1800 s simulados houve **57 pedidos e 3 atendimentos completos**, com **53,009293547452 kWh** debitados e recebidos. Outros serviços podem estar pendentes no limite da simulação; eles não são contados como concluídos.

**Interpretação:** MEETING mede proximidade da posição histórica recebida. O carro é parado ao receber MEETING onde estiver; pode estar distante do caminhão no urbano. O modelo atual não comprova rendezvous físico entre veículos móveis. A métrica `requesterDisplacementAtMeeting` registra esse desvio.

**Sem Receipt:** a igualdade de energia fornecida/recebida foi observada nestes testes; não é garantia de entrega em caso de perda da mensagem final. Não foi implementada confirmação, rollback ou recuperação de transação.

Reprodução:

```bash
opp_env run -w .. --no-build -c 'make -j$(nproc) && python3 scripts/test_battery.py 0'
grep -E 'originAddress=17 requestId=1' results/battery/BatteryMinimal-seed0.log
```

Logs completos e scalars: `results/battery/<configuração>-seed0.*`. Resumo preservado: [battery-test-results.json](battery-test-results.json).
