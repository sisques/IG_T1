#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "punto_direccion.h"
#include <iostream>
#include <math.h>
#include "perlinNoise.h"

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


//Efecto psicodélico basado en senos
class texture1 : public texture {
public:
	texture1():texture(){}
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

//Circulos concéntricos basados en un centro
class texture2 : public texture {
public:
	texture2():texture(){}
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

//Degradado basado en la distancia a la camara
class texture3 : public texture {
public:
	texture3():texture(){}
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

//Madera
class texture4 : public texture {
	perlinNoise pn;
public:
	texture4():texture(){
		pn = perlinNoise();
	}
	int getR(point p, int r) override {
		double n = 20 * pn.noise(p.x/10, p.y/10, p.z/10);
		n = n - floor(n);
		return floor(r * n);
	}
    int getG(point p, int g) override {
		double n = 20 * pn.noise(p.x/10, p.y/10, p.z/10);
		n = n - floor(n);
		return floor(g * n);
	}
    int getB(point p, int b) override {
		double n = 20 * pn.noise(p.x/10, p.y/10, p.z/10);
		n = n - floor(n);
		return floor(b * n);
	}
};

//Emborronao
class texture5 : public texture {
	perlinNoise pn;
public:
	texture5():texture(){
		pn = perlinNoise();
	}
	int getR(point p, int r) override {
		double n = pn.noise(p.x/10, p.y/10, p.z/10);
		return floor(r * n);
	}
    int getG(point p, int g) override {
		double n = pn.noise(p.x/10, p.y/10, p.z/10);
		return floor(g * n);
	}
    int getB(point p, int b) override {
		double n = pn.noise(p.x/10, p.y/10, p.z/10);
		return floor(b * n);
	}
};

//Agua para plano
class texture6 : public texture {
	perlinNoise pn;
public:
	texture6():texture(){
		pn = perlinNoise();
	}
	int getR(point c, point p, int r) override {
		double n = pn.noise(p.x/200, p.y/200, p.z/200);
		n *= n - floor(n);
		int d = dist(c,p);
		if((d/200)%2 == 1){
			if(r+50 > 255){return 255;}
			return (r+50)*n;
		}
		return floor(r * n);
	}
    int getG(point c, point p, int g) override {
		double n = 20 * pn.noise(p.x/200, p.y/200, p.z/200);
		n = n - floor(n);
		int d = dist(c,p);
		if((d/200)%2 == 1){
			if(g+25 > 255){return 255;}
			return (g+25)*n;
		}
		return floor(g * n);
	}
    int getB(point c, point p, int b) override {
		double n = 20 * pn.noise(p.x/200, p.y/200, p.z/200);
		n = n - floor(n);
		return floor(b * n);
	}
};

//Transparencia circular
class texture7 : public texture {
	perlinNoise pn;
public:
	texture7():texture(){
		pn = perlinNoise();
	}
	int getR(point c, point p, int r) override {
		int d = dist(c,p);
		if((d/100)%2 == 1){
			return -1;
		}
		return r;
	}
    int getG(point c, point p, int g) override {
		int d = dist(c,p);
		if((d/100)%2 == 1){
			return -1;
		}
		return g;
	}
    int getB(point c, point p, int b) override {
		int d = dist(c,p);
		if((d/100)%2 == 1){
			return -1;
		}
		return b;
	}
};

#endif
