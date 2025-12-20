#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <queue>
#include <vector>
#include <deque>

#include "Config.h"
#include "Event.h"
#include "Packet.h"
#include "Server.h"
#include "Stats.h"
#include "Random.h"

using namespace std;

class Simulator {
public:
    Simulator(const Config& cfg);
    
    void run();
    
    const Stats& getStats() const;
    
private:
    Config cfg;
    
    Server server;
    
    Stats stats;
    
    Random rng;
    
    priority_queue<Event, vector<Event>, greater<Event>> eventQueue;
    
    deque<Packet> buffer;
    
    int nextPacketId;
    
    double currentTime;
    
    void scheduleArrival(double baseTime);
    
    void handleArrival(const Event& ev);
    void handleStartService(const Event& ev);
    void handleEndService(const Event& ev);
};

#endif // SIMULATOR_H
