#ifndef CONFIG_H
#define CONFIG_H

using namespace std;

struct Config {
    double lambda;
    double mu;
    int bufferCapacity; // K, includes packet in service
    int numQueues;
    int numFlows;
    double simEndTime;
    unsigned int seed;
};

#endif // CONFIG_H
