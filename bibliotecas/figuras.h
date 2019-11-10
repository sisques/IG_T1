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
    dir getNormal(){ return this->n;}

    bool intersection(dir rd, point ro, double &t, double &dist) override {
        dir diff = this->p - ro;
        double p1 = dot(diff, this->n);
        double p2 = dot(rd, this->n);
        if (p2 == 0){
            //lineas paralelas
            return false;
        } else if (p1 == 0) {
            //linea contenida en el plano
            return false;
        } else {
            //hay un puntpo de interseccion
            t = p1 / p2;
            if (t < 0) {
                return false;
            }
            dist = 0;
            return true;
        }

    }
};




class triangulo : public figura {
private:
    point v0, v1, v2;
public:
    triangulo(point _v0, point _v1, point _v2, int _R, int _G, int _B): figura(_R, _G, _B){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
    }
    bool implicit(point p) override  {
        dir d = p - this -> p;
        return dot(d, this -> getNormal()) <= 0;
    }

    dir getNormal(){
       dir arista1 = this->v1 - this->v0;
       dir arista2 = this->v2 - this->v0;
       return cross(arista1, arista2);
    }
    point getVertice0(){ return this->v0;}
    point getVertice1(){ return this->v1;}
    point getVertice2(){ return this->v2;}

    //Implementacion del algoritmo de Möller-Trumbore
    bool intersection(dir rd, point ro, double &t, double &dist) override {
        const double EPSILON = 0.0000001;
        dir arista1, arista2, h, s, q;
        double a, f, u, v;
        arista1 = this->v1 - this->v0;
        arista2 = this->v2 - this->v0;
        h = cross(rd, arista2);
        a = dot(arista1, h);

        //El rayo es paralelo al triangulo
        if (a > -EPSILON && a < EPSILON){
            return false;
        }

        f = 1.0/a;
        s = ro - this -> v0;
        u = f*dot(s,h);
        if (u < 0.0 || u > 1.0) {
            return false;
        }

        q = cross(s, arista1);
        v = f*dot(rd, q);
        if (v < 0.0 || u + v > 1.0){
            return false;
        }

        //Ahora se pasa a calcular t para obtener el punto de interseccion
        dist = 0;
        t = f*dot(arista2, q);
        //Hay interseccion
        if (t > EPSILON && t < 1/EPSILON) {
            return true;
        }
        // Intersecta la linea pero no el plano
        else {
            return false;
        }

    }
};
#endif
