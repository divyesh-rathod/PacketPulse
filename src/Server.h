#ifndef SERVER_H
#define SERVER_H

#include "Packet.h"

using namespace std;

struct Server {
    bool busy;
    Packet current;
    double mu;
    double busyTime;
    
    Server()
        : busy(false)
        , current()
        , mu(0.0)
        , busyTime(0.0)
    {}
    
    Server(double mu)
        : busy(false)
        , current()
        , mu(mu)
        , busyTime(0.0)
    {}
    
    void reset() {
        busy = false;
        current = Packet();
    }
};

#endif // SERVER_H
