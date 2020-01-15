#ifndef PHOTONMAP_H
#define PHOTONMAP_H

#include <iostream>
#include <list>
#include <map>
#include "punto_direccion.h"
#include "photon.h"
#include "kdTree.h"
#include <mutex>
#include <condition_variable>

using namespace std;

class photonMap
{
private:
	list<pair<point,photon>> photones;
	list<photon> photonList;
	KDTree tree;
	
public:
	photonMap(){}
	
	photonMap(KDTree t){
		tree = t;
	}

	void addPhoton(const point &p, photon ph){
		photones.push_back(pair<point,photon>(p,ph));
		photonList.push_back(ph);
	}
	
	void clear(){
		photones.clear();
		photonList.clear();
	}
	
	void generateTree(){
		tree = KDTree(photonList);
		//photones.clear();
		//photonList.clear();
	}
	
	KDTree generateTreeAux(){
		return KDTree(photonList);
	}
	
	double gaussianFilter(double d, double r){
		double denAux = pow(2.7182818,-1.953);
		double exp = -1.953*d*d/(2*r*r);
		double numAux = pow(7182818,exp);
		return 0.918*(1-((1-numAux)/(1-denAux)));
	}
	
	void getColorAt2(const point &p, double &r, double &g, double &b){
		r = 0; g = 0; b = 0;
		list<photon> aux = tree.fotonesCercanos(p,1);
		double maxD = 0;
		//cout << "Kd tree" << endl;
		for(photon ph:aux){
			if(mod(ph.p-p) > maxD){maxD = mod(p-ph.p);}
			//cout << ph.p.x << " " << ph.p.y << " " << ph.p.z << endl;
			//cout <<  mod(p-ph.p) << endl;
		}
		for(photon ph:aux){
			r += ph.flow*ph.R*(1-(mod(p-ph.p)/maxD));
			g += ph.flow*ph.G*(1-(mod(p-ph.p)/maxD));
			b += ph.flow*ph.B*(1-(mod(p-ph.p)/maxD));
		}
		double A = M_PI*maxD*maxD;
		double k = 1.0;
		r = r/((1-2/(3*k))*A);
		g = g/((1-2/(3*k))*A);
		b = b/((1-2/(3*k))*A);
		double mAux = max(r, max(g,b));
		if(mAux > 1){
			r = r/mAux;
			g = g/mAux;
			b = b/mAux;
		}
	}
	
	void getPhotonAt(const point &p, double &r, double &g, double &b){
		list<photon> aux = tree.fotonesCercanos(p,1);
		if(mod(p-aux.front().p) < 0.001){
			r = 1; g = 1; b = 1;
		}
		else{
			r = 0; g = 0; b = 0;
		}
	}
	
	void getColorAt3(const point &p, double &r, double &g, double &b){
		list<photon> aux = tree.fotonesCercanos(p,10);
		double maxD = 0;
		for(photon ph:aux){
			if(mod(p-ph.p) > maxD){maxD = mod(p-ph.p);}
		}
		for(photon ph:aux){
			r += ph.R*ph.flow*(1-mod(p-ph.p)/maxD);
			g += ph.G*ph.flow*(1-mod(p-ph.p)/maxD);
			b += ph.B*ph.flow*(1-mod(p-ph.p)/maxD);
		}
		double A = M_PI*maxD*maxD;
		double k = 1.0;
		r = r/((1-2/(3*k))*A);
		g = g/((1-2/(3*k))*A);
		b = b/((1-2/(3*k))*A);
		double mAux = max(r, max(g,b));
		if(mAux > 1){
			r = r/mAux;
			g = g/mAux;
			b = b/mAux;
		}
	}
	
	void getColorAt(const point &p, double &r, double &g, double &b){
		r = 0; g = 0; b = 0;
		if(photones.empty()){
			return;
		}
		int N = 50, last = 0;;
		double dist[N], maxD = 0;
		photon lista[N];
		for(int i = 0; i < N; ++i){dist[i] = 99999999;}
		for(pair<point,photon> i:photones){
			if(last < N-1){
				lista[last] = i.second;
				dist[last+1] = mod(p-i.first);
				if(dist[last+1] > maxD){maxD = dist[last+1];}
				++last;
			}
			else{
				photon aux = i.second;
				double distAux = mod(p-i.first);
				for(int j = 0; j < N; ++j){
					if(distAux < dist[j]){
						photon aux2 = lista[j];
						double disAux2 = dist[j];
						lista[j] = aux;
						dist[j] = distAux;
						aux = aux2;
						distAux = disAux2;
					}
				}
			}
		}
		//cout << "Lista" << endl;
		for(int i = 0; i < last; ++i){
			//cout << lista[i].p.x << " " << lista[i].p.y << " " << lista[i].p.z << endl;
			//cout << mod(p-lista[i].p) << endl;
			r += lista[i].R*lista[i].flow*(1-mod(p-lista[i].p)/maxD);
			g += lista[i].G*lista[i].flow*(1-mod(p-lista[i].p)/maxD);
			b += lista[i].B*lista[i].flow*(1-mod(p-lista[i].p)/maxD);
		}
		double A = M_PI*maxD*maxD;
		double k = 1.0;
		r = r/((1-2/(3*k))*A);
		g = g/((1-2/(3*k))*A);
		b = b/((1-2/(3*k))*A);
		double mAux = max(r, max(g,b));
	}
	
	void imprimir(){
		for(pair<point,photon> i:photones){
			if(i.first.x <= 1 && i.first.y <= 1 && i.first.z <= 1){
			cout << "x = " << i.first.x << " y = " << i.first.y << " z = " << i.first.z << endl;
			cout << " r = " << i.second.R << " g = " << i.second.G << " b = " << i.second.B << endl << endl;}
		}
	}
};

#endif // PHOTONMAP_H
