#ifndef PHOTONMAP_H
#define PHOTONMAP_H

#include <iostream>
#include <list>
#include <map>
#include "punto_direccion.h"
#include "photon.h"
#include "kdtree.h"

using namespace std;

class photonMap
{
private:
	list<pair<point,photon>> photones;
	
	list<photon> photonList;
	
	kdtree *tree;
	
public:
	void addPhoton(const point &p, photon ph){
		photones.push_back(pair<point,photon>(p,ph));
		//photonList.push_back(ph);
	}
	
	void generateTree(){
		tree = new kdtree(photonList);
	}
	
	void getColorAt2(const point &p, double &r, double &g, double &b){
		photon phaux;phaux.p = p;
		list<photon> aux = tree->cercanos(phaux,photonList,5);
		for(photon ph:aux){
			r += ph.R;
			g += ph.G;
			b += ph.B;
		}
		r /= aux.size();
		g /= aux.size();
		b /= aux.size();
	}
	
	void getColorAt3(const point &p, double &r, double &g, double &b){
		map<double,photon> maximos;
		int nPh = 5;
		for(pair<point,photon> i:photones){
			if(maximos.size() < nPh){
				maximos.insert(pair<double,photon>(mod(p-i.first),i.second));
			}
			else{
				double photonToReplace = 99999999;
				for(pair<int,photon> j:maximos){
					if(j.first > mod(p-i.first) && j.first < photonToReplace){
						photonToReplace = j.first;
					}
				}
				if(photonToReplace != 99999999){
					maximos.erase(photonToReplace);
					maximos.insert(pair<double,photon>(mod(p-i.first),i.second));
				}
			}
		}
		for(pair<int,photon> i:maximos){
			r += i.second.R;
			g += i.second.G;
			b += i.second.B;
		}
		r /= maximos.size();
		g /= maximos.size();
		b /= maximos.size();
	}
	
	void getColorAt(const point &p, double &r, double &g, double &b){
		double dist1 = 99999999,dist2 = 99999999,dist3 = 99999999,dist4 = 99999999,dist5 = 99999999;
		double r1 = 0, g1 = 0, b1 = 0;
		double r2 = 0, g2 = 0, b2 = 0;
		double r3 = 0, g3 = 0, b3 = 0;
		double r4 = 0, g4 = 0, b4 = 0;
		double r5 = 0, g5 = 0, b5 = 0;
		for(pair<point,photon> i:photones){
			if(mod(p-i.first) < 0.1 && mod(p-i.first) < dist1 && dist2 != 99999999 && dist3 != 99999999 && dist4 != 99999999 && dist5 != 99999999){
				r1 = i.second.R;
				g1 = i.second.G;
				b1 = i.second.B;
				dist1 = mod(p-i.first);
			}
			else if(mod(p-i.first) < 0.1 && mod(p-i.first) < dist2 && dist3 != 99999999 && dist4 != 99999999 && dist5 != 99999999){
				r2 = i.second.R;
				g2 = i.second.G;
				b2 = i.second.B;
				dist2 = mod(p-i.first);
			}
			else if(mod(p-i.first) < 0.1 && mod(p-i.first) < dist3 && dist4 != 99999999 && dist5 != 99999999){
				r3 = i.second.R;
				g3 = i.second.G;
				b3 = i.second.B;
				dist3 = mod(p-i.first);
			}
			else if(mod(p-i.first) < 0.1 && mod(p-i.first) < dist4 && dist5 != 99999999){
				r4 = i.second.R;
				g4 = i.second.G;
				b4 = i.second.B;
				dist4 = mod(p-i.first);
			}
			else if(mod(p-i.first) < 0.1 && mod(p-i.first) < dist5){
				r5 = i.second.R;
				g5 = i.second.G;
				b5 = i.second.B;
				dist5 = mod(p-i.first);
			}
		}
		double maxD = max(max(max(max(dist1,dist2),dist3),dist4),dist5);
		r = (r1/(dist1/maxD) + r2/(dist2/maxD) + r3/(dist3/maxD) + r4/(dist4/maxD) + r5/(dist5/maxD))/5;
		g = (g1/(dist1/maxD) + g2/(dist2/maxD) + g3/(dist3/maxD) + g4/(dist4/maxD) + g5/(dist5/maxD))/5;
		b = (b1/(dist1/maxD) + b2/(dist2/maxD) + b3/(dist3/maxD) + b4/(dist4/maxD) + b5/(dist5/maxD))/5;
		if(max(r, max(g,b)) > 1){
			r = r/max(r, max(g,b));
			g = g/max(r, max(g,b));
			b = b/max(r, max(g,b));
		}
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
