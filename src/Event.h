#ifndef EVENT_H
#define EVENT_H

#include "Packet.h"

using namespace std;

enum class EventType {
    ARRIVAL,
    START_SERVICE,
    END_SERVICE
};

struct Event {
    double time;
    EventType type;
    Packet packet;
    
    Event()
        : time(0.0)
        , type(EventType::ARRIVAL)
        , packet()
    {}
    
    Event(double t, EventType et, Packet pkt = Packet())
        : time(t)
        , type(et)
        , packet(pkt)
    {}
    
    bool operator>(const Event& other) const {
        return time > other.time;
    }
};

#endif // EVENT_H
