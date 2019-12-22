#ifndef PHOTONMAP_H
#define PHOTONMAP_H

#include <iostream>
#include <list>
#include "punto_direccion.h"

using namespace std;

class photonMap
{
private:
	list<pair<point,dir>> photones;
public:
	void addPhoton(const point &p, const double &r, const double &g, const double &b){
		photones.push_back(pair<point,dir>(p,newDir(r,g,b)));
	}
	
	void getColorAt(const point &p, double &r, double &g, double &b){
		double dist1 = 99999999,dist2 = 99999999,dist3 = 99999999;
		double r1 = 0, g1 = 0, b1 = 0;
		double r2 = 0, g2 = 0, b2 = 0;
		double r3 = 0, g3 = 0, b3 = 0;
		for(pair<point,dir> i:photones){
			if(mod(p-i.first) < dist1 && mod(p-i.first) < 1){
				r1 = i.second.x;
				g1 = i.second.y;
				b1 = i.second.z;
			}
			else if(mod(p-i.first) < dist2 && mod(p-i.first) < 1){
				r2 = i.second.x;
				g2 = i.second.y;
				b2 = i.second.z;
			}
			else if(mod(p-i.first) < dist3 && mod(p-i.first) < 1){
				r3 = i.second.x;
				g3 = i.second.y;
				b3 = i.second.z;
			}
		}
		r = (r1 + r2 + r3)/3;
		g = (g1 + g2 + g3)/3;
		b = (b1 + b2 + b3)/3;
	}
	
	void imprimir(){
		for(pair<point,dir> i:photones){
			if(i.first.x <= 1 && i.first.y <= 1 && i.first.z <= 1){
			cout << "x = " << i.first.x << " y = " << i.first.y << " z = " << i.first.z << endl;
			cout << " r = " << i.second.x << " g = " << i.second.y << " b = " << i.second.z << endl << endl;}
		}
	}
};

#endif // PHOTONMAP_H
