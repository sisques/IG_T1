#ifndef PHOTON_H
#define PHOTON_H

#include <iostream>
#include "punto_direccion.h"
#include <vector>

using namespace std;

// Clase que representa un photón generado por un rayo de luz
class photon{
public:
	// Posición del fotón
    point p;
	// Normal de la superficie en la que impactóo
    dir n;
	// RGB del fotón
    double R,G,B;
	// Flujo del fotón
    double flow;
    bool inicializado = false;
	// Entero para controlar a que photon map debe ir
	// Si es 0 al global, si es 1 ha entrado en refracción,
	// si es 2 debe ir al de cáusticas, y si es 3, proviene
	// de reflexión, y debe ir al de cáusticas
    int state;
	
	// Constructores
    photon(const point &p) : p(p) {}
    photon(const double &x) : p(newPoint(x,x,x)) {}
    photon() = default;
	// Destructor
    ~photon() = default;


	// Operador de igualdad
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
	
	// Para obtener la información de la posición
    double at(int cd) const{
        if(cd % 3 == 0){
            return p.x;
        } else if ( cd % 3 == 1) {
            return p.y;
        } else if ( cd % 3 == 2) {
            return p.z;
        }
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