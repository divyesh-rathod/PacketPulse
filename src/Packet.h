#ifndef PACKET_H
#define PACKET_H

using namespace std;

struct Packet {
    int id;
    int flowId;
    int sizeBits;
    double arrivalTime;
    double startServiceTime;
    double endTime;
    bool valid;
    
    Packet()
        : id(-1)
        , flowId(0)
        , sizeBits(12000)
        , arrivalTime(0.0)
        , startServiceTime(0.0)
        , endTime(0.0)
        , valid(false)
    {}
    
    Packet(int id, int flowId, int sizeBits)
        : id(id)
        , flowId(flowId)
        , sizeBits(sizeBits)
        , arrivalTime(0.0)
        , startServiceTime(0.0)
        , endTime(0.0)
        , valid(true)
    {}
};

#endif // PACKET_H
