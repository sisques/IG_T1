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
		photones.clear();
		photonList.clear();
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
	
	double gaussianFilter(double d, double r){
		double denAux = pow(2.7182818,-1.953);
		double exp = -1.953*d*d/(2*r*r);
		double numAux = pow(7182818,exp);
		return 0.918*(1-((1-numAux)/(1-denAux)));
	}
	
	void getColorAt(const point &p, double &r, double &g, double &b){
		double dist1 = 99999999,dist2 = 99999999,dist3 = 99999999,dist4 = 99999999,dist5 = 99999999;
		photon p1,p2,p3,p4,p5;
		for(pair<point,photon> i:photones){
			if(mod(p-i.first) < 0.1 && mod(p-i.first) < dist1 && dist2 != 99999999 && dist3 != 99999999 && dist4 != 99999999 && dist5 != 99999999){
				p1 = i.second;
				dist1 = mod(p-i.first);
			}
			else if(mod(p-i.first) < 0.1 && mod(p-i.first) < dist2 && dist3 != 99999999 && dist4 != 99999999 && dist5 != 99999999){
				p2 = i.second;
				dist2 = mod(p-i.first);
			}
			else if(mod(p-i.first) < 0.1 && mod(p-i.first) < dist3 && dist4 != 99999999 && dist5 != 99999999){
				p3 = i.second;
				dist3 = mod(p-i.first);
			}
			else if(mod(p-i.first) < 0.1 && mod(p-i.first) < dist4 && dist5 != 99999999){
				p4 = i.second;
				dist4 = mod(p-i.first);
			}
			else if(mod(p-i.first) < 0.1 && mod(p-i.first) < dist5){
				p5 = i.second;
				dist5 = mod(p-i.first);
			}
		}
		double maxD = max(max(max(max(dist1,dist2),dist3),dist4),dist5);
		r = (p1.flow*p1.R*(1-dist1/maxD) + p2.flow*p2.R*(1-dist2/maxD) + p3.flow*p3.R*(1-dist3/maxD) + p4.flow*p4.R*(1-dist4/maxD) + p5.flow*p5.R*(1-dist5/maxD));
		g = (p1.flow*p1.G*(1-dist1/maxD) + p2.flow*p2.G*(1-dist2/maxD) + p3.flow*p3.G*(1-dist3/maxD) + p4.flow*p4.G*(1-dist4/maxD) + p5.flow*p5.G*(1-dist5/maxD));
		b = (p1.flow*p1.B*(1-dist1/maxD) + p2.flow*p2.B*(1-dist2/maxD) + p3.flow*p3.B*(1-dist3/maxD) + p4.flow*p4.B*(1-dist4/maxD) + p5.flow*p5.B*(1-dist5/maxD));
		
		//r = (p1.flow*p1.R*gaussianFilter(dist1,maxD) + p2.flow*p2.R*gaussianFilter(dist1,maxD) + p3.flow*p3.R*gaussianFilter(dist1,maxD) + p4.flow*p4.R*gaussianFilter(dist1,maxD) + p5.flow*p5.R*gaussianFilter(dist1,maxD));
		//g = (p1.flow*p1.G*gaussianFilter(dist1,maxD) + p2.flow*p2.G*gaussianFilter(dist1,maxD) + p3.flow*p3.G*gaussianFilter(dist1,maxD) + p4.flow*p4.G*gaussianFilter(dist1,maxD) + p5.flow*p5.G*gaussianFilter(dist1,maxD));
		//b = (p1.flow*p1.B*gaussianFilter(dist1,maxD) + p2.flow*p2.B*gaussianFilter(dist1,maxD) + p3.flow*p3.B*gaussianFilter(dist1,maxD) + p4.flow*p4.B*gaussianFilter(dist1,maxD) + p5.flow*p5.B*gaussianFilter(dist1,maxD));
		
		double A = M_PI*maxD*maxD;
		double k = 1.0;
		r = r/((1-2/(3*k))*A);
		g = g/((1-2/(3*k))*A);
		b = b/((1-2/(3*k))*A);
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
