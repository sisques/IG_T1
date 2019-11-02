#ifndef _PLANO_H_
#define _PLANO_H_

#include "punto_direccion.h"





struct plane{
public:
    point o;
    dir n;
    int R, G, B;
};

plane newPlane(point _o, dir _n, int _R, int _G, int _B){
    plane p;
    p.o = _o;
    p.n = _n;
    p.R = _R;
    p.G = _G;
    p.B = _B;
    return p;
}
    /**
    *   p = punto que queremos comprobar si pertenece al plano
    *   n = vector normal al plano
    *   o = un punto en el plano
    *   p pertenece al plano si y solo si (p-o) es perpendicular a n
    *   es decir (p-o)·n = 0
    */

   //poner un threshold
    bool planeImplicit(point p, plane pl) {
        dir d = p - pl.o;
        return dot(d,pl.n) <= 0;
    }

    /**
    *   a, b = vectores tangentes al plano   
    *   c = punto del plano
    *   s, t = numeros reales pasados como parametro
    *   Cualquier punto X del plano se define por s*a +t*b +c para algun valor de s, t
    */
    point planeParametric(double s, double t, dir a, dir b, point c) {
        return c + s*a +t*b;
    }

#endif
