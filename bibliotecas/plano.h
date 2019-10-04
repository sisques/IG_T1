#ifndef _PLANO_H_
#define _PLANO_H_

#include "punto_direccion.h"


    /**
    *   p = punto que queremos comprobar si pertenece al plano
    *   n = vector normal al plano
    *   o = un punto en el plano
    *   p pertenece al plano si y solo si (p-o) es perpendicular a n
    *   es decir (p-o)·n = 0
    */
    bool planeImplicit(point p, point o, dir n) {
        dir d = p - o;
        return dot(d,n) == 0;
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
