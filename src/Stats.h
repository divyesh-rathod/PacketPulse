#ifndef STATS_H
#define STATS_H

using namespace std;

struct Stats {
    long totalArrivals;
    long totalAccepted;
    long totalDropped;
    long totalServed;
    
    double sumDelay;
    double sumWaiting;
    
    double lastEventTime;
    double areaNumInSystem;
    int numInSystem;
    
    Stats()
        : totalArrivals(0)
        , totalAccepted(0)
        , totalDropped(0)
        , totalServed(0)
        , sumDelay(0.0)
        , sumWaiting(0.0)
        , lastEventTime(0.0)
        , areaNumInSystem(0.0)
        , numInSystem(0)
    {}
    
    void recordArrival() {
        totalArrivals++;
    }
    
    void recordDrop() {
        totalDropped++;
    }
    
    void recordAccepted() {
        totalAccepted++;
    }
    
    void recordPacketTimes(double arrival, double startService, double endTime) {
        double delay = endTime - arrival;
        double waiting = startService - arrival;
        sumDelay += delay;
        sumWaiting += waiting;
        totalServed++;
    }
    
    // Call before changing numInSystem.
    void updateNumInSystem(double currentTime, int currentN) {
        double timeDelta = currentTime - lastEventTime;
        areaNumInSystem += currentN * timeDelta;
        lastEventTime = currentTime;
    }
    
    double getAverageDelay() const {
        return (totalServed > 0) ? sumDelay / totalServed : 0.0;
    }
    
    double getAverageWaiting() const {
        return (totalServed > 0) ? sumWaiting / totalServed : 0.0;
    }
    
    double getAverageNumInSystem() const {
        return (lastEventTime > 0) ? areaNumInSystem / lastEventTime : 0.0;
    }
    
    double getDropProbability() const {
        return (totalArrivals > 0) ? static_cast<double>(totalDropped) / totalArrivals : 0.0;
    }
};

#endif // STATS_H
