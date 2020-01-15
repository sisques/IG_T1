#ifndef RUSSIANROULETTE_H
#define RUSSIANROULETTE_H

#include <iostream>
#include <cmath>
#include "camara.h"
#include <list>
#include <memory>
#include <limits>

using namespace std;

// Declaración de los tipos de evento
// Para emissión no se usa ruleta, ya que un objeto emisor no
// refleja ni refracta ni tiene propiedades difusas
enum event_enum {PHONG, REFRACTION, REFLEXION, EMISSION, DEATH};

// Clase para realizar la selección de eventos mediante el método
// de la ruleta rusa, que selecciona cada evento con una probabilidad
class russianRoulette
{
private:
	// Número de eventos
    int nE = 4;
	// Véctor con los eventos en un orden dado
    event_enum eventos[4];
	// Véctor con las probabilidades de cada evento
    double probs[4];

public:
	// Constructor por defecto
    russianRoulette(){};
	
	// Constructor que pide el vector de eventos y sus probabilidades
	// Espera que sean de tamaño 3, es decir, para phong, refracción y
	// refracción
    russianRoulette(event_enum _eventos[], double _probs[]){
		// Inicializa la semilla de la ruleta
        srand(static_cast<unsigned int>(clock()));
        double totalP = 0.0;
		// Guarda el orden de eventos y sus probabilidades
        for(int i = 0; i < nE-1; ++i){
            eventos[i] = _eventos[i];
            probs[i] = _probs[i];
            totalP += _probs[i];
        }
		// Normaliza las probabilidades sobre 0.9
		for(int i = 0; i < nE; ++i){
            probs[i] = 0.9*probs[i]/totalP;
        }
		// Se establece un 0.1 de probabilidad de muerte
        probs[nE-1] = 0.1;
        eventos[nE-1] = DEATH;
    }

	// Destructor
    ~russianRoulette(){}

	// Devuelve un evento aleatorio, con probs como distribución
	// de probabilidades de cada evento
    event_enum event(){
        double k = (double)rand() / RAND_MAX;
        double aux = 0.0;
		// Para todos los eventos
        for(int i = 0; i < nE; ++i){
			// Si el aleatorio pertenece a la franja del evento
			// se devuelve dicho evento
            if(k < aux + probs[i]){
                return eventos[i];
            }
            aux += probs[i];
        }
		// Por defecto muerte
        return DEATH;
    }

	// Devuelve la probabilidad asociada a un evento
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
