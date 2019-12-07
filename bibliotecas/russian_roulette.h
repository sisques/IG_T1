#ifndef RUSSIANROULETTE_H
#define RUSSIANROULETTE_H

#include <iostream>
#include <cmath>
#include "camara.h"
#include <list>
#include <memory>
#include <limits>

using namespace std;

enum event_enum {PHONG, REFRACTION, REFLEXION, EMISSION, DEATH};

class russianRoulette
{
private:
    int nE = 4;
    event_enum eventos[4];
    double probs[4];

public:
    russianRoulette(){};
    russianRoulette(event_enum _eventos[], double _probs[]){
        srand(static_cast<unsigned int>(clock()));
        double totalP = 0.0;
        for(int i = 0; i < nE; ++i){
            eventos[i] = _eventos[i];
            probs[i] = _probs[i];
            totalP += _probs[i];
        }
		for(int i = 0; i < nE; ++i){
            probs[i] = 0.9*probs[i]/totalP;
        }
        probs[nE] = 0.1;
        eventos[nE] = DEATH;
    }


    ~russianRoulette(){}

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
