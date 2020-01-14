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


class getRGB;
using namespace std;

// Clase hija de renderer que implementa el algoritmo de photon mapping
class photonMapper : public Renderer
{
private:

	// Genera fotones a lo largo del cambio por la escena del rayo con origen en c y direccion rayo, añadiéndolos
	// al photon map global, o al de caustivas en caso de que sea el resultado de una refracción
	void generatePhotons(const point &c, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces,
				photonMap &pm, photonMap &pmc, const dir &rayo, const photon &phAnt, const bool &luzPuntual){
		double R = phAnt.R, G = phAnt.G, B = phAnt.B;
		photon newPhoton;
		
        shared_ptr<figura> actualFig = nullptr;
        point colP = newPoint(0,0,0);
		// Se comprueba si colisiona
        bool colisiona = colision(c,e,rayo,actualFig,colP);
		event_enum event;
		
		// Si colisiona y el evento no es muerte
        if(colisiona && !actualFig->isLight() && (event = actualFig->evento()) != DEATH){
			dir n = actualFig->getNormal(colP);
			dir dirNewRay = actualFig->nextRay(event, rayo, colP);
			
			double R_act, G_act, B_act;
			// Se obtiene el RGB de la figura colisionada
			if(event == PHONG){
				actualFig->phongColor(rayo,dirNewRay,colP,R_act,G_act,B_act);
			}
			else{
				actualFig->getRGB(event,colP,R_act,G_act,B_act);
			}
			
			// Se calcula su RGB usando la luz entrante del camino
			R_act = R_act * R * abs(dot(n,rayo));
			G_act = G_act * G * abs(dot(n,rayo));
			B_act = B_act * B * abs(dot(n,rayo));
			
			// Se define un nuevo fotón
			newPhoton.R = R_act;
			newPhoton.G = G_act;
			newPhoton.B = B_act;
			newPhoton.n = n;
			newPhoton.p = colP;
			newPhoton.flow = phAnt.flow;
			
			// Si es phong se almacena el fotón
			// Si no lo es no, simplemente sigue el camino
			if(event == PHONG){
				// Si ha salido de una refracción se guarda en el photon map de cáusticas
				if(phAnt.refr == 2){
					newPhoton.refr = 0;
					pmc.addPhoton(colP,newPhoton);
				}
				// Sino el el photon map global
				else{
					pm.addPhoton(colP,newPhoton);
				}
			}
			// Si el evento es refracción se gestiona, para que cuando salga de esta
			// se guarde el fotón en el photon map de cáusticas
			else if(event == REFRACTION){
				if(phAnt.refr == 0){newPhoton.refr = 1;}
				else if(phAnt.refr == 1){newPhoton.refr = 2;}
			}
			// Sigue con el camino generando más fotones
			generatePhotons(colP,  e, luces, pm, pmc, dirNewRay, newPhoton, luzPuntual);
        }
    }

public:
    photonMapper(){}
    ~photonMapper(){};

	// Función que realiza la fase del trazado de fotones y genera los photon maps de la iluminación global y de las cáusticas
	void generatePhotonMap(photonMap &base, photonMap &caustics, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces,
								const bool &luzPuntual){
		// Dividimos el número de rayos a lanzar entre el número de luces de la escena
		int nRayos = 10000/luces.size();
		// Para todas las luces de la escena
		for(shared_ptr<figura> i:luces){
			for(int j = 0; j < nRayos;++j){
				double rx,ry,rz;
				point pl; dir dl;
				// Generamos un punto y dirección aleatorios, dependiente del tipo de figura
				// Si es un puz puntual, obviamente el punto será la propia liz
				i->getLightRay(pl,dl);
				dir rayo = newDir(rx,ry,rz);
				photon ph;
				ph.n = rayo;
				// Calculamos el flujo
				ph.flow = 2.0/(double)nRayos;
				// Asignamos al photon inicial el RGB de la luz
				i->getRGB(EMISSION,pl,ph.R,ph.G,ph.B);
				// Generamos los photones que dicho rayo deje en la escena en su camino
				generatePhotons(pl, e, luces, base, caustics, dl, ph, luzPuntual);
			}
        }
	}
	
	// Obtenemos el RGB correspondiente al rayo con origen en c y dirección rayo a partir del photon map de
	// iluminación global y cáusticas
	void getRGB(const point &c, const list<shared_ptr<figura>> &e, photonMap &pm, photonMap &pmc, const dir &rayo, 
				double& R, double& G, double& B){
		shared_ptr<figura> actualFig = nullptr;
		point colP;
		// Comprobamos si colisiona
		bool colisiona = colision(c,e,rayo,actualFig,colP);
		// Si no colisiona
		if(!colisiona){
			R = 0;
			G = 0;
			B = 0;
		}
		// Si colisiona
		else{
			event_enum event;
			// Se selecciona un evento distinto de muerte aleatoriamente
			do{event = actualFig->evento();}while(event == DEATH);
			
			dir dirNewRay = actualFig->nextRay(event, rayo, colP);
			// Si es emisor devuelve su RGB directamente
			if(actualFig->isLight()){
				actualFig->getRGB(EMISSION,colP,R,G,B);
			}
			// Si es refracción o reflexión, calcula la interacción, y devuelve el RGB
			// de dicha interacciónn
			else if(event != PHONG && event != EMISSION){
				getRGB(colP, e, pm, pmc, dirNewRay, R,G,B);
				double r,g,b;
				actualFig->getRGB(event,colP,r,g,b);
				R *= r; G *= g; B *= b;
			}
			// Si es phong
			else{
				// Halla el RGB en el photon map global
				pm.getColorAt(colP,R,G,B);
				double r,g,b;
				// Y le suma el RGB de las cáusticas
				pmc.getColorAt(colP,r,g,b);
				R += r; G += g; B += b;
				actualFig->phongColor(rayo,dirNewRay,colP,r,g,b);
				R *= r; G *= g; B *= b;
				// Si se pasa del límite, se reescala
				if(max(R, max(G,B)) > 1){
					R = R/max(R, max(G,B));
					G = G/max(R, max(G,B));
					B = B/max(R, max(G,B));
				}
			}
		}
	}
    
};

#endif // PHOTONMAPPER_H
