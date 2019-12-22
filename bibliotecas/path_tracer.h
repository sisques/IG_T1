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

class pathTracer : public Renderer
{
public:
    pathTracer(){}
    ~pathTracer(){};

    void getRGB(const point &c, const list<shared_ptr<figura>> &e, const list<shared_ptr<figura>> &luces,
				const dir &rayo, double& R, double& G, double& B, const bool &luzPuntual){
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
            getRGB(colP,  e, luces, dirNewRay, R_siguiente, G_siguiente, B_siguiente, luzPuntual);
			
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
        }
        else{
            actualFig->getRGB(REFLEXION,colP,R,G,B);
        }
    }
};

#endif // PATHTRACER_H
