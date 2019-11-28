#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <iostream>
#include <math.h>
#include "camara.h"
#include "russian_roulette.h"
#include <list>
#include <memory>
#include <limits>

using namespace std;

class pathTracer
{
private:

	bool colision(point c, list<shared_ptr<figura>> e, dir rayo, shared_ptr<figura> &fig, point &col){
		double t = 0;
		double distMin = numeric_limits<double>::max();
		double distActual = 0;
		bool colision;
		bool yes = false;
		shared_ptr<figura> nearest;
		for( auto it = e.begin(); it != e.end(); ++it){
			shared_ptr<figura> f = *it;
            colision = f->intersection(rayo, c, t);
            if (colision) {
                point p = c + rayo * t;
                distActual = mod(c - p);
                if (distActual < distMin) {
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

public:
	pathTracer(){}
	~pathTracer(){};
	
	void getRGB(point c, list<shared_ptr<figura>> e,  dir rayo, double& R, double& G, double& B){
		shared_ptr<figura> actualFig;
		point colP;
		bool colisiona = colision(c,e,rayo,actualFig,colP);
		event_enum event;
		if(!colisiona){
			event = DEATH;
		}
		else{
			event = actualFig->evento();
		}
        double R_anterior, G_anterior, B_anterior;
		
		if(colisiona && actualFig->isLight()){
			R = actualFig->getR(colP);
			G = actualFig->getG(colP);
			B = actualFig->getB(colP);
		}
		else if(event == DEATH){
			R = 0;
			G = 0;
			B = 0;
		}
		else if(event == REFRACTION || event == REFLEXION){
			dir dirNewRay;
			
			dirNewRay = actualFig->nextRay(event, rayo, colP);
			
			getRGB(colP,  e,  dirNewRay, R_anterior, G_anterior, B_anterior);

			R = actualFig->getR(colP);
			G = actualFig->getG(colP);
			B = actualFig->getB(colP);
            double luminance = (0.2126*R*1.0 + 0.7152*G*1.0 + 0.0722*B*1.0);
			if(actualFig->isPhong()){
				R = R*(1-actualFig->probEvent(event))+R_anterior*actualFig->probEvent(event);
				G = G*(1-actualFig->probEvent(event))+G_anterior*actualFig->probEvent(event);
				B = B*(1-actualFig->probEvent(event))+B_anterior*actualFig->probEvent(event);
			}
			else{
				R = R*(1-actualFig->probEvent(event))+R_anterior*actualFig->probEvent(event);
				G = G*(1-actualFig->probEvent(event))+G_anterior*actualFig->probEvent(event);
				B = B*(1-actualFig->probEvent(event))+B_anterior*actualFig->probEvent(event);
			}
			R = R;//*luminance;
			G = G;//*luminance;
			B = B;//*luminance;
		}
		else{
			R = actualFig->getR(colP);
			G = actualFig->getG(colP);
			B = actualFig->getB(colP);
		}
	}
	
	double cFunc(const double v, const double min, const double max){
		return (max-min)*v + min;
	}
};

#endif // PATHTRACER_H
