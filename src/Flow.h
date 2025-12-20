#ifndef FLOW_H
#define FLOW_H

using namespace std;

struct Flow {
    int id;
    double lambda;
    
    Flow()
        : id(0)
        , lambda(0.0)
    {}
    
    Flow(int id, double lambda)
        : id(id)
        , lambda(lambda)
    {}
};

#endif // FLOW_H
