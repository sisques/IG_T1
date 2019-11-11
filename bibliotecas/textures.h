#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "punto_direccion.h"
#include <iostream>
#include <math.h>

using namespace std;

class texture{
public:
    texture(){}

	virtual int getR(point p, int r){ return r;}
    virtual int getG(point p, int g){ return g;}
    virtual int getB(point p, int b){ return b;}
	
	virtual int getR(point c, point p, int r){ return r;}
    virtual int getG(point c, point p, int g){ return g;}
    virtual int getB(point c, point p, int b){ return b;}
};



class texture1 : public texture {
public:
	int getR(point p, int r) override {
		double c = sin(p.x)*sin(p.z);
		int aux = (r*c+97);
		return aux%256;
	}
    int getG(point p, int g) override {
		double c = sin(p.x)*sin(p.z);
		int aux = (g*c+34);
		return aux%256;
	}
    int getB(point p, int b) override {
		double c = sin(p.x)*sin(p.z);
		int aux = (b*c+65);
		return aux%256;
	}
};

class texture2 : public texture {
public:
	int getR(point c, point p, int r) override {
		int d = dist(c,p);
		if((d/10)%2 == 1){
			return abs((r+85)%256);
		}
		return r;
	}
    int getG(point c, point p, int g) override {
		int d = dist(c,p);
		if((d/10)%2 == 1){
			return abs((g-53)%256);
		}
		return g;
	}
    int getB(point c, point p, int b) override {
		int d = dist(c,p);
		if((d/10)%2 == 1){
			return abs((b+85)%256);
		}
		return b;
	}
};

class texture3 : public texture {
public:
	int getR(point c, point p, int r) override {
		int d = dist(c,p);
		return abs(r-d)%256;
	}
    int getG(point c, point p, int g) override {
		int d = dist(c,p);
		return abs(g-d)%256;
	}
    int getB(point c, point p, int b) override {
		int d = dist(c,p);
		return abs(b-d)%256;
	}
};

#endif
