#ifndef PHOTON_H
#define PHOTON_H

#include <iostream>
#include "punto_direccion.h"

using namespace std;

class photon{
public:
    point p; // position
    dir n;
    double R,G,B;; // power packed as 4 chars
    double flow;
    //dir phi, theta; // compressed incident direction
    int refr = 0;
};

bool operator ==(photon p, photon q){
    return p.p==q.p;
}

#endif