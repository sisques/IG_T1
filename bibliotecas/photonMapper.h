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
#include "photon.h"


using namespace std;

class photonMapper : public Renderer
{
private:

	void generatePhotons(const point &c, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces,
				photonMap &pm, photonMap &pmc, const dir &rayo, const photon &phAnt, const bool &luzPuntual){
		double R = phAnt.R, G = phAnt.G, B = phAnt.B;
		photon newPhoton;
		
        shared_ptr<figura> actualFig = nullptr;
        point colP = newPoint(0,0,0);
        bool colisiona = colision(c,e,rayo,actualFig,colP);

		event_enum event;
        if(colisiona &&  (event = actualFig->evento()) != DEATH){
			dir n = actualFig->getNormal(colP);
			dir dirNewRay = actualFig->nextRay(event, rayo, colP);
			
			double R_act, G_act, B_act;
			if(event == PHONG){
				actualFig->phongColor(rayo,dirNewRay,colP,R_act,G_act,B_act);
			}
			else{
				actualFig->getRGB(event,colP,R_act,G_act,B_act);
			}
			
			R_act = R_act * R * abs(dot(n,rayo));
			G_act = G_act * G * abs(dot(n,rayo));
			B_act = B_act * B * abs(dot(n,rayo));
			
			newPhoton.R = R_act;
			newPhoton.G = G_act;
			newPhoton.B = B_act;
			newPhoton.n = n;
			newPhoton.flow = phAnt.flow;
			
			if(event == PHONG){
				if(phAnt.refr == 2){
					newPhoton.refr = 0;
					pmc.addPhoton(colP,newPhoton);
				}
				else{
					pm.addPhoton(colP,newPhoton);
				}
			}
			else if(event == REFRACTION){
				if(phAnt.refr == 0){newPhoton.refr = 1;}
				else if(phAnt.refr == 1){newPhoton.refr = 2;}
			}
			
			generatePhotons(colP,  e, luces, pm, pmc, dirNewRay, newPhoton, luzPuntual);
        }
    }

public:
    photonMapper(){}
    ~photonMapper(){};

	void generatePhotonMap(photonMap &base, photonMap &caustics, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces,
								const bool &luzPuntual){
		photonMap pm,pmc;
		int nRayos = 5000/luces.size();
		for(shared_ptr<figura> i:luces){
			for(int j = 0; j < nRayos;++j){
				double rx,ry,rz;
				point pl; dir dl;
				i->getLightRay(pl,dl);
				dir rayo = newDir(rx,ry,rz);
				photon ph;
				ph.n = rayo;
				ph.flow = 2.0/(double)nRayos;
				i->getRGB(EMISSION,pl,ph.R,ph.G,ph.B);
				generatePhotons(pl, e, luces, pm, pmc, dl, ph, luzPuntual);
			}
        }
		base = pm;
		caustics = pmc;
	}
	
	
	void getRGB(const point &c, const list<shared_ptr<figura>> &e, photonMap &pm, photonMap &pmc, const dir &rayo, 
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
			event_enum event;
			do{event = actualFig->evento();}while(event == DEATH);
			if(event != PHONG && event != EMISSION){
				dir dirNewRay = actualFig->nextRay(event, rayo, colP);
				getRGB(colP, e, pm, pmc, dirNewRay, R,G,B);
			}
			else if(event == EMISSION){
				actualFig->getRGB(EMISSION,colP,R,G,B);
			}
			else{
				pm.getColorAt(colP,R,G,B);
				double r = R,g = G,b = B;
				pmc.getColorAt(colP,R,G,B);
				R += r; G += g; B += b;
				if(max(R, max(G,B)) > 1){
					R = R/max(R, max(G,G));
					G = G/max(R, max(G,B));
					B = B/max(R, max(G,B));
				}
			}
		}
	}
    
};

#endif // PHOTONMAPPER_H
