#ifndef _ESFERA_H_
#define _ESFERA_H_

#include "punto_direccion.h"



struct sphere{
    public:
        point c;
        double r;
        int R, G, B;
};

    sphere newSphere(point _c, double _r, int _R, int _G, int _B){
        sphere s;
        s.c = _c;
        s.r = _r;
        s.R = _R;
        s.G = _G;
        s.B = _B;
        return s;
    }

    /**
    *   p = punto que queremos comprobar si pertenece a la esfera
    *   c = centro de la esfera
    *   r = radio de la esfera
    */
    bool sphereImplicit(point p, sphere s){
        dir aux = p - s.c;
        return ( dot(aux, aux) - pow(s.r,2) ) <= 0;
    }
    

#endif
