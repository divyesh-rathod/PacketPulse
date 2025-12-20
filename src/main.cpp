#include <iostream>
#include <iomanip>

#include "Config.h"
#include "Simulator.h"

using namespace std;

int main() {
    Config cfg;
    cfg.lambda = 80.0;
    cfg.mu = 100.0;
    cfg.bufferCapacity = 50;
    cfg.numFlows = 1;
    cfg.simEndTime = 1000.0;
    cfg.seed = 12345;
    
    cout << "========================================" << endl;
    cout << "   M/M/1/K Queue Network Simulator" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    cout << "Configuration:" << endl;
    cout << "  Arrival rate (lambda): " << cfg.lambda << " packets/unit time" << endl;
    cout << "  Service rate (mu):     " << cfg.mu << " packets/unit time" << endl;
    cout << "  Buffer capacity (K):   " << cfg.bufferCapacity << endl;
    cout << "  Offered load (rho):    " << (cfg.lambda / cfg.mu) << endl;
    cout << "  Simulation end time:   " << cfg.simEndTime << endl;
    cout << "  Random seed:           " << cfg.seed << endl;
    cout << endl;
    
    Simulator sim(cfg);
    sim.run();
    
    const Stats& st = sim.getStats();
    
    double dropProbability = st.getDropProbability();
    double lambdaEffective = cfg.lambda * (1.0 - dropProbability);
    double avgDelay = st.getAverageDelay();
    double avgWaiting = st.getAverageWaiting();
    double avgNumInSystem = st.getAverageNumInSystem();
    
    cout << "========================================" << endl;
    cout << "          Simulation Results" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    cout << "Packet Statistics:" << endl;
    cout << "  Total arrivals:     " << st.totalArrivals << endl;
    cout << "  Packets accepted:   " << st.totalAccepted << endl;
    cout << "  Packets dropped:    " << st.totalDropped << endl;
    cout << endl;
    
    cout << "Performance Metrics:" << endl;
    cout << fixed << setprecision(6);
    cout << "  Drop probability:           " << dropProbability << endl;
    cout << "  Effective arrival rate:     " << lambdaEffective << " packets/unit time" << endl;
    cout << "  Average delay (system time):" << avgDelay << " time units" << endl;
    cout << "  Average waiting time:       " << avgWaiting << " time units" << endl;
    cout << "  Average number in system:   " << avgNumInSystem << endl;
    cout << endl;
    
    double rho = cfg.lambda / cfg.mu;
    cout << "Theoretical M/M/1 (infinite buffer) for comparison:" << endl;
    cout << "  Utilization (rho):          " << rho << endl;
    if (rho < 1.0) {
        double theoreticalAvgN = rho / (1.0 - rho);
        double theoreticalAvgDelay = 1.0 / (cfg.mu - cfg.lambda);
        cout << "  Avg number in system:       " << theoreticalAvgN << endl;
        cout << "  Avg delay (W):              " << theoreticalAvgDelay << endl;
    } else {
        cout << "  System is unstable (rho >= 1)" << endl;
    }
    cout << endl;
    
    cout << "========================================" << endl;
    cout << "        Simulation Complete" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
