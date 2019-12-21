#ifndef PHOTONMAPPER_H
#define PHOTONMAPPER_H

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

class photonMapper
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
    photonMapper(){}
    ~photonMapper(){};

	void generatePhotons(const point &c, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces,
				photonMap &pm, const dir &rayo, double& R, double& G, double& B, const bool &luzPuntual){
        shared_ptr<figura> actualFig = nullptr;
        point colP;
        bool colisiona = colision(c,e,rayo,actualFig,colP);
        event_enum event;
        if(!colisiona){
            event = DEATH;
        }
        else{
            event = actualFig->evento();
        }
        double R_siguiente, G_siguiente, B_siguiente;

        if(colisiona && actualFig->isLight()){
            actualFig->getRGB(EMISSION,colP, R,G,B);
			pm.addPhoton(colP,R,G,B);
        }
        else if(event == DEATH){
            R = 0;
            G = 0;
            B = 0;
        }
        else if(event == REFRACTION || event == REFLEXION || event == PHONG){
			dir luz = newDir(0.0,0.0,0.0);
			if(luzPuntual){
				luz =  luzDirecta(e, luces,actualFig->getNormal(colP),colP);
			}
            dir dirNewRay;
            double p = 0.0;
			dirNewRay = actualFig->nextRay(event, rayo, colP);
			p = actualFig->probEvent(event);
            generatePhotons(colP,  e, luces, pm, dirNewRay, R_siguiente, G_siguiente, B_siguiente, luzPuntual);
			
			dir n = actualFig->getNormal(colP);
			
			if(event != PHONG){
				luz.x = R_siguiente * abs(dot(n,dirNewRay));
				luz.y = G_siguiente * abs(dot(n,dirNewRay));
				luz.z = B_siguiente * abs(dot(n,dirNewRay));
			}
			else{
				luz.x = (luz.x + R_siguiente * abs(dot(n,dirNewRay)));
				luz.y = (luz.y + G_siguiente * abs(dot(n,dirNewRay)));
				luz.z = (luz.z + B_siguiente * abs(dot(n,dirNewRay)));
			}
			
            if(event == PHONG){
				actualFig->phongColor(rayo,dirNewRay,colP,R,G,B);
            }
            else{
				actualFig->getRGB(event,colP,R,G,B);
            }
			
			R = R*luz.x/p;
			G = G*luz.y/p;
			B = B*luz.z/p;
			pm.addPhoton(colP,R,G,B);
        }
        else{
            actualFig->getRGB(REFLEXION,colP,R,G,B);
			pm.addPhoton(colP,R,G,B);
        }
    }

	photonMap generatePhotonMap(const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces,
								const bool &luzPuntual){
		photonMap pm;
		list<point> puntosLuces;
		for(shared_ptr<figura> i:luces){
			for(point x:i->getLightPoints()){
				puntosLuces.push_back(x);
			}
		}
		int nRayos = 100000/puntosLuces.size();
		srand (time(NULL));
		for(point pl:puntosLuces){
			for(int i = 0; i < nRayos;++i){
				double R,G,B;
				dir rayo = newDir((double)rand() / RAND_MAX,(double)rand() / RAND_MAX,(double)rand() / RAND_MAX);
				generatePhotons(pl, e, luces, pm, rayo, R, G, B, luzPuntual);
			}
        }
		return pm;
	}
	
	
	void getRGB(const point &c, const list<shared_ptr<figura>> &e, photonMap &pm, const dir &rayo, 
				double& R, double& G, double& B){
		shared_ptr<figura> actualFig = nullptr;
		point colP;
		bool colisiona = colision(c,e,rayo,actualFig,colP);
		event_enum event;
		if(!colisiona){
			R = 0;
			G = 0;
			B = 0;
		}
		else{
			pm.getColorAt(colP,R,G,B);
		}
	}
    
};

#endif // PHOTONMAPPER_H
