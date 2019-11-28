#ifndef RUSSIANROULETTE_H
#define RUSSIANROULETTE_H

#include <iostream>
#include <math.h>
#include "camara.h"
#include <list>
#include <memory>
#include <limits>

using namespace std;

enum event_enum {PHONG, REFRACTION, REFLEXION, DEATH};

class russianRoulette
{
private:
	event_enum *eventos;
	double *probs;
	int nE = 0;
public:
	russianRoulette(){};
	russianRoulette(event_enum _eventos[], double _probs[]){
		srand(static_cast<unsigned int>(clock()));
		nE = sizeof(event_enum)/sizeof(event_enum)+1;
		eventos = new event_enum[nE]();
		probs = new double[nE]();
		double totalP = 0.0;
		for(int i = 0; i < nE-1; ++i){
			eventos[i] = _eventos[i];
			probs[i] = _probs[i];
			totalP += _probs[i];
		}
		probs[nE-1] = 1 - totalP;
		eventos[nE-1] = DEATH;
	}
	~russianRoulette(){};
	
	event_enum event(){
		double k = (double)rand() / RAND_MAX;
		double aux = 0.0;
		for(int i = 0; i < nE; ++i){
			if(k < aux + probs[i]){
				return eventos[i];
			}
			aux += probs[i];
		}
		return DEATH;
	}
	
	double probEvent(event_enum e){
		double aux = 0.0;
		for(int i = 0; i < nE;++i){
			if(eventos[i] == e){
				aux = probs[i];
			}
		}
		return aux;
	}
};

#endif // RUSSIANROULETTE_H
