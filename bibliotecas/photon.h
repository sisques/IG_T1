#ifndef PHOTON_H
#define PHOTON_H

#include <iostream>
#include "punto_direccion.h"
#include <vector>

using namespace std;

class photon{
public:
    point p; // position
    dir n;
    double R,G,B;; // power packed as 4 chars
    double flow;
    //dir phi, theta; // compressed incident direction
    bool inicializado = false;
    int refr;


    photon(const point &p) : p(p) {}
    photon(const double &x) : p(newPoint(x,x,x)) {}
    photon() = default;
    ~photon() = default;



    photon& operator=(const photon& in){
        this->p = in.p;
        this->n = in.n;
        this->R = in.R;
        this->G = in.G;
        this->B = in.B;
        this->flow = in.flow;
        //this->dir = in.dir;
        //this->theta = in.theta;
        this->inicializado = in.inicializado;
    }

    double at(int cd) const{
        if(cd % 3 == 0){
            return p.x;
        } else if ( cd % 3 == 1) {
            return p.y;
        } else if ( cd % 3 == 2) {
            return p.z;
        }
    }




};

bool operator ==(photon p, photon q) {
    return p.p==q.p;
}

bool operator < (photon p, photon q) {
    return p.p < q.p;
}

double dist(photon p, photon q){
    return dist(p.p, q.p);
}


#endif