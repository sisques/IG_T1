#ifndef _FIGURAS_H_
#define _FIGURAS_H_


#include "punto_direccion.h"
#include <iostream>
#include <math.h>

using namespace std;

class figura{
    int R, G, B;
public:
    figura(int _R, int _G, int _B){
        this -> R = _R;
        this -> G = _G;
        this -> B = _B;
    }

    virtual bool implicit(point p) {
        return false;
    };
    int getR(){ return this->R;}
    int getG(){ return this->G;}
    int getB(){ return this->B;}

    virtual bool intersection(dir rd, point ro, double &t, double &dist){
        return false;
    }
};



class esfera : public figura {
private:
    point c;
    double r;
public:
    esfera(point _c, double _r, int _R, int _G, int _B): figura(_R, _G, _B){
        this -> c = _c;
        this -> r = _r;
    }


    point getCenter(){ return this->c;}
    double getRadius(){ return this->r;}


    bool implicit(point p) override {
        dir aux = p - this -> c;
        return ( dot(aux, aux) - this -> r*this -> r ) <= 0;
    }


    /*
     * Los puntos interseccion se calcularan usando la siguiente formula:
     *  p1 = ro + rd*(t-dist)
     *  p2 = ro + rd*(t+dist)
     */
    bool intersection(dir rd, point ro, double &t, double &dist) override {
        point s = this->c;
        double r = this->r;
        t = dot(s - ro, rd);
        point p = ro + rd*t;
        double y = mod(s - p);
        if ( y < r) {
            dist = sqrt(r * r - y * y);
            return true;
        } else {
            return false;
        }
    }

};


class plano : public figura {
private:
    point p;
    dir n;
public:
    plano(point _p, dir _n, int _R, int _G, int _B): figura(_R, _G, _B){
        this -> p = _p;
        this -> n = _n;
    }
    bool implicit(point p) override  {
        dir d = p - this -> p;
        return dot(d, this -> n) <= 0;
    }

    point getPoint(){ return this->p;}
    dir getNorma(){ return this->n;}

    bool intersection(dir rd, point ro, double &t, double &dist) override {
        dir diff = ro - this->p;
        double p1 = dot(diff, this->n);
        double p2 = dot(rd, this->n);
        t = p1 / p2;
        dist = 0;
        return true;
    }
};
#endif
