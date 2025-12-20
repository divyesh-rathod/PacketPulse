#ifndef RANDOM_H
#define RANDOM_H

#include <random>

using namespace std;

class Random {
private:
    mt19937 generator;
    
public:
    Random(unsigned int seed)
        : generator(seed)
    {}
    
    double expRand(double rate) {
        exponential_distribution<double> dist(rate);
        return dist(generator);
    }
};

#endif // RANDOM_H
