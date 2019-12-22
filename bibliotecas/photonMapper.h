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
#include "Renderer.h"


using namespace std;

class photonMapper : public Renderer
{
private:

	void generatePhotons(const point &c, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces,
				photonMap &pm, const dir &rayo,const double &R,const double &G,const double &B, const bool &luzPuntual){
        shared_ptr<figura> actualFig = nullptr;
        point colP = newPoint(0,0,0);
        bool colisiona = colision(c,e,rayo,actualFig,colP);
		event_enum event;
        if(colisiona &&  (event = actualFig->evento()) != DEATH){
			dir n = actualFig->getNormal(colP);
			dir dirNewRay;
			dirNewRay = actualFig->nextRay(event, rayo, colP);
			
			double R_act, G_act, B_act;
			if(event == PHONG){
				actualFig->phongColor(rayo,dirNewRay,colP,R_act,G_act,B_act);
			}
			else{
				actualFig->getRGB(event,colP,R_act,G_act,B_act);
			}
			
			R_act = R_act * R * abs(dot(n,dirNewRay));
			G_act = G_act * G * abs(dot(n,dirNewRay));
			B_act = B_act * B * abs(dot(n,dirNewRay));
			
			pm.addPhoton(colP,R_act,G_act,B_act);
			
			generatePhotons(colP,  e, luces, pm, dirNewRay, R_act, G_act, B_act, luzPuntual);
        }
    }

public:
    photonMapper(){}
    ~photonMapper(){};

	photonMap generatePhotonMap(const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces,
								const bool &luzPuntual){
		photonMap pm;
		double r,g,b;
		list<pair<point,point>> puntosLuces;
		for(shared_ptr<figura> i:luces){
			for(point x:i->getLightPoints()){
				i->getRGB(EMISSION,x,r,g,b);
				puntosLuces.push_back(pair<point,point>(x,newPoint(r,g,b)));
			}
		}
		int nRayos = 10/puntosLuces.size();
		srand (time(NULL));
		for(pair<point,point> pl:puntosLuces){
			for(int i = 0; i < nRayos;++i){
				double rx = 2.0f * ((double)rand() / (double)RAND_MAX) - 1.0f;
				double ry = 2.0f * ((double)rand() / (double)RAND_MAX) - 1.0f;
				double rz = 2.0f * ((double)rand() / (double)RAND_MAX) - 1.0f;
				dir rayo = newDir(rx,ry,rz);
				generatePhotons(pl.first, e, luces, pm, rayo, pl.second.x, pl.second.y, pl.second.z, luzPuntual);
			}
        }
		return pm;
	}
	
	
	void getRGB(const point &c, const list<shared_ptr<figura>> &e, photonMap &pm, const dir &rayo, 
				double& R, double& G, double& B){
		shared_ptr<figura> actualFig = nullptr;
		point colP;
		bool colisiona = colision(c,e,rayo,actualFig,colP);
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
