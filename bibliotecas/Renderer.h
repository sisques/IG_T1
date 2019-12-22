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

class Renderer
{
protected:

    bool colision(const point &c, const list<shared_ptr<figura>> &e, const dir &rayo, shared_ptr<figura> &fig, point &col){
		double t = 0;
        double distMin = numeric_limits<double>::max()-1;
        double distActual = 0;
        bool colision = false;
        bool yes = false;
        shared_ptr<figura> nearest;
        for( auto it = e.begin(); it != e.end(); ++it){
            shared_ptr<figura> f = *it;
            colision = f->intersection(rayo, c, t);
            if (colision) {
                point p = c + rayo * t;
                distActual = mod(c - p);
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

    dir luzDirecta(const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces, dir n, point p){
		dir luz = newDir(0.0,0.0,0.0);
        list<point> puntosLuces;
        for(shared_ptr<figura> i:luces){
			if(i->IsPoint()){
				for(point x:i->getLightPoints()){
					puntosLuces.push_back(x);
				}
			}
        }
        list<dir> rayos;
        for(point d:puntosLuces){
            rayos.push_back(d-p);
        }
        for(dir i:rayos){
            shared_ptr<figura> fig;
            point p2;
            bool colisiona = colision(p, e, i, fig, p2);
            if(colisiona && fig->isLight()){
				double r,g,b;
				fig->getRGB(EMISSION,p2,r,g,b);
				luz.x += r*abs(dot(n,i))/pow(mod(p2-p)*3,2.0);
				luz.y += g*abs(dot(n,i))/pow(mod(p2-p)*3,2.0);
				luz.z += b*abs(dot(n,i))/pow(mod(p2-p)*3,2.0);
            }
        }
		luz.x /= rayos.size();
		luz.y /= rayos.size();
		luz.z /= rayos.size();
		if (luz.x > 1) { luz.x = 1; }
		if (luz.y > 1) { luz.y = 1; }
		if (luz.z > 1) { luz.z = 1; }
        return luz;
    }

public:
    Renderer(){}
    ~Renderer(){};    
};

#endif // RENDERER_H
