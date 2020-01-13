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
	short flag;
	
	photon& operator=(const photon& in){
        this->p = in.p;
        this->n = in.n;
        this->R = in.R;
        this->G = in.G;
        this->B = in.B;
        this->flow = in.flow;
        this->flag = in.flag;
    }
};

bool operator ==(photon p, photon q){
    return p.p==q.p;
}

#endif