#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <math.h>
#include "camara.h"
#include "russian_roulette.h"
#include <list>
#include <memory>
#include <limits>
#include "globals.h"
#include "photonMap.h"


using namespace std;

// Clase padre de photon mapper y path tracing, que contiene el comportamiento
// común de ambos
class Renderer
{
protected:

	// Devuelve en "fig" la figura de la lista de figuras "e" con la que primero intersecta el rayo
	// prodecente del punto "c" con dirección "rayo", en "col" se devuelve el punto de intersección
    bool colision(const point &c, const list<shared_ptr<figura>> &e, const dir &rayo, shared_ptr<figura> &fig, point &col){
		double t = 0;
        double distMin = numeric_limits<double>::max()-1;
        double distActual = 0;
        bool colision = false;
        bool yes = false;
        shared_ptr<figura> nearest;
		// Se comprueba la colisión con todas las figuras de la lista
        for( auto it = e.begin(); it != e.end(); ++it){
            shared_ptr<figura> f = *it;
            colision = f->intersection(rayo, c, t);
			// Si intersecta
            if (colision) {
                point p = c + rayo * t;
                distActual = mod(c - p);
				// Si la distancia entre "c" y la intersección es menor que la guardada
				// se actualiza la información de la colisión
				// En la colisión se da preferencia a las luces frente a las figuras normales
                if (distActual < distMin || f->isLight() && (distActual > distMin-EPSILON && distActual < distMin+EPSILON)) {
                    nearest = f;
                    distMin = distActual;
                    col = p;
                    yes = true;
                }
            }

        }
        fig = nearest;
        return yes;
    }

	// Devuelve una dirección que contiene en sus valores x, y, z la luminancia R, G, B correspodiente a la luz
	// directa incidente sobre un punto "p" con normal "n". Solo se cuentan las luces puntuales
    dir luzDirecta(const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces, dir rayo, dir n, point p){
		dir luz = newDir(0.0,0.0,0.0);
        list<point> puntosLuces;
		// Se obtiene la posición de las luces puntuales
        for(shared_ptr<figura> i:luces){
			if(i->IsPoint()){
				for(point x:i->getLightPoints()){
					puntosLuces.push_back(x);
				}
			}
        }
        list<dir> rayos;
		// Se trazan rayos desde "p" hacia las luces
        for(point d:puntosLuces){
            rayos.push_back(d-p);
        }
		// Para todos esos rayos, se comprueba si intersectan antes con
		// alguna otra figura que con la luz, en caso de que no colisione
		// con nada que no sea la luz, se añade su luminancia a la cuenta
        for(dir i:rayos){
            shared_ptr<figura> fig;
            point p2;
            bool colisiona = colision(p, e, i, fig, p2);
            if(colisiona && (fig->isLight())){
				double r,g,b;
				fig->getRGB(EMISSION,p2,r,g,b);
				// Se divide por la distancia al cuadrado
				// Se multiplica la distancia por 3 para hacer un pequeño escalado
				luz.x += r*abs(dot(n,i))/pow(mod(p2-p)*3,2.0);
				luz.y += g*abs(dot(n,i))/pow(mod(p2-p)*3,2.0);
				luz.z += b*abs(dot(n,i))/pow(mod(p2-p)*3,2.0);
            }
			else if(colisiona && fig->evento() == REFRACTION){
				double r,g,b;
				fig->getRGB(REFRACTION,p2,r,g,b);
				dir nR = fig->nextRay(REFRACTION,rayo,p2);
				dir aux = luzDirecta(e,luces,nR,fig->getNormal(p2),p2);
				luz.x += r*aux.x*abs(dot(n, nR));
				luz.y += g*aux.y*abs(dot(n, nR));
				luz.z += b*aux.z*abs(dot(n, nR));
			}
        }
		// Hace la media de la luz entre el número de rayos
		luz.x /= rayos.size();
		luz.y /= rayos.size();
		luz.z /= rayos.size();
		// Se capa en caso de exceso
		if (luz.x > 1) { luz.x = 1; }
		if (luz.y > 1) { luz.y = 1; }
		if (luz.z > 1) { luz.z = 1; }
        return luz;
    }

public:
	// Constructor
    Renderer(){}
	// Destructor
    ~Renderer(){};    
};

#endif // RENDERER_H
