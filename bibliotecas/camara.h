#ifndef _CAMARA_H_
#define _CAMARA_H_

#include "punto_direccion.h"


struct camara {
public:
    point o;
    dir u, l, f;
};
    camara newCamara(point _o, dir _u, dir _l, dir _f){
        camara c;
        c.o = _o;
        c.u = _u;
        c.l = _l;
        c.f = _f;
        return c;
    }


#endif
