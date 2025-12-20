#include "Simulator.h"

using namespace std;

Simulator::Simulator(const Config& cfg)
    : cfg(cfg)
    , server(cfg.mu)
    , stats()
    , rng(cfg.seed)
    , eventQueue()
    , buffer()
    , nextPacketId(0)
    , currentTime(0.0)
{
    stats.lastEventTime = 0.0;
    stats.numInSystem = 0;
}

void Simulator::scheduleArrival(double baseTime) {
    double interarrivalTime = rng.expRand(cfg.lambda);
    double arrivalTime = baseTime + interarrivalTime;
    
    Packet packet(nextPacketId++, 0, 12000);
    packet.arrivalTime = arrivalTime;
    
    Event arrivalEvent(arrivalTime, EventType::ARRIVAL, packet);
    eventQueue.push(arrivalEvent);
}

void Simulator::run() {
    scheduleArrival(0.0);
    
    while (!eventQueue.empty()) {
        Event ev = eventQueue.top();
        eventQueue.pop();
        
        currentTime = ev.time;
        
        if (currentTime > cfg.simEndTime) {
            break;
        }
        
        stats.updateNumInSystem(currentTime, stats.numInSystem);
        
        switch (ev.type) {
            case EventType::ARRIVAL:
                handleArrival(ev);
                break;
            case EventType::START_SERVICE:
                handleStartService(ev);
                break;
            case EventType::END_SERVICE:
                handleEndService(ev);
                break;
        }
    }
}

void Simulator::handleArrival(const Event& ev) {
    stats.recordArrival();
    
    scheduleArrival(currentTime);
    
    if (stats.numInSystem < cfg.bufferCapacity) {
        stats.recordAccepted();
        
        buffer.push_back(ev.packet);
        stats.numInSystem++;
        
        if (!server.busy) {
            Event startServiceEvent(currentTime, EventType::START_SERVICE);
            eventQueue.push(startServiceEvent);
        }
    } else {
        stats.recordDrop();
    }
}

void Simulator::handleStartService(const Event& ) {
    if (server.busy || buffer.empty()) {
        return;
    }
    
    Packet packet = buffer.front();
    buffer.pop_front();
    
    server.busy = true;
    server.current = packet;
    
    server.current.startServiceTime = currentTime;
    
    double serviceTime = rng.expRand(cfg.mu);
    double endServiceTime = currentTime + serviceTime;
    
    Event endServiceEvent(endServiceTime, EventType::END_SERVICE, server.current);
    eventQueue.push(endServiceEvent);
}

void Simulator::handleEndService(const Event& ev) {
    Packet packet = ev.packet;
    
    packet.endTime = currentTime;
    
    stats.recordPacketTimes(packet.arrivalTime, packet.startServiceTime, packet.endTime);
    
    stats.numInSystem--;
    
    server.busy = false;
    server.current = Packet();
    
    if (!buffer.empty()) {
        Event startServiceEvent(currentTime, EventType::START_SERVICE);
        eventQueue.push(startServiceEvent);
    }
}

const Stats& Simulator::getStats() const {
    return stats;
}
