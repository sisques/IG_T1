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
#include "Renderer.h"

using namespace std;

// Clase hija de renderer que implementa el algoritmo de path tracing
class pathTracer : public Renderer
{
public:
	// Constructor
    pathTracer(){}
	//Destructor
    ~pathTracer(){};

	// Devuelve el rojo, verde y azul correspondientes al camino realizado con origen en c y dirección
	// rayo, aplicando next event estimation en caso de que se le indice que hay luces puntuales en la
	// escena, a través de luzPuntual 
    void getRGB(const point &c, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces,
				const dir &rayo, double& R, double& G, double& B, const bool &luzPuntual){
        shared_ptr<figura> actualFig = nullptr;
        point colP;
		
		// Comprobamos si el rayo intersecta con algún objeto de la escena
        bool colisiona = colision(c,e,rayo,actualFig,colP);
		
        event_enum event;
		// Si no colisiona, el evento correspondiente es la muerte del rayo
        if(!colisiona){
            event = DEATH;
        }
		// Si colisiona, se selecciona evento a través de la ruleta rusa del objeto
        else{
            event = actualFig->evento();
        }
        double R_siguiente, G_siguiente, B_siguiente;
		
		// Si ha colisionado con la luz, obtiene el RGB de la luz y los devuelve en R, G y B
        if(colisiona && actualFig->isLight()){
            actualFig->getRGB(EMISSION,colP, R,G,B);
        }
		// Si el rayo muere devuelve RGB = 0
        else if(event == DEATH){
            R = 0;
            G = 0;
            B = 0;
        }
		// En caso de que haya habido refracción, reflexión o difusión
        else if(event == REFRACTION || event == REFLEXION || event == PHONG){
			dir luz = newDir(0.0,0.0,0.0);
			// Calculamos la luz directa por las luces puntuales de la escena
			// si es que las hay
			if(luzPuntual){
				luz =  luzDirecta(e, luces,actualFig->getNormal(colP),colP);
			}
            dir dirNewRay;
            double p = 0.0;
			// Calculamos el nuevo rayo según el evento con la figura colisionada
			dirNewRay = actualFig->nextRay(event, rayo, colP);
			p = actualFig->probEvent(event);
			// Obtenemos el RGB del resto del camino
            getRGB(colP,  e, luces, dirNewRay, R_siguiente, G_siguiente, B_siguiente, luzPuntual);
			dir n = actualFig->getNormal(colP);
			// Si el evento es distinto de phong no tenemos en cuenta la luz directa
			// ya que se refleja con la intensidad de lo reflejado, y se refracta con
			// la instensidad de lo refractado
			if(event != PHONG){
				luz.x = R_siguiente * abs(dot(n,dirNewRay));
				luz.y = G_siguiente * abs(dot(n,dirNewRay));
				luz.z = B_siguiente * abs(dot(n,dirNewRay));
			}
			// Si es phong se le suma la luz directa al RGB obtenido del resto del camino
			else{
				luz.x = (luz.x + R_siguiente * abs(dot(n,dirNewRay)));
				luz.y = (luz.y + G_siguiente * abs(dot(n,dirNewRay)));
				luz.z = (luz.z + B_siguiente * abs(dot(n,dirNewRay)));
			}
			// Obtenemos el RGB de la figura colisionada
            if(event == PHONG){
				actualFig->phongColor(rayo,dirNewRay,colP,R,G,B);
            }
            else{
				actualFig->getRGB(event,colP,R,G,B);
            }
			// Dividimos por la probabilidad del evento
			R = R*luz.x/p;
			G = G*luz.y/p;
			B = B*luz.z/p;
        }
    }
};

#endif // PATHTRACER_H
