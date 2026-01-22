#include "Simulator.h"

using namespace std;

Simulator::Simulator(const Config& cfg)
    : cfg(cfg)
    , servers()
    , stats()
    , rng(cfg.seed)
    , eventQueue()
    , buffers()
    , numInSystemPerQueue()
    , rrNextQueue(0)
    , nextPacketId(0)
    , currentTime(0.0)
{
    stats.lastEventTime = 0.0;
    stats.numInSystem = 0;

    if (this->cfg.numQueues <= 0) {
        this->cfg.numQueues = 5;
    }

    servers.assign(this->cfg.numQueues, Server(this->cfg.mu));
    buffers.assign(this->cfg.numQueues, deque<Packet>());
    numInSystemPerQueue.assign(this->cfg.numQueues, 0);
}

void Simulator::scheduleArrival(double baseTime) {
    double interarrivalTime = rng.expRand(cfg.lambda);
    double arrivalTime = baseTime + interarrivalTime;
    
    Packet packet(nextPacketId++, 0, 12000);
    packet.arrivalTime = arrivalTime;
    
    int q = rrNextQueue;
    rrNextQueue = (rrNextQueue + 1) % cfg.numQueues;
    Event arrivalEvent(arrivalTime, EventType::ARRIVAL, q, packet);
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
    
    scheduleArrival(currentTime);//aa timej hu navu packet generte karavu chhu
    
    int q = ev.queueIndex;
    if (q < 0 || q >= cfg.numQueues) {
        return;
    }

    if (numInSystemPerQueue[q] < cfg.bufferCapacity) {
        stats.recordAccepted();
        
        buffers[q].push_back(ev.packet);
        numInSystemPerQueue[q]++;
        stats.numInSystem++;
        
        if (!servers[q].busy) {
            Event startServiceEvent(currentTime, EventType::START_SERVICE, q);
            eventQueue.push(startServiceEvent);
        }
    } else {
        stats.recordDrop();
    }
}

void Simulator::handleStartService(const Event& ev) {
    int q = ev.queueIndex;
    if (q < 0 || q >= cfg.numQueues) {
        return;
    }

    Server& server = servers[q];
    deque<Packet>& buffer = buffers[q];

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
    
    Event endServiceEvent(endServiceTime, EventType::END_SERVICE, q, server.current);
    eventQueue.push(endServiceEvent);
}

void Simulator::handleEndService(const Event& ev) {
    int q = ev.queueIndex;
    if (q < 0 || q >= cfg.numQueues) {
        return;
    }

    Packet packet = ev.packet;
    
    packet.endTime = currentTime;
    
    stats.recordPacketTimes(packet.arrivalTime, packet.startServiceTime, packet.endTime);
    
    numInSystemPerQueue[q]--;
    stats.numInSystem--;
    
    servers[q].busy = false;
    servers[q].current = Packet();
    
    if (!buffers[q].empty()) {
        Event startServiceEvent(currentTime, EventType::START_SERVICE, q);
        eventQueue.push(startServiceEvent);
    }
}

const Stats& Simulator::getStats() const {
    return stats;
}
