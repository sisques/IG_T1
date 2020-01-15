#ifndef _TRIANGULO_H_
#define _TRIANGULO_H_


#include "punto_direccion.h"

    /**
    *   p = punto que queremos comprobar si pertenece al triangulo
    *   n = vector normal al triangulo
    *   o = un punto en el triangulo
    *   p pertenece al triangulo si y solo si (p-o) es perpendicular a n
    *   es decir (p-o)·n = 0
    */
    bool triangleImplicit(point p, point o, dir n) {
        dir d = p - o;
        return dot(d,n) == 0;
    }

    /**
    *   a, b, c = vertices del triangulo   
    *   s, t = numeros reales pasados como parametro, tal que
    *       s >= 0
    *       t >= 0
    *       s + t <= 1
    */
    point triangleParametric(double s, double t, point a, point b, point c) {
        return a + s*(b-a) +t*(c-a);
    }

#endif
