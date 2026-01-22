# Network_Simulator (PacketPulse)

A discrete-event simulator for an **M/M/1/K queue network**.

- **Arrivals** follow a Poisson process with rate **λ** (exponential inter-arrival times).
- **Service times** are exponential with rate **μ**.
- Each queue has a finite **buffer capacity K** (including the job in service).
- Multiple queues are modeled as parallel servers; each arrival is assigned to a queue via **round-robin**.

This project prints basic performance metrics such as drop probability, average delay, and average waiting time.

## Project Layout

- [src/main.cpp](src/main.cpp) – simulation configuration and result reporting
- [src/Simulator.cpp](src/Simulator.cpp), [src/Simulator.h](src/Simulator.h) – discrete-event engine
- [src/Stats.h](src/Stats.h) – statistics collection
- [src/Random.h](src/Random.h) – random/exponential sampling utilities
- [src/Event.h](src/Event.h) – event definitions and priority ordering
- [src/Server.h](src/Server.h) – server state
- [src/Packet.h](src/Packet.h) – packet/job representation

## Build

There is no build system (CMake/VS solution) included; compile the sources directly.

### Option A: MinGW-w64 / g++ (Windows)

From the repository root:

```bash
g++ -std=c++17 -O2 -I./src \
  ./src/main.cpp ./src/Simulator.cpp \
  -o network_sim.exe
```

Run:

```bash
./network_sim.exe
```

### Option B: MSVC (Developer Command Prompt)

From the repository root:

```bat
cl /std:c++17 /O2 /EHsc /I src src\main.cpp src\Simulator.cpp /Fe:network_sim.exe
```

Run:

```bat
network_sim.exe
```

## Configure a Simulation

Edit the `Config` values in [src/main.cpp](src/main.cpp):

- `lambda` – arrival rate (λ)
- `mu` – service rate (μ)
- `bufferCapacity` – finite capacity **K** (includes the packet in service)
- `numQueues` – number of parallel queues/servers
- `simEndTime` – simulation stop time
- `seed` – RNG seed for reproducibility

## Output Metrics

The program reports:

- Total arrivals / accepted / dropped / served
- Drop probability: $P_{drop} = \frac{\text{dropped}}{\text{arrivals}}$
- Effective arrival rate: $\lambda_{eff} = \lambda(1 - P_{drop})$
- Average delay (time in system)
- Average waiting time (time before service)
- Average number in system (time-average via area under $N(t)$)

It also prints a simple theoretical reference for an *infinite-buffer* parallel M/M/1 model (for comparison only).

## Notes / Assumptions

- Arrivals are assigned to queues using a round-robin policy.
- Each queue is FIFO.
- Capacity is enforced per queue based on “number in system” at that queue.

## License

See [LICENSE](LICENSE).
