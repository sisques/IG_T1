#ifndef _CAMARA_H_
#define _CAMARA_H_

#include "punto_direccion.h"

// Estructura que representa una cámara pinhole
struct camara {
public:
	// Posición de la cámara
    point o;
	// Base de coordenadas de la cámara
    dir u, l, f;
};
	// Contructor de la cámara que pide el punto origen
	// y loa vectores de la base de coordenadas
    camara newCamara(point _o, dir _u, dir _l, dir _f){
        camara c;
        c.o = _o;
        c.u = _u;
        c.l = _l;
        c.f = _f;
        return c;
    }


#endif
