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
    int refr = 0;


    photon(const point &p) : p(p) {}

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

    bool empty(){
        return inicializado;
    }

    bool vacio(){
        bool a = this->p.x == 0;
        bool b = this->p.y == 0;
        bool c = this->p.z == 0;
        bool d = this->p.w == 0;
        bool e = this->n.x == 0;
        bool f = this->n.y == 0;
        bool g = this->n.z == 0;
        bool h = this->n.w == 0;
        bool i = this->R == 0;
        bool j = this->G == 0;
        bool k = this->B == 0;
        bool l = this->flow == 0;
        //this->dir = in.dir;
        //this->theta = in.theta;
        bool m = this->inicializado == false;
        bool n = this->refr == 0;
        bool resul =  a && b && c && d && e && f && g && h && i && j && k && l && m && n;
        return resul;
    }



    int size() const {
        return 3;
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