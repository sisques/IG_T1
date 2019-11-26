#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "punto_direccion.h"
#include <iostream>
#include <math.h>
#include "perlinNoise.h"
#include "ppm_reader.h"
#include <list>
#include "matrix.h"

using namespace std;

enum texture_enum { NO_TEXTURE, WOOD, PERLIN_NOISE, IMAGE};

class texture{
protected:
	int cr = 256;
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

//Madera
class texture1 : public texture {
private:
	perlinNoise pn;
public:
	texture1():texture(){
		pn = perlinNoise();
	}
	texture1(int _cr):texture(_cr){
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

//Perlin_noise
class texture2 : public texture {
private:
	perlinNoise pn;
public:
	texture2():texture(){
		pn = perlinNoise();
	}
	texture2(int _cr):texture(_cr){
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

//Imagen
class texture3 : public texture {
private:
	int **Rs;
	int **Gs;
	int **Bs;
	int width, height;
	dir n;
	Matrix toPlane = Matrix().empty();


	void setMatrix(point c, point p){
		dir x = c - p;
		dir z = n;
		dir y = cross(x,z);
		x = cross(y,z);
		toPlane = newBase(x, y, z, c);
	}

public:
	texture3(string im, dir _n):texture(){
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
	~texture3(){
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

		if(toPlane.isEmpty()){setMatrix(c,p);}

		int x = (int)(toPlane*p).x % width;
		int y = (int)(toPlane*p).y % height;

		if(x < 0){x = width + x;}
		if(y < 0){y = height + y;}
		if(Rs[y][x] == -1){return -1;}
		return (Rs[y][x]*this->cr)/256;
	}
    int getG(point c, point p) override {

		if(toPlane.isEmpty()){setMatrix(c,p);}

		int x = (int)(toPlane*p).x % width;
		int y = (int)(toPlane*p).y % height;

		if(x < 0){x = width + x;}
		if(y < 0){y = height + y;}
		if(Gs[y][x] == -1){return -1;}
		return (Gs[y][x]*this->cr)/256;
	}
    int getB(point c, point p) override {

		if(toPlane.isEmpty()){setMatrix(c,p);}

		int x = (int)(toPlane*p).x % width;
		int y = (int)(toPlane*p).y % height;

		if(x < 0){x = width + x;}
		if(y < 0){y = height + y;}
		if(Bs[y][x] == -1){return -1;}
		return (Bs[y][x]*this->cr)/256;
	}
};

#endif
