#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "punto_direccion.h"
#include <iostream>
#include <math.h>
#include "perlinNoise.h"
#include "ppm_reader.h"

using namespace std;

enum textures { flatColor = 0 , psychedelic = 1, concentricCircles = 2,
				distanceBased = 3, wood = 4, fuzzy = 5, waterPlain = 6,
				circularTransparence = 7, imagePlain = 8}

class texture{
protected:
	int cr = 256
public:
    texture(){}
	texture(int _cr){cr = _cr;}

	virtual int getR(point p, int r){ return r;}
    virtual int getG(point p, int g){ return g;}
    virtual int getB(point p, int b){ return b;}
	
	virtual int getR(point c, point p, int r){ return r;}
    virtual int getG(point c, point p, int g){ return g;}
    virtual int getB(point c, point p, int b){ return b;}
	
	virtual int getR(point c, point p){ return cr-1;}
    virtual int getG(point c, point p){ return cr-1;}
    virtual int getB(point c, point p){ return cr-1;}
};


//Efecto psicodélico basado en senos
class texture1 : public texture {
public:
	texture1():texture(){}
	texture1(int _cr):texture(_cr){}
	
	int getR(point p, int r) override {
		double c = sin(p.x)*sin(p.z);
		int aux = (r*c+(97*this->cr)/256);
		return aux%this->cr;
	}
    int getG(point p, int g) override {
		double c = sin(p.x)*sin(p.z);
		int aux = (g*c+(34*this->cr)/256);
		return aux%this->cr;
	}
    int getB(point p, int b) override {
		double c = sin(p.x)*sin(p.z);
		int aux = (b*c+(65*this->cr)/256);
		return aux%this->cr;
	}
};

//Circulos concéntricos basados en un centro
class texture2 : public texture {
public:
	texture2():texture(){}
	texture2(int _cr):texture(_cr){}
	
	int getR(point c, point p, int r) override {
		int d = dist(c,p);
		if((d/10)%2 == 1){
			return abs((r+(85*this->)/256)%this->cr);
		}
		return r;
	}
    int getG(point c, point p, int g) override {
		int d = dist(c,p);
		if((d/10)%2 == 1){
			return abs((g-(53*this->)/256)%this->cr);
		}
		return g;
	}
    int getB(point c, point p, int b) override {
		int d = dist(c,p);
		if((d/10)%2 == 1){
			return abs((b+(85*this->cr)/256)%this->cr);
		}
		return b;
	}
};

//Degradado basado en la distancia a la camara
class texture3 : public texture {
public:
	texture3():texture(){}
	texture3(int _cr):texture(_cr){}
	
	int getR(point c, point p, int r) override {
		int d = 5*dist(c,p)*(this->cr/256.0);
		return abs(r-d)%this->cr;
	}
    int getG(point c, point p, int g) override {
		int d = 5*dist(c,p)*(this->cr/256.0);
		return abs(g-d)%this->cr;
	}
    int getB(point c, point p, int b) override {
		int d = 5*dist(c,p)*(this->cr/256.0);
		return abs(b-d)%this->cr;
	}
};

//Madera
class texture4 : public texture {
private:
	perlinNoise pn;
public:
	texture4():texture(){
		pn = perlinNoise();
	}
	texture4(int _cr):texture(_cr){
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
private:
	perlinNoise pn;
public:
	texture5():texture(){
		pn = perlinNoise();
	}
	texture5(int _cr):texture(_cr){
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
private:
	perlinNoise pn;
public:
	texture6():texture(){
		pn = perlinNoise();
	}
	texture6(int _cr):texture(_cr){
		pn = perlinNoise();
	}
	
	int getR(point c, point p, int r) override {
		double n = pn.noise(p.x/200, p.y/200, p.z/200);
		n *= n - floor(n);
		int d = dist(c,p);
		if((d/200)%2 == 1){
			if(r+(50*this->cr)/256 > this->cr-1){return this->cr-1;}
			return (r+(50*this->cr)/256)*n;
		}
		return floor(r * n);
	}
    int getG(point c, point p, int g) override {
		double n = 20 * pn.noise(p.x/200, p.y/200, p.z/200);
		n = n - floor(n);
		int d = dist(c,p);
		if((d/200)%2 == 1){
			if(g+(25*this->cr)/256 > this->cr-1){return this->cr-1;}
			return (g+(25*this->cr)/256)*n;
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
public:
	texture7():texture(){}
	texture7(int _cr):texture(_cr){}
	
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

//Imagen plano
class texture8 : public texture {
private:
	int **Rs;
	int **Gs;
	int **Bs;
	int width, height;
	dir n;
public:
	texture8(string im, dir _n):texture(){
		n = _n;
		list<int*> pixels = ppmReader(im);
		width = pixels.front()[0]; 
		height = pixels.front()[1];
		pixels.pop_front();
		
		Rs = new int*[height];
		for (int i = 0; i < height; ++i){Rs[i] = new int[width];}
		Gs = new int*[height];
		for (int i = 0; i < height; ++i){Gs[i] = new int[width];}
		Bs = new int*[height];
		for (int i = 0; i < height; ++i){Bs[i] = new int[width];}
		
		for(int i = 0; i < height; ++i){
			for(int j = 0; j < width; ++j){
				Rs[i][j] = pixels.front()[0]; 
				Gs[i][j] = pixels.front()[1]; 
				Bs[i][j] = pixels.front()[2];
				pixels.pop_front();
			}
		}
		
	}
	~texture8(){
		for (int i = 0; i < height; ++i){
			delete [] Rs[i];
		}
		delete [] Rs;
		for (int i = 0; i < height; ++i){
			delete [] Gs[i];
		}
		delete [] Gs;
		for (int i = 0; i < height; ++i){
			delete [] Bs[i];
		}
		delete [] Bs;
	}
	int getR(point c, point p) override {
		dir aux = c - p;
		int x = (int)aux.x%width;
		int y = (int)aux.y%height;
		if(n.x == 0 && n.z == 0){
			x = (int)aux.x%width;
			y = (int)aux.z%height;
		}
		else if(n.y == 0 && n.z == 0){
			x = (int)aux.z%width;
			y = (int)aux.y%height;
		}
		if(x < 0){x = width + x;}
		if(y < 0){y = height + y;}
		return (Rs[y][x]*this->cr)/256;
	}
    int getG(point c, point p) override {
		dir aux = c - p;
		int x = (int)aux.x%width;
		int y = (int)aux.y%height;
		if(n.x == 0 && n.z == 0){
			x = (int)aux.x%width;
			y = (int)aux.z%height;
		}
		else if(n.y == 0 && n.z == 0){
			x = (int)aux.z%width;
			y = (int)aux.y%height;
		}
		if(x < 0){x = width + x;}
		if(y < 0){y = height + y;}
		return (Gs[y][x]*this->cr)/256;
	}
    int getB(point c, point p) override {
		dir aux = c - p;
		int x = (int)aux.x%width;
		int y = (int)aux.y%height;
		if(n.x == 0 && n.z == 0){
			x = (int)aux.x%width;
			y = (int)aux.z%height;
		}
		else if(n.y == 0 && n.z == 0){
			x = (int)aux.z%width;
			y = (int)aux.y%height;
		}
		if(x < 0){x = width + x;}
		if(y < 0){y = height + y;}
		return (Bs[y][x]*this->cr)/256;
	}
};

#endif
