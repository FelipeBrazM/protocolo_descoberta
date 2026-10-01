# Validação: parada no REQUEST e duração configurável

Compilação e nove cenários integrados aprovados, semente 0. As duas alterações
mantêm confirmação, destino fixo, mensagens, TTL, hops, deduplicação e energia.
Todos os arquivos `sumo/*.rou.xml` conservaram seus hashes nesta alteração.

| Cenário | Carros | Trucks | Atendidos | Energia fornecida/recebida (kWh) |
|---|---:|---:|---:|---:|
| BatteryMinimal | 1 | 1 | 1 | 20.000000 / 20.000000 |
| BatteryConfirmation | 1 | 2 | 1 | 20.000000 / 20.000000 |
| BatteryResponseTimeout | 1 | 1 | 0 | 0.000000 / 0.000000 |
| BatteryInsufficient | 1 | 1 | 0 | 0.000000 / 0.000000 |
| BatteryRateLimited | 1 | 1 | 1 | 40.000000 / 40.000000 |
| BatteryMultihop | 3 | 1 | 0 | 0.000000 / 0.000000 |
| BatteryHopLimit | 3 | 1 | 0 | 0.000000 / 0.000000 |
| BatteryExpiredFrames | 3 | 1 | 0 | 0.000000 / 0.000000 |
| BatteryUrban | 50 | 2 | 7 | 125.557899 / 125.557899 |

## Evidência mínima

- REQUEST e comando CAR_STOPPED: 2 s, posição `(175,2029.8)`.
- Destino do caminhão conserva o snapshot do REQUEST após confirmação.
- MEETING e início da carga no truck: 12 s, dentro de 20 m do snapshot.
- Conclusão no truck: 22 s; duração **10 s**, anteriormente **720 s**.
- Crédito e CAR_RESUMED: 22,004675879319 s.
- Vetores de mobilidade: velocidade zero e X constante em todas as amostras
  entre REQUEST e conclusão; movimento retomado na amostra de 23 s.
- Truck **500 → 480 kWh**, carro **20 → 40 kWh**.
- Pedido de 40 kWh também concluído em 10 s; a energia não determina a duração.

Com dois trucks, apenas um foi confirmado e transferiu 20 kWh. O outro
retornou a AVAILABLE após o timeout de confirmação de 5 s.
O teste de energia insuficiente confirmou expiração e liberação dos carros.
A matriz também valida balanço de energia, exclusividade do atendimento,
3 hops no teste multihop e bloqueio por TTL/hopLimit nos casos negativos.

## Urbano

50 carros e 2 trucks; 50 pedidos, 7 atendimentos em 1800 s, com
125,557899476635 kWh fornecidos e recebidos. Os trucks completaram 5 e 2
atendimentos, respectivamente. Todas as cargas concluídas duraram 10 s.

`setSpeed(0)` mantém o controle de frenagem no SUMO. Carros que estavam rápidos
podem avançar até parar: o maior deslocamento entre snapshot e MEETING entre
os sete atendidos foi 20,616 m. O destino e o critério de MEETING continuam
referenciando o snapshot, conforme solicitado; não há reposicionamento ou
perseguição. A parada exatamente no snapshot foi comprovada no cenário mínimo.

## Reprodução

Na raiz do projeto:

```bash
opp_env run -w .. --no-build -c 'make -j$(nproc)'
opp_env run -w .. --no-build -c 'python3 scripts/run_battery.py BatteryMinimal'
opp_env run -w .. --no-build -c 'python3 scripts/run_battery.py BatteryConfirmation'
opp_env run -w .. --no-build -c 'python3 scripts/run_battery.py BatteryUrban'
opp_env run -w .. --no-build -c 'python3 scripts/test_battery.py 0'
```

Logs, scalars e vetores da matriz: `results/battery/<cenário>-seed0.*`.
Resumo: [battery-test-results.json](battery-test-results.json).

Testes adicionais aprovados:

- Sem resposta válida, `requestLifetime=10s`: expiração e CAR_RESUMED em 12 s;
  vetores confirmaram retomada do movimento, sem transferência.
- `chargingDuration=3s`: duração observada de 3 s, mantendo transferência de 20 kWh.

```bash
opp_env run -w .. --no-build -c 'python3 scripts/run_battery.py BatteryResponseTimeout --*.node[*].appl.requestLifetime=10s --output-scalar-file=results/battery/stop-timeout.sca --output-vector-file=results/battery/stop-timeout.vec > results/battery/stop-timeout.log 2>&1'
opp_env run -w .. --no-build -c 'python3 scripts/run_battery.py BatteryMinimal --*.truck[*].appl.chargingDuration=3s --sim-time-limit=40s --output-scalar-file=results/battery/duration-three.sca --output-vector-file=results/battery/duration-three.vec > results/battery/duration-three.log 2>&1'
```
