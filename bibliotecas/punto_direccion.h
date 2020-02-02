#ifndef _PUNTO_DIRECCION_H_
#define _PUNTO_DIRECCION_H_

#include <cmath>

// Estructura que representa un punto en el espacio
struct point {
public:
    double x;
    double y;
    double z;
    double w;
};

// Estructura que representa una dirección en el espacio
struct dir {
public:
    double x;
    double y;
    double z;
    double w;
};

// Devuelve un punto con coordenadas _x, _y y _z
point newPoint(double _x, double _y, double _z, double _w) {
    point p;
    p.x = _x;
    p.y = _y;
    p.z = _z;
    p.w = _w;
    return p;
}

// Devuelve un punto con coordenadas _x, _y y _z
point newPoint(double _x, double _y, double _z) {
    point p;
    p.x = _x;
    p.y = _y;
    p.z = _z;
    p.w = 1;
    return p;
}

// Devuelve una dirección según _x, _y y _z
dir newDir(double _x, double _y, double _z) {
    dir p;
    p.x = _x;
    p.y = _y;
    p.z = _z;
    p.w = 0;
    return p;
}

// Operador de suma entre puntos
point operator +(point p, point q)         {
    return newPoint(p.x + q.x, p.y + q.y, p.z + q.z);
}

// Operador de resta entre puntos (dirección entre ambos)
dir operator -(point p, point q)         {
    return newDir(p.x - q.x, p.y - q.y, p.z - q.z);
}

// Operador de suma entre punto y dirección
point operator +(point p, dir q)         {
    return newPoint(p.x + q.x, p.y + q.y, p.z + q.z);
}

// Operador de resta entre punto y dirección
point operator -(point p, dir q)         {
    return newPoint(p.x - q.x, p.y - q.y, p.z - q.z);
}

// Operador de igualdad entre puntos
bool operator ==(point p, point q)         {
    return p.x == q.x && p.y == q.y && p.z == q.z;
}

// Operador de suma entre direcciones
dir operator +(dir p, dir q)         {
    return newDir(p.x + q.x, p.y + q.y, p.z + q.z);
}

// Operador de resta entre direcciones
dir operator -(dir p, dir q)         {
    return newDir(p.x - q.x, p.y - q.y, p.z - q.z);
}

// Operador de negación de una dirección
dir operator -(dir d)         {
    return newDir(-d.x, -d.y, -d.z);
}

// Operador de escalado de una dirección
dir operator *( double q, dir p)         {
    return newDir(p.x*q, p.y*q, p.z*q);
}

// Operador de escalado de una dirección
dir operator *( dir p, double q)         {
    return newDir(q*p.x, q*p.y, q*p.z);
}

// Operador de escalado inverso de una dirección
dir operator /(dir p, double q)         {
    return newDir(p.x/q, p.y/q, p.z/q);
}

// Módulo de una dirección
double mod(dir d){
    return sqrt( pow(d.x,2) + pow(d.y,2) + pow(d.z,2) );
}

// Normaliza una dirección
dir normalize(dir d){
    return d / mod(d);
}

// Devuelve la distancia entre dos puntos
double dist(point p, point q) {
    return mod(q-p);
}

// Calcula el producto escalar entre dos direcciones
double dot(dir d, dir e) {
    return d.x*e.x + d.y*e.y + d.z*e.z;
}

// Calcula el ángulo entre dos direcciones
double angle(dir d, dir e) {
    return acos( ( dot(d,e) )/( mod(d)*mod(e) ) );
}

// Calcula el producto vectorial entre dos direcciones
dir cross(dir d, dir e) {
    return newDir(d.y*e.z - d.z*e.y, d.z*e.x - d.x*e.z, d.x*e.y -d.y*e.x);
}


#endif