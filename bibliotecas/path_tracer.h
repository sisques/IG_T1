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
public:
	pathTracer(){}
	~pathTracer(){};
	
	void getRGB(camara c, list<shared_ptr<figura>> e,  dir rayo, int& R, int& G, int& B){
		double t = 0;
		double delta = 0;
		double distMin = numeric_limits<double>::max();
		double distActual = 0;
		bool colision;
		shared_ptr<figura> nearest;
		point colP;
		for( auto it = e.begin(); it != e.end(); ++it){
			shared_ptr<figura> f = *it;
            colision = f->intersection(rayo, c.o, t, delta);
            if (colision) {
                point p = c.o + rayo * (t - delta);
                distActual = mod(c.o - p);
                if (distActual < distMin && f->getR(p) != -1 && f->getG(p) != -1 && f->getB(p) != -1) {
                    nearest = f;
                    distMin = distActual;
                    colP = p;
                }
            }

		}
		event_enum event = nearest->evento();
		int auxR, auxG, auxB;
		if(event == DEATH){
			R = 0;
			G = 0;
			B = 0;
		}
		else if(event == REFRACTION /*||*/&& event == REFLEXION){
			dir d; 
			if(event == REFRACTION){/* d = nearest.nextRay(REFRACTION, d, p);*/}
			else{/* d = nearest.nextRay(REFLEXION, d, p);*/}
			bool colisiona = false;
			if(colisiona /* && figuraConLaQueIntersecciona.isLight()*/){
				R = nearest->getR(colP);
				G = nearest->getG(colP);
				B = nearest->getB(colP);
			}
			else if(colisiona){
				getRGB(c,  e,  dd, auxR, auxG, auxB);
				if(nearest->isPhong()){
					if(event == REFRACTION){
						
					}
					else{
						
					}
				}
				else{
					if(event == REFRACTION){
						
					}
					else{
						
					}
				}
			}
			else{
				R = 0;
				G = 0;
				B = 0;
			}
		}
		else{
			R = nearest->getR(colP);
			G = nearest->getG(colP);
			B = nearest->getB(colP);
		}
	}
	
	double cFunc(const double v, const double min, const double max){
		return (max-min)*v + min;
	}
};

#endif // PATHTRACER_H
