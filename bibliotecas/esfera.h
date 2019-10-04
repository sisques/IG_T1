#ifndef _ESFERA_H_
#define _ESFERA_H_

#include "punto_direccion.h"


    /**
    *   p = punto que queremos comprobar si pertenece a la esfera
    *   c = centro de la esfera
    *   r = radio de la esfera
    */
    bool sphereImplicit(point p, point c, double r){
        dir aux = p - c;
        return ( dot(aux, aux) - pow(r,2) ) == 0;
    }
    
    /**
    *   c = centro de la esfera
    *   r = radio de la esfera
    *   inclination = posicion del punto en el eje vertical de la esfera, en radianes
    *   azimuth = posicion del punto en el eje horizontal de la esfera, en radianes
    */ 
    point sphereParametric(point c, double r, double inclination, double azimuth){
        point aux = newPoint(   r*sin(inclination)*sin(azimuth), 
                                r*sin(inclination)*cos(azimuth),
                                r*cos(inclination)              );
        return c + aux;
    }

#endif
