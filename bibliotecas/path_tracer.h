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
		double delta = 0;
		double distMin = numeric_limits<double>::max();
		double distActual = 0;
		bool colision = true;
		bool yes = false;
		shared_ptr<figura> nearest;
		for( auto it = e.begin(); it != e.end(); ++it){
			shared_ptr<figura> f = *it;
            colision = f->intersection(rayo, c, t, delta);
            if (colision) {
                point p = c + rayo * (t - delta);
                distActual = mod(c - p);
                if (distActual < distMin && f->getR(p) != -1 && f->getG(p) != -1 && f->getB(p) != -1) {
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
	
	void getRGB(point c, list<shared_ptr<figura>> e,  dir rayo, int& R, int& G, int& B){
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
		int auxR, auxG, auxB;
		
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
			
			getRGB(colP,  e,  dirNewRay, auxR, auxG, auxB);
			double luminance = (0.2126*auxR*1.0 + 0.7152*auxG*1.0 + 0.0722*auxB*1.0);
			R = actualFig->getR(colP);
			G = actualFig->getG(colP);
			B = actualFig->getB(colP);
			if(actualFig->isPhong()){
				R = R*(1-actualFig->probEvent(event))+auxR*actualFig->probEvent(event);
				G = G*(1-actualFig->probEvent(event))+auxG*actualFig->probEvent(event);
				B = B*(1-actualFig->probEvent(event))+auxB*actualFig->probEvent(event);
			}
			else{
				R = R*(1-actualFig->probEvent(event))+auxR*actualFig->probEvent(event);
				G = G*(1-actualFig->probEvent(event))+auxG*actualFig->probEvent(event);
				B = B*(1-actualFig->probEvent(event))+auxB*actualFig->probEvent(event);
			}
			R = (double)R*luminance;
			G = (double)G*luminance;
			B = (double)B*luminance;
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
