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

	int maxRays = 100000, maxPhotonsGlobal = 10000, maxPhotonsCaustics = 10000;
	int currentRays = 0;

	// Genera fotones a lo largo del cambio por la escena del rayo con origen en c y direccion rayo, añadiéndolos
	// al photon map global, o al de caustivas en caso de que sea el resultado de una refracción
	// Devuelve true si aún queda espacio en alguno de lo dos photon maps, falso en caso contrario
	bool generatePhotons(const point &c, const list<shared_ptr<figura>> &e,
				photonMap &pm, photonMap &pmc, const dir &rayo, const photon &phAnt){
		
		// Si ya se han lanzado los rayos máximos
		if(currentRays >  maxRays){return false;}
		
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
			bool refInt = false;
			dir dirNewRay = actualFig->nextRay(event, rayo, colP, refInt);
			
			double R_act, G_act, B_act;
			// Se obtiene el RGB de la figura colisionada
			if(event == PHONG){
				actualFig->phongColor(rayo,dirNewRay,colP,R_act,G_act,B_act);
			}
			else{
				actualFig->getRGB(event,colP,R_act,G_act,B_act);
			}
			
			// Se calcula su RGB usando la luz entrante del camino
			R_act = R_act * R * abs(dot(n,dirNewRay));
			G_act = G_act * G * abs(dot(n,dirNewRay));
			B_act = B_act * B * abs(dot(n,dirNewRay));
			
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
				if((phAnt.state == 2 || phAnt.state == 3) && pmc.size() < maxPhotonsCaustics){
					newPhoton.state = 0;
					pmc.addPhoton(colP,newPhoton);
				}
				// Sino el el photon map global
				else if(pm.size() < maxPhotonsGlobal){
					pm.addPhoton(colP,newPhoton);
				}
			}
			// Si el evento es refracción se gestiona, para que cuando salga de esta
			// se guarde el fotón en el photon map de cáusticas
			else if(event == REFRACTION){
				if(phAnt.state == 0){newPhoton.state = 1;}
				else if(phAnt.state == 1 && !refInt){newPhoton.state = 2;}
			}
			else if(event == REFLEXION && phAnt.state == 0){
				newPhoton.state = 3;
			}
			else{
				newPhoton.state = phAnt.state;
			}
			// Si quedan fotones por meter
			if(pm.size() < maxPhotonsGlobal || pmc.size() < maxPhotonsCaustics){
				// Sigue con el camino generando más fotones
				generatePhotons(colP,  e, pm, pmc, dirNewRay, newPhoton);
			}
			// Si los photon maps ya han alcanzado el limite
			else{
				return false;
			}
        }
		return (pm.size() < maxPhotonsGlobal || pmc.size() < maxPhotonsCaustics) && currentRays <  maxRays;
    }

public:
    photonMapper(){}
	photonMapper(int mr, int mpg, int mpc):maxRays(mr), maxPhotonsGlobal(mpg), maxPhotonsCaustics(mpc){}
    ~photonMapper(){};

	// Función que realiza la fase del trazado de fotones y genera los photon maps de la iluminación global y de las cáusticas
	void generatePhotonMap(photonMap &base, photonMap &caustics, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces){
		while(currentRays < maxRays && (base.size() < maxPhotonsGlobal || caustics.size() < maxPhotonsCaustics)){
			// Dividimos el número de rayos a lanzar entre el número de luces de la escena
			int nRayos = (maxPhotonsGlobal+maxPhotonsCaustics-base.size()-caustics.size())/luces.size();
			bool masRayos = true;
			// Para todas las luces de la escena
			for(shared_ptr<figura> i:luces){
				for(int j = 0; masRayos;++j){
					++currentRays;
					double rx,ry,rz;
					point pl; dir dl;
					// Generamos un punto y dirección aleatorios, dependiente del tipo de figura
					// Si es un puz puntual, obviamente el punto será la propia liz
					i->getLightRay(pl,dl);
					dir rayo = newDir(rx,ry,rz);
					photon ph;
					ph.n = rayo;
					// Calculamos el flujo
					ph.flow = 20.0/nRayos;
					// Asignamos al photon inicial el RGB de la luz
					i->getRGB(EMISSION,pl,ph.R,ph.G,ph.B);
					// Generamos los photones que dicho rayo deje en la escena en su camino
					masRayos = generatePhotons(pl, e, base, caustics, dl, ph);
				}
			}
		}
	}
	
	// Obtenemos el RGB correspondiente al rayo con origen en c y dirección rayo a partir del photon map de
	// iluminación global y cáusticas
	void getRGB(const point &c, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces, photonMap &pm, photonMap &pmc, const dir &rayo, 
				double& R, double& G, double& B, const bool &luzPuntual){
		shared_ptr<figura> actualFig = nullptr;
		point colP;
		static int aux = 0;
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
			bool trash;
			dir dirNewRay = actualFig->nextRay(event, rayo, colP, trash);
			// Si es emisor devuelve su RGB directamente
			if(actualFig->isLight()){
				actualFig->getRGB(EMISSION,colP,R,G,B);
			}
			// Si es refracción o reflexión, calcula la interacción, y devuelve el RGB
			// de dicha interacciónn
			else if(event != PHONG && event != EMISSION){
				++aux;
				double r,g,b;
				if(aux > 50){
					colP.x /= 0.99;
					colP.y /= 0.99;
					colP.z /= 0.99;
				}
				getRGB(colP, e,luces, pm, pmc, dirNewRay, R,G,B, luzPuntual);
				actualFig->getRGB(event,colP,r,g,b);
				R *= r; G *= g; B *= b;
			}
			// Si es phong
			else{
				aux = 0;
				// Halla el RGB en el photon map global
				pm.getColorAt(colP,R,G,B);
				double r,g,b;
				// Y le suma el RGB de las cáusticas
				pmc.getColorAt(colP,r,g,b);
				R += r; G += g; B += b;
				// Si hay luces puntuales se calcula la luz directa
				if(luzPuntual){
					luzDirecta(e,luces,dirNewRay,actualFig->getNormal(colP),colP);
					R += r; G += g; B += b;
				}
				actualFig->phongColor(rayo,dirNewRay,colP,r,g,b);
				R *= r; G *= g; B *= b;
			}
		}
	}
    
};

#endif // PHOTONMAPPER_H
