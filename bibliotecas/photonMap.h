#ifndef PHOTONMAP_H
#define PHOTONMAP_H

#include <iostream>
#include <list>
#include <map>
#include "punto_direccion.h"
#include "photon.h"
#include "kdtree.h"
#include <mutex>
#include <condition_variable>

using namespace std;

// Clase que representa un photon map, el cual contiene todos los
// fotones de una escena, normalmente o de iluminación global o de
// cáusticas
class photonMap
{
private:
	// Lista auxiliar de fotones para formar el kdtree
	list<photon> photonList;
	// Estructura para guardar los fotones de forma eficiente
	kdTree tree;
	// Número de fotones a coger entorno a un punto
	int photonsPerRay = 50;
	
	bool vacio = false;
	
public:
	// Costructor por defecto
	photonMap(){}
	// Constructor a partir de un árbol
	photonMap(kdTree t, int size){
		if(size > 0){
			tree = t;
		}
		else{
			vacio = true;
		}
	}
	// Configurar el número de fotones por rayo
	void setPhotonsPerRay(int ppr){ photonsPerRay = ppr;}
	
	// Devuelve el número de fotones
	// Solo funciona si no se ha hecho clear
	int size(){return photonList.size();}

	// Añade un fotón a la estructura
	void addPhoton(const point &p, photon ph){
		photonList.push_back(ph);
	}
	
	// Borra la lista auxiliar
	void clear(){
		photonList.clear();
	}
	
	// Genera el árbol y vacía la lista auxiliar
	void generateTree(){
		tree = kdTree(photonList);
		photonList.clear();
	}
	
	// Devuelve un árbol generado a partir de la lista auxiliar
	kdTree generateTreeAux(){
		if(photonList.size() > 0){
			return kdTree(photonList);
		}
		else{
			return kdTree();
		}
	}
	
	// Filtro gaussiano para los fotones capturados
	double gaussianFilter(double d, double r){
		double denAux = pow(2.7182818,-1.953);
		double exp = -1.953*d*d/(2*r*r);
		double numAux = pow(7182818,exp);
		return 0.918*(1-((1-numAux)/(1-denAux)));
	}
	
	//  Devuelve en r,g y b el RGB resultante de estimar la radiancia
	// de los photonsPerRay fotones más cercanos al punto p
	void getColorAt(const point &p, double &r, double &g, double &b){
		r = 0; g = 0; b = 0;
		if(vacio){return;}
		// Obtiene los photonsPerRay fotones más cercanos
		list<photon> aux = tree.fotonesCercanos(p,photonsPerRay);
		double maxD = 0;
		// Haya la distancia del fotón más lejano
		for(photon ph:aux){
			if(mod(ph.p-p) > maxD){maxD = mod(p-ph.p);}
		}
		// Estima la radiandia aplicando el filtrado cónico
		for(photon ph:aux){
			if(aux.size() > 1){
				r += ph.flow*ph.R*(1-(mod(p-ph.p)/maxD));
				g += ph.flow*ph.G*(1-(mod(p-ph.p)/maxD));
				b += ph.flow*ph.B*(1-(mod(p-ph.p)/maxD));
			}
			else{
				r += ph.flow*ph.R;
				g += ph.flow*ph.G;
				b += ph.flow*ph.B;
			}
		}
		// Haya el área
		double A = M_PI*maxD*maxD;
		double k = 1.0;
		// Divide por el área, aplicando la normalización del filtrado cónico
		if(maxD > 0){
			r = r/((1-2/(3*k))*A);
			g = g/((1-2/(3*k))*A);
			b = b/((1-2/(3*k))*A);
		}
	}
	
	// Devuelve RGB = 1 si hay un fotón muy cercano al punto p, devuelve
	// RGB = 0 en caso contrario
	void getPhotonAt(const point &p, double &r, double &g, double &b){
		list<photon> aux = tree.fotonesCercanos(p,1);
		if(mod(p-aux.front().p) < 0.001){
			r = 1; g = 1; b = 1;
		}
		else{
			r = 0; g = 0; b = 0;
		}
	}
	
};

#endif // PHOTONMAP_H
