#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <iostream>
#include <math.h>
#include "camara.h"
#include "russian_roulette.h"
#include <list>
#include <memory>
#include <limits>
#include "globals.h"
#include "phong.h"


using namespace std;

class pathTracer
{
private:

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
            for(point x:i->getLightPoints()){
                puntosLuces.push_back(x);
            }
        }
        list<dir> rayos;
        for(point d:puntosLuces){
            rayos.push_back(d-p);
        }
		int l = 0;
        for(dir i:rayos){
            shared_ptr<figura> fig;
            point p2;
            bool colisiona = colision(p, e, i, fig, p2);
            if(colisiona && fig->isLight()){
				double r,g,b;
				fig->getRGB(EMISSION,r,g,b);
				luz.x += r*abs(dot(n,i));//pow(mod(p2-p),2.0);
				luz.y += g*abs(dot(n,i));//pow(mod(p2-p),2.0);
				luz.z += b*abs(dot(n,i));//pow(mod(p2-p),2.0);
				++l;
            }
        }
		luz.x /= rayos.size();
		luz.y /= rayos.size();
		luz.z /= rayos.size();
        return luz;
    }

public:
    pathTracer(){}
    ~pathTracer(){};

    void getRGB(const point &c, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces, const dir &rayo, double& R, double& G, double& B, double &dist){
		bool guarro = false;
        shared_ptr<figura> actualFig = nullptr;
        point colP;
        bool colisiona = colision(c,e,rayo,actualFig,colP);
		dist += mod(c-colP);
        event_enum event;
        if(!colisiona){
            event = DEATH;
        }
        else{
            event = actualFig->evento();
        }
        double R_siguiente, G_siguiente, B_siguiente;

        if(colisiona && actualFig->isLight()){
            actualFig->getRGB(EMISSION, R,G,B);
        }
        else if(event == DEATH){
            R = 0;
            G = 0;
            B = 0;
        }
        else if(event == REFRACTION || event == REFLEXION || event == PHONG){
			dir luz;// = luzDirecta(e, luces,actualFig->getNormal(colP),colP);
            dir dirNewRay;
            point nextPoint;
			double p = 0.0;
			dirNewRay = actualFig->nextRay(event, rayo, colP,nextPoint);
			p = actualFig->probEvent(event);
            if ( event == REFRACTION) {
                colP = nextPoint;
            }
            getRGB(colP,  e, luces, dirNewRay, R_siguiente, G_siguiente, B_siguiente, dist);
			
			dir n = actualFig->getNormal(colP);
			
			/*if(mod(luz) != 0){normalize(luz);}
			if((double)rand() / RAND_MAX < 0.5){*/
				luz.x = R_siguiente * abs(dot(n,dirNewRay));
				luz.y = G_siguiente * abs(dot(n,dirNewRay));
				luz.z = B_siguiente * abs(dot(n,dirNewRay));
			//}
			
            if(event == PHONG){
                actualFig->phongColor(rayo,dirNewRay,colP,R,G,B);
            }
            else{
                actualFig->getRGB(event,R,G,B);
            }
			
			if(!guarro){
				R = R*luz.x/p;
				G = G*luz.y/p;
				B = B*luz.z/p;
			}
			else{
				double m = (R_siguiente+G_siguiente+B_siguiente)/3;
				if(event == REFLEXION || event == REFRACTION){
					R = R*(1-p) + R_siguiente*p;
					G = G*(1-p) + G_siguiente*p;
					B = B*(1-p) + B_siguiente*p;
				}
				R = R*m*abs(dot(n,dirNewRay))/p;
				G = G*m*abs(dot(n,dirNewRay))/p;
				B = B*m*abs(dot(n,dirNewRay))/p;
			}
        }
        else{
            actualFig->getRGB(REFLEXION,R,G,B);
        }
    }
};

#endif // PATHTRACER_H
